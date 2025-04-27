#include "CubeRecognizer.h"

static QMap<QString, QMap<QString, HSV>> map_face_color_hsv;		//存放hsv阈值数据的嵌套 map,访问方式为 map[faceName][colorName]
static QList<QString> list_faceID, list_colorID;					//两个下标list，用于通过整数 index 取得对应的字符串作为 map 的key值来访问 map_face_color_hsv;
/*
static QMap<QString, vector<SamRec>> map_pic_id_samRec;				//存放采样框数据的 map ,其中每个元素为存储 samRec 类型的vector，对应一张图片的18个采样位置*/
static QMap<QString, vector<SamRec>> map_pic_id_samRec;				//存放采样框数据的 map ,其中每个元素为存储 samRec 类型的vector，对应一张图片的采样位置
QList<QString> list_picID;											//用于采样框map的下标list，通过整数 index 取得对应的照片名作为 map 的key值来访问 map_pic_id_samRec

static QMap<QString, vector<QString>> map_face_id_recogRes;			//存放识别结果，访问方式为 map[face][blockID]
static QMap<QString, cv::Scalar> map_color_scalar;					//存放预设颜色值，用于绘制识别结果图

static QMap<QString, vector<cv::Mat>> map_face_id_recogMat;		//保存当前识别过程中的采样图片，方便识别结束时查看对比HSV数据

//new things-----------------------------------------------------------
static vector<CubeBlock> vec_samBlocks;								//保存拍摄采样的色块

//vector<pair<QString, int>> vec_index_blockPair;
//---------------------------------------------------------------------

//HSV模块
void iniHSVMap() {
	//初始化整个存储hsv数据的map，先添加六个面对应的次级空map到其中，以面名为key进行索引
	QMap<QString, HSV> map_color_hsv_f, map_color_hsv_r, map_color_hsv_u, map_color_hsv_b, map_color_hsv_l, map_color_hsv_d;
	map_face_color_hsv.insert("f", map_color_hsv_f);
	map_face_color_hsv.insert("r", map_color_hsv_r);
	map_face_color_hsv.insert("u", map_color_hsv_u);
	map_face_color_hsv.insert("b", map_color_hsv_b);
	map_face_color_hsv.insert("l", map_color_hsv_l);
	map_face_color_hsv.insert("d", map_color_hsv_d);

	list_colorID.append("red_h");	list_faceID.append("u"); 		//初始化两个下标list
	list_colorID.append("red_l");	list_faceID.append("r"); 		//
	list_colorID.append("orange");	list_faceID.append("f"); 		//
	list_colorID.append("blue");	list_faceID.append("d"); 		//
	list_colorID.append("green");	list_faceID.append("l"); 		//
	list_colorID.append("yellow");	list_faceID.append("b"); 		//
	list_colorID.append("white");

	//从本地文件读取存储的HSV数据
	QFile file_hsv(QDir::currentPath() + "/data/hsv_threshold.txt");//读取阈值数据文件到 str_hsvData 以进行分割、遍历
	file_hsv.open(QIODevice::ReadWrite);							//
	QTextStream ts(&file_hsv);										//
	QString str_hsvData = ts.readAll();								//


	QList<QString> list_face = str_hsvData.split('~');				//首先根据 '~' 分割得到六个以面为单位的HSV数据
	QList<QString> list_color;
	QList<QString> list_hsv;

	for (int i = 0; i < list_face.length(); i++) {
		list_color = list_face[i].split('-');						//然后根据 '-' 分割得到当前面(i为下标遍历)中以颜色为单位的HSV数据
		map_face_color_hsv[list_faceID[i]].clear();
		for (int j = 0; j < list_color.length(); j++) {
			list_hsv = list_color[j].split('_');					//最后根据 '_' 分割得到对应面(i为下标)中对应颜色(j为下标)的各个HSV阈值

			map_face_color_hsv[list_faceID[i]].insert(				//在 map_face_color_hsv 中的指定面(list_faceID[i]获得key)添加一组HSV数据(key由list_colorID[j]获取)
				list_colorID[j],
				HSV{ list_hsv[0].toInt(),list_hsv[1].toInt(),		//iLowH,iHighH
					list_hsv[2].toInt(),list_hsv[3].toInt(),		//iLowS,iHighS
					list_hsv[4].toInt(),list_hsv[5].toInt() });		//iLowV,iHighV
		}
	}

	file_hsv.close();
}

QMap<QString, QMap<QString, HSV>>& getHSVMap() {
	return map_face_color_hsv;
}

void saveHSVData() {
	//将当前的 map_face_color_hsv 中保存的数据转换为字符串保存到 "./data/hsv_threshold.txt" 文件
	QString data2write;

	QMap<QString, HSV> map_face;
	HSV hsv;

	for (int i = 0; i < map_face_color_hsv.size(); i++) {
		map_face.clear();
		map_face = map_face_color_hsv[list_faceID[i]];						//获取各个面的hsv数据map
		for (int j = 0; j < map_face.size(); j++) {
			hsv = map_face[list_colorID[j]];
			data2write.append(
				QString::number(hsv.iLowH) + "_" + QString::number(hsv.iHighH) + "_" +		//以"iLowH_iHighH_iLowS_iHighS_iLowV_iHighV"的格式存储单个HSV结构
				QString::number(hsv.iLowS) + "_" + QString::number(hsv.iHighS) + "_" +		//
				QString::number(hsv.iLowV) + "_" + QString::number(hsv.iHighV));			//

			if (j != map_face.size() - 1) data2write.append('-');			//除了最后一个颜色之外，都在其后添加一个 '-' 作为颜色的分割
		}
		if (i != map_face_color_hsv.size() - 1) data2write.append('~');		//除了最后一个面之外，都在其后添加一个 '~' 作为面的分割
	}

	QFile::remove(QDir::currentPath() + "/data/hsv_threshold.txt");			//直接删除原来的数据文件

	QFile file_hsv(QDir::currentPath() + "/data/hsv_threshold.txt");		//创建并打开新文件进行输入
	file_hsv.open(QIODevice::WriteOnly);									//
	QTextStream ts(&file_hsv);												//
	ts << data2write;														//

	file_hsv.close();
}

//采样框模块
void iniRecMap() {
	vector<SamRec> sampleRec_FR, sampleRec_U, sampleRec_BL, sampleRec_D;		//存储照片的采样框
	map_pic_id_samRec.insert("FR", sampleRec_FR);
	map_pic_id_samRec.insert("U", sampleRec_U);
	map_pic_id_samRec.insert("BL", sampleRec_BL);
	map_pic_id_samRec.insert("D", sampleRec_D);

	list_picID.append("FR");										//初始化图片 key 下标list，0对应 "FR"
	list_picID.append("U");											//1对应 "U"
	list_picID.append("BL");										//2对应 "BL"
	list_picID.append("D");											//3对应 "D"

	//从本地文件读取存储的采样框数据
	QFile file_rec(QDir::currentPath() + "/data/sample_rec.txt");	//读取采样框数据文件到 str_samRecData 以进行分割、遍历
	file_rec.open(QIODevice::ReadWrite);							//
	QTextStream ts(&file_rec);										//
	QString str_samRecData = ts.readAll();							//

	QList<QString> list_pic = str_samRecData.split('~');			//首先根据 '~' 分割得到三个以图片为单位的采样框数据
	QList<QString> list_id;
	QList<QString> list_rec;

	for (int i = 0; i < list_pic.length(); i++) {
		list_id = list_pic[i].split('-');							//然后根据 '-' 分割得到当前照片名(i为下标遍历)中以照片为单位的采样框数据
		map_pic_id_samRec[list_picID[i]].clear();
		for (int j = 0; j < list_id.length(); j++) {
			list_rec = list_id[j].split('_');						//最后根据 '_' 分割得到对应面(j为下标)中的各个采样框数据

			map_pic_id_samRec[list_picID[i]].push_back(				//在 map_pic_id_samRec 中的指定面(list_picID[i]获得key)添加一组采样框数据(按顺序排序)
				SamRec{ list_rec[0].toInt(),						//x1
						list_rec[1].toInt(),						//x2
						list_rec[2].toInt(),						//y1
						list_rec[3].toInt() });						//y2
		}
	}
}

QMap<QString, vector<SamRec>>& getSamRecMap() {
	return map_pic_id_samRec;
}

void setSampleRec(QString strFaceGroup, SamRec rect2set, int nFaceID, int nBlockID) {
	//将选定的采样框设置为对应面、序号的采样框
//	int index = (nFaceID-1) * 9 + nBlockID - 1;					//获取采样框序号，0~17
	int index = (nFaceID == 2 || nFaceID == 5) ? nBlockID + 8 : nBlockID - 1;	//获取采样框序号
	map_pic_id_samRec[strFaceGroup][index] = rect2set;
	//将新的采样框数据保存到文件 "./data/sample_rec.txt" 文件
	QString data2write;
	vector<SamRec> vec_pic;
	SamRec rec;

	for (int i = 0; i < map_pic_id_samRec.size(); i++) {
		vec_pic = map_pic_id_samRec[list_picID[i]];
		for (int j = 0; j < vec_pic.size(); j++) {
			rec = vec_pic[j];
			data2write.append(
				QString::number(rec.x1) + "_" +
				QString::number(rec.x2) + "_" +
				QString::number(rec.y1) + "_" +
				QString::number(rec.y2));

			if (j != vec_pic.size() - 1) data2write.append('-');
		}
		if (i != map_pic_id_samRec.size() - 1) data2write.append('~');
	}

	QFile::remove(QDir::currentPath() + "/data/sample_rec.txt");			//直接删除原来的数据文件

	QFile file_rec(QDir::currentPath() + "/data/sample_rec.txt");		//创建并打开新文件进行输入
	file_rec.open(QIODevice::WriteOnly);									//
	QTextStream ts(&file_rec);												//
	ts << data2write;														//

	file_rec.close();
}

//识别模块
void iniRecogVars() {
	//初始化识别结果 map 为 6*9 大小
	vector<QString> vec_fill;
	vec_fill.resize(9);
	map_face_id_recogRes.insert("f", vec_fill);
	map_face_id_recogRes.insert("r", vec_fill);
	map_face_id_recogRes.insert("u", vec_fill);
	map_face_id_recogRes.insert("b", vec_fill);
	map_face_id_recogRes.insert("l", vec_fill);
	map_face_id_recogRes.insert("d", vec_fill);

	//初始化绘制识别结果图所用的预设颜色 map
	map_color_scalar.insert("red", cv::Scalar(0, 0, 255));
	map_color_scalar.insert("orange", cv::Scalar(0, 165, 255));
	map_color_scalar.insert("blue", cv::Scalar(255, 0, 0));
	map_color_scalar.insert("green", cv::Scalar(0, 255, 0));
	map_color_scalar.insert("yellow", cv::Scalar(0, 255, 255));
	map_color_scalar.insert("white", cv::Scalar(255, 255, 255));

	map_color_scalar.insert("#FF3030", cv::Scalar(48, 48, 255));
	map_color_scalar.insert("#FF8C00", cv::Scalar(0, 140, 255));
	map_color_scalar.insert("#3A5FCD", cv::Scalar(205, 95, 58));
	map_color_scalar.insert("#32CD32", cv::Scalar(50, 205, 50));
	map_color_scalar.insert("#EEEE00", cv::Scalar(0, 238, 238));
	map_color_scalar.insert("#FFFAFA", cv::Scalar(250, 250, 255));

	//初始化识别采样图存储map
	vector<cv::Mat> vec_mat_fill;
	vec_mat_fill.resize(9);
	map_face_id_recogMat.insert("f", vec_mat_fill);
	map_face_id_recogMat.insert("r", vec_mat_fill);
	map_face_id_recogMat.insert("u", vec_mat_fill);
	map_face_id_recogMat.insert("b", vec_mat_fill);
	map_face_id_recogMat.insert("l", vec_mat_fill);
	map_face_id_recogMat.insert("d", vec_mat_fill);

	//初始化色块容器
	vec_samBlocks.resize(54);									//
	/*vec_index_blockPair.resize(54);
	for (int i = 0; i < vec_index_blockPair.size(); i++) {
		vec_index_blockPair.
	}*/
}

QMap<QString, vector<cv::Mat>>& getLastRecogMatMap() {
	return map_face_id_recogMat;
}

int isColor(cv::Mat imgHSV, QString color, QString face)
{
	HSV hsv;
	if (color == "red") hsv = map_face_color_hsv[face]["red_l"];
	else hsv = map_face_color_hsv[face][color];

	cv::Mat imgThresholded;

	cv::inRange(imgHSV, cv::Scalar(hsv.iLowH, hsv.iLowS, hsv.iLowV), cv::Scalar(hsv.iHighH, hsv.iHighS, hsv.iHighV), imgThresholded); //阈值化处理图片，符合阈值的像素点三通道置为全255

	int count = 0;
	int totalPixNum = imgThresholded.cols * imgThresholded.rows;

	for (int i = 0; i < imgThresholded.rows; i++) {				//遍历阈值化处理后的图片
		cv::Vec3b* p = imgThresholded.ptr<cv::Vec3b>(i);
		for (int j = 0; j < imgThresholded.cols; j++) {
			if (p[j] == cv::Vec3b{ 255,255,255 })				//如果当前像素为(255,255,255)，则计数器加一
				count++;										//
		}
	}

	if (color == "red") {						//红色HSV阈值有两个区间，需要分别进行计算累加
		hsv = map_face_color_hsv[face]["red_h"];
		cv::inRange(imgHSV, cv::Scalar(hsv.iLowH, hsv.iLowS, hsv.iLowV), cv::Scalar(hsv.iHighH, hsv.iHighS, hsv.iHighV), imgThresholded);
		int count1 = 0;
		for (int i = 0; i < imgThresholded.rows; i++) {				//遍历阈值化处理后的图片
			cv::Vec3b* p = imgThresholded.ptr<cv::Vec3b>(i);
			for (int j = 0; j < imgThresholded.cols; j++) {
				if (p[j] == cv::Vec3b{ 255,255,255 })				//如果当前像素为(255,255,255)，则计数器加一
					count1++;										//
			}
		}
		count += count1;
	}

	if (count > totalPixNum * RECOG_SIMILARITY_COE) return 1;		//符合阈值的像素点所占比例大于预设系数，判定取色块为当前颜色
	return 0;													//否则，判定取色块不是当前颜色
}

void judgeColor(cv::Mat image, QString picName, int caseID, int num) {
	vector<SamRec> vec_samRec1, vec_samRec2;				//分别存放picName指定图片所采样的采样框数据，从小到大需满足kociemba算法序号要求
	vector<vector<SamRec>> vec_samRec;						//存放picName指定图片所采样的两个面的采样框数据，从小到大需满足kociemba算法序号要求
	vector<int> vec_specialBlockID;							//存放特殊块（在case1中会被遮挡的快）的序号
	//首先根据kociemba算法对魔方块排序的要求，将对应面的采样框按照序号放到vec_samRec临时容器中
	if (picName == "FR") {
		//F面的采样框数据
		vec_samRec1.push_back(map_pic_id_samRec["FR"][0]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][1]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][2]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][3]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][4]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][5]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][6]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][7]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][8]);
		vec_samRec.push_back(vec_samRec1);
		//R面的采样框数据
		vec_samRec2.push_back(map_pic_id_samRec["FR"][9]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][10]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][11]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][12]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][13]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][14]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][15]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][16]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][17]);
		vec_samRec.push_back(vec_samRec2);

		//F面的1,2,7,8号块会在case1中被遮挡
		//R面的2,3,8,9号块会在case1中被遮挡
		vec_specialBlockID.push_back(1);
		vec_specialBlockID.push_back(2);
		vec_specialBlockID.push_back(7);
		vec_specialBlockID.push_back(8);
		vec_specialBlockID.push_back(11);
		vec_specialBlockID.push_back(12);
		vec_specialBlockID.push_back(17);
		vec_specialBlockID.push_back(18);
	}
	else if (picName == "U") {
		//U面采样框数据
		vec_samRec1.push_back(map_pic_id_samRec["U"][0]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][1]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][2]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][3]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][4]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][5]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][6]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][7]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][8]);
		vec_samRec.push_back(vec_samRec1);

		//U面的6,8号块会在case1中被遮挡
		vec_specialBlockID.push_back(6);
		vec_specialBlockID.push_back(8);
	}
	else if (picName == "BL") {
		//B面采样框数据
		vec_samRec1.push_back(map_pic_id_samRec["BL"][0]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][1]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][2]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][3]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][4]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][5]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][6]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][7]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][8]);
		vec_samRec.push_back(vec_samRec1);
		//L面采样框数据
		vec_samRec2.push_back(map_pic_id_samRec["BL"][9]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][10]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][11]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][12]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][13]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][14]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][15]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][16]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][17]);
		vec_samRec.push_back(vec_samRec2);
	}
	else if (picName == "D") {
		//D面采样框数据
		vec_samRec2.push_back(map_pic_id_samRec["D"][0]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][1]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][2]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][3]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][4]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][5]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][6]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][7]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][8]);
		vec_samRec.push_back(vec_samRec2);

		//D面的2,6会在case1中被遮挡
		vec_specialBlockID.push_back(2);
		vec_specialBlockID.push_back(6);
	}

	//然后根据不同的case进行不同的识别方案
	//case1时，忽略special块，仅识别未被遮挡的块
	//case2时，忽略case1能够处理的色块，仅识别special块
	cv::Mat mat_t, mat_hsv;
	QString faceName;
	vector<SamRec> vec_samRec_t;
	for (int faceID = 0; faceID < num; faceID++) {
		faceName = picName[faceID].toLower();
		vec_samRec_t = vec_samRec[faceID];
		for (int i = 0; i < vec_samRec_t.size(); i++) {
			//判断是否忽略当前色块，关键点是利用抑或条件使得两种case得到不同的结果：
			//case1，当前序号若存在于specialBlockID容器中，判断结果为 1^0=1，continue进行忽略；否则判断结果为 0^0=0
			//case2，当前序号若不存在于specialBlockID容器中，判断结果为 0^1=1，continue进行忽略；否则判断结果为 1^1=0
			if (std::find(vec_specialBlockID.cbegin(), vec_specialBlockID.cend(), (faceID * 9 + i)) != vec_specialBlockID.cend() ^ (caseID == 2)) continue;

			//截取得到采样框内的图片并进行转换、识别
			mat_t = image(cv::Range(vec_samRec_t[i].y1, vec_samRec_t[i].y2), cv::Range(vec_samRec_t[i].x1, vec_samRec_t[i].x2));
			map_face_id_recogMat[faceName][i] = mat_t;														//将采样图保存到map，以供识别结束之后的调试使用
			cv::cvtColor(mat_t, mat_hsv, cv::COLOR_BGR2HSV);													//转换为HSV图片
			if (isColor(mat_hsv, "red", faceName)) map_face_id_recogRes[faceName][i] = "red";					//穷举法判断当前块属于哪一个颜色
			else if (isColor(mat_hsv, "orange", faceName)) map_face_id_recogRes[faceName][i] = "orange";		//
			else if (isColor(mat_hsv, "blue", faceName)) map_face_id_recogRes[faceName][i] = "blue";			//
			else if (isColor(mat_hsv, "green", faceName)) map_face_id_recogRes[faceName][i] = "green";			//
			else if (isColor(mat_hsv, "yellow", faceName)) map_face_id_recogRes[faceName][i] = "yellow";		//
			else if (isColor(mat_hsv, "white", faceName)) map_face_id_recogRes[faceName][i] = "white";			//
			else map_face_id_recogRes[faceName][i] = "unkown";													//若在穷举范围内没有找到结果，则存储为"unkown"
		}
	}
}

//HSV排序法识别
string recognizeNew() { // ！！重要算法
	vector<CubeBlock> vec_red_orange_blocks;

	//获取所有的采样块并按kociemba要求的顺序存储到容器中
	for (int i = 0; i < vec_samBlocks.size(); i++) {
		vec_samBlocks[i].matSample = map_face_id_recogMat[list_faceID[i / 9]][i % 9];
		vec_samBlocks[i].bJudged = false;
		vec_samBlocks[i].index = i;														//记录初始序列号，用于排序识别结果的赋值
	}

	int cntWhite = 9, cntRed = 9, cntOrange = 9, cntYellow = 9, cntGreen = 9, cntBlue = 9;
	//ofstream in;
	//in.open(".\\Mechanical\\test.txt", ios::trunc);
	//int cnt1 = 0;
	//计算得到各个采样图的HSV均值，HSV有色调、饱和度、亮度三维，通过取平均使HSV值平均化用于排序
	for (int index = 0; index < vec_samBlocks.size(); index++) {
		int sumH = 0, sumS = 0, sumV = 0;
		int cntH = 0, cntS = 0, cntV = 0;
		//int cntH170 = 0;
		//cnt1++;
		cv::Mat mat_hsv;
		cv::cvtColor(vec_samBlocks[index].matSample, mat_hsv, cv::COLOR_BGR2HSV);
		for (int i = 0; i < mat_hsv.rows; i++) {				//遍历图片每个像素
			for (int j = 0; j < mat_hsv.cols; j++) {
				cv::Point p(j, i);
				if (mat_hsv.at<cv::Vec3b>(p)[0] >= 0 && mat_hsv.at<cv::Vec3b>(p)[0] <= 150) {
					sumH += mat_hsv.at<cv::Vec3b>(p)[0];
					cntH++;
				}
				if (mat_hsv.at<cv::Vec3b>(p)[1] >= 0 && mat_hsv.at<cv::Vec3b>(p)[1] <= 255) {
					sumS += mat_hsv.at<cv::Vec3b>(p)[1];
					cntS++;
				}
				if (mat_hsv.at<cv::Vec3b>(p)[2] >= 0 && mat_hsv.at<cv::Vec3b>(p)[2] <= 255) {
					sumV += mat_hsv.at<cv::Vec3b>(p)[2];
					cntV++;
				}
			}
		}
		vec_samBlocks[index].meanH = (float)sumH / cntH;
		vec_samBlocks[index].meanS = (float)sumS / cntS;
		vec_samBlocks[index].meanV = (float)sumV / cntV;
		//vec_samBlocks[index].ratioH170 = (float)cntH170 / cntH;

		//if ((float)cntH150 / cntH > 0.03) {							//挑选出H在170-180之间的红色块，避免区间跳跃导致的均值失真
		//	vec_samBlocks[index].bJudged = true;
		//	strResult[index] = 'r';
		//	cntRed--;
		//}
	}
	//in << cnt1 << endl;
	//int cntjudge = 0;
	//对S进行排序，取最小的6个作为白色块
	sort(vec_samBlocks.begin(), vec_samBlocks.end(), [](CubeBlock& a, CubeBlock& b) {return a.meanS < b.meanS; });

	int i = 0;
	while (cntWhite > 0 && i < vec_samBlocks.size()) {
		if (vec_samBlocks[i].bJudged == false) {
			vec_samBlocks[i].bJudged = true;
			int index = vec_samBlocks[i].index;
			map_face_id_recogRes[list_faceID[index / 9]][index % 9] = "white";
			cntWhite--;
			//cntjudge++;
		}
		i++;
	}
	//in << "cntjudge = " << cntjudge << endl;
	//in << "i = " << i << endl;
	//对H进行递减排序，挑选出蓝色、绿色、黄色块。从大到小依次为蓝色、绿色、黄色
	sort(vec_samBlocks.begin(), vec_samBlocks.end(), [](CubeBlock& a, CubeBlock& b) {return a.meanH > b.meanH; });

	for (int i = 0; i < vec_samBlocks.size(); i++) {
		if (vec_samBlocks[i].bJudged == false) {
			//cntjudge++;
			int index = vec_samBlocks[i].index;
			if (cntBlue > 0) {
				vec_samBlocks[i].bJudged = true;
				map_face_id_recogRes[list_faceID[index / 9]][index % 9] = "blue";
				cntBlue--;
			}
			else if (cntGreen > 0) {
				vec_samBlocks[i].bJudged = true;
				map_face_id_recogRes[list_faceID[index / 9]][index % 9] = "green";
				cntGreen--;
			}
			else if (cntYellow > 0) {
				vec_samBlocks[i].bJudged = true;
				map_face_id_recogRes[list_faceID[index / 9]][index % 9] = "yellow";
				cntYellow--;
			}
			else {
				//map_face_id_recogRes[list_faceID[index / 9]][index % 9] = "red/orange";
				vec_red_orange_blocks.push_back(vec_samBlocks[i]);
			}
			/*	else if (cntRed > 0) {
					vec_samBlocks[i].bJudged = true;
					map_face_id_recogRes[list_faceID[index / 9]][index % 9] = "red";
					cntRed--;
				}
				else if (cntOrange > 0) {
					vec_samBlocks[i].bJudged = true;
					map_face_id_recogRes[list_faceID[index / 9]][index % 9] = "orange";
					cntOrange--;
				}*/
		}
	}
	//in << "cntjudge = " << cntjudge << endl;
	//in << "size = " << vec_red_orange_blocks.size() << endl;

	//最后处理未判定的红色、橙色块
	//方案一：强光照射下，红橙色H值在某些面由于各方向的反射光，很难区分。但是可以发现亮度值橙色明显高于红色，因此可以对V值进行排序，区分出红色橙色。
	sort(vec_red_orange_blocks.begin(), vec_red_orange_blocks.end(), [](CubeBlock& a, CubeBlock& b) {return a.meanV < b.meanV; });
	

	for (int i = 0; i < vec_red_orange_blocks.size(); i++) {
		int index = vec_red_orange_blocks[i].index;
		if (i < vec_red_orange_blocks.size() / 2) {
			map_face_id_recogRes[list_faceID[index / 9]][index % 9] = "red";
		}
		else {
			map_face_id_recogRes[list_faceID[index / 9]][index % 9] = "orange";
		}
	}

	//根据识别结果绘制识别结果示意图，存放到本地文件夹"./pic_res"
	//并对识别结果进行排列得到 kociemba 算法所需要的字符串序列（U-R-F-D-L-B）
	QString str_res="";
	QMap<QString, QString> map_color_face;
	for (int i = 0; i < 6; i++) {
		map_color_face.insert(map_face_id_recogRes[list_faceID[i]][4], list_faceID[i]);		//存储各面中心块颜色，用以得到色块序号（U-R-F-D-L-B）
	}
	//in << "mapsize = " << map_face_id_recogRes.size() << endl;
	//cnt1 = 0;
	for (int i = 0; i < map_face_id_recogRes.size(); i++) {
		vector<QString>& vec_res = map_face_id_recogRes[list_faceID[i]];
		cv::Mat mat_res(360, 360, CV_8UC3, cv::Scalar(255, 255, 255));
		for (int j = 0; j < vec_res.size(); j++) {
			switch (j) {
			case(0): rectangle(mat_res, cv::Rect(0, 0, 120, 120), map_color_scalar[vec_res[j]], -1, 1, 0);
			case(1): rectangle(mat_res, cv::Rect(120, 0, 240, 120), map_color_scalar[vec_res[j]], -1, 1, 0);
			case(2): rectangle(mat_res, cv::Rect(240, 0, 360, 120), map_color_scalar[vec_res[j]], -1, 1, 0);
			case(3): rectangle(mat_res, cv::Rect(0, 120, 120, 240), map_color_scalar[vec_res[j]], -1, 1, 0);
			case(4): rectangle(mat_res, cv::Rect(120, 120, 240, 240), map_color_scalar[vec_res[j]], -1, 1, 0);
			case(5): rectangle(mat_res, cv::Rect(240, 120, 360, 240), map_color_scalar[vec_res[j]], -1, 1, 0);
			case(6): rectangle(mat_res, cv::Rect(0, 240, 120, 360), map_color_scalar[vec_res[j]], -1, 1, 0);
			case(7): rectangle(mat_res, cv::Rect(120, 240, 240, 360), map_color_scalar[vec_res[j]], -1, 1, 0);
			case(8): rectangle(mat_res, cv::Rect(240, 240, 360, 360), map_color_scalar[vec_res[j]], -1, 1, 0);
			}
			//in << vec_res[j].toStdString() << " ";
			str_res.append(map_color_face[vec_res[j]]);		//单个面内从0-8依次排列即可
		}
		//in << endl;
		line(mat_res, cv::Point(0, 120), cv::Point(360, 120), cv::Scalar(0, 0, 0), 2, 8, 0); //绘制分隔色块的黑线，linewidth = 2, linetype = 8, 0表示非反锯齿
		line(mat_res, cv::Point(0, 240), cv::Point(360, 240), cv::Scalar(0, 0, 0), 2, 8, 0); //Scalar函数 S(x）生成灰度颜色， S(r,g,b)三通道 ,S(r,g,b,a) 三通道+透明度
		line(mat_res, cv::Point(120, 0), cv::Point(120, 360), cv::Scalar(0, 0, 0), 2, 8, 0);
		line(mat_res, cv::Point(240, 0), cv::Point(240, 360), cv::Scalar(0, 0, 0), 2, 8, 0);

		imwrite("pic_res/res_" + list_faceID[i].toStdString() + ".png", mat_res);
	}
	//in.close();
	return str_res.toUpper().toStdString();


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//string strResult(54,'u');


	//int startIndex;

	////FR
	//vector<SamRec> vec_samRect = map_pic_id_samRec["FR"];
	////case1
	//image = cv::imread("pic_cam/cam_case1_FR.jpg");
	//cv::resize(image, image, cv::Size(640, 480));
	//startIndex = 18;										//F面
	//vec_samBlocks[startIndex + 2].matSample = image(cv::Range(vec_samRect[2].y1, vec_samRect[2].y2), cv::Range(vec_samRect[2].x1, vec_samRect[2].x2));
	//vec_samBlocks[startIndex + 3].matSample = image(cv::Range(vec_samRect[3].y1, vec_samRect[3].y2), cv::Range(vec_samRect[3].x1, vec_samRect[3].x2));
	//vec_samBlocks[startIndex + 4].matSample = image(cv::Range(vec_samRect[4].y1, vec_samRect[4].y2), cv::Range(vec_samRect[4].x1, vec_samRect[4].x2));
	//vec_samBlocks[startIndex + 5].matSample = image(cv::Range(vec_samRect[5].y1, vec_samRect[5].y2), cv::Range(vec_samRect[5].x1, vec_samRect[5].x2));
	//vec_samBlocks[startIndex + 8].matSample = image(cv::Range(vec_samRect[8].y1, vec_samRect[8].y2), cv::Range(vec_samRect[8].x1, vec_samRect[8].x2));
	//startIndex = 9;											//R面
	//vec_samBlocks[startIndex + 0].matSample = image(cv::Range(vec_samRect[9].y1, vec_samRect[9].y2), cv::Range(vec_samRect[9].x1, vec_samRect[9].x2));
	//vec_samBlocks[startIndex + 1].matSample = image(cv::Range(vec_samRect[10].y1, vec_samRect[10].y2), cv::Range(vec_samRect[10].x1, vec_samRect[10].x2));
	//vec_samBlocks[startIndex + 2].matSample = image(cv::Range(vec_samRect[11].y1, vec_samRect[11].y2), cv::Range(vec_samRect[11].x1, vec_samRect[11].x2));
	//vec_samBlocks[startIndex + 3].matSample = image(cv::Range(vec_samRect[12].y1, vec_samRect[12].y2), cv::Range(vec_samRect[12].x1, vec_samRect[12].x2));
	//vec_samBlocks[startIndex + 4].matSample = image(cv::Range(vec_samRect[13].y1, vec_samRect[13].y2), cv::Range(vec_samRect[13].x1, vec_samRect[13].x2));
	//vec_samBlocks[startIndex + 5].matSample = image(cv::Range(vec_samRect[14].y1, vec_samRect[14].y2), cv::Range(vec_samRect[14].x1, vec_samRect[14].x2));
	//vec_samBlocks[startIndex + 6].matSample = image(cv::Range(vec_samRect[15].y1, vec_samRect[15].y2), cv::Range(vec_samRect[15].x1, vec_samRect[15].x2));
	//vec_samBlocks[startIndex + 7].matSample = image(cv::Range(vec_samRect[16].y1, vec_samRect[16].y2), cv::Range(vec_samRect[16].x1, vec_samRect[16].x2));
	//vec_samBlocks[startIndex + 8].matSample = image(cv::Range(vec_samRect[17].y1, vec_samRect[17].y2), cv::Range(vec_samRect[17].x1, vec_samRect[17].x2));

	////case2，补充之前被遮挡的色块
	//image = cv::imread("pic_cam/cam_case2_FR.jpg");
	//cv::resize(image, image, cv::Size(640, 480));
	//startIndex = 18;										//F面
	//vec_samBlocks[startIndex + 0].matSample = image(cv::Range(vec_samRect[0].y1, vec_samRect[0].y2), cv::Range(vec_samRect[0].x1, vec_samRect[0].x2));
	//vec_samBlocks[startIndex + 1].matSample = image(cv::Range(vec_samRect[1].y1, vec_samRect[1].y2), cv::Range(vec_samRect[1].x1, vec_samRect[1].x2));
	//vec_samBlocks[startIndex + 6].matSample = image(cv::Range(vec_samRect[6].y1, vec_samRect[6].y2), cv::Range(vec_samRect[6].x1, vec_samRect[6].x2));
	//vec_samBlocks[startIndex + 7].matSample = image(cv::Range(vec_samRect[7].y1, vec_samRect[7].y2), cv::Range(vec_samRect[7].x1, vec_samRect[7].x2));

	////UB
	//vec_samRect = map_pic_id_samRec["UB"];
	////case1
	//image = cv::imread("pic_cam/cam_case1_UB.jpg");
	//cv::resize(image, image, cv::Size(640, 480));
	//startIndex = 0;											//U面
	//vec_samBlocks[startIndex + 0].matSample = image(cv::Range(vec_samRect[2].y1, vec_samRect[2].y2), cv::Range(vec_samRect[2].x1, vec_samRect[2].x2));
	//vec_samBlocks[startIndex + 1].matSample = image(cv::Range(vec_samRect[5].y1, vec_samRect[5].y2), cv::Range(vec_samRect[5].x1, vec_samRect[5].x2));
	//vec_samBlocks[startIndex + 2].matSample = image(cv::Range(vec_samRect[8].y1, vec_samRect[8].y2), cv::Range(vec_samRect[8].x1, vec_samRect[8].x2));
	//vec_samBlocks[startIndex + 3].matSample = image(cv::Range(vec_samRect[1].y1, vec_samRect[1].y2), cv::Range(vec_samRect[1].x1, vec_samRect[1].x2));
	//vec_samBlocks[startIndex + 4].matSample = image(cv::Range(vec_samRect[4].y1, vec_samRect[4].y2), cv::Range(vec_samRect[4].x1, vec_samRect[4].x2));
	//vec_samBlocks[startIndex + 5].matSample = image(cv::Range(vec_samRect[7].y1, vec_samRect[7].y2), cv::Range(vec_samRect[7].x1, vec_samRect[7].x2));
	//vec_samBlocks[startIndex + 6].matSample = image(cv::Range(vec_samRect[0].y1, vec_samRect[0].y2), cv::Range(vec_samRect[0].x1, vec_samRect[0].x2));
	//vec_samBlocks[startIndex + 8].matSample = image(cv::Range(vec_samRect[6].y1, vec_samRect[6].y2), cv::Range(vec_samRect[6].x1, vec_samRect[6].x2));
	//startIndex = 45;										//B面
	//vec_samBlocks[startIndex + 0].matSample = image(cv::Range(vec_samRect[15].y1, vec_samRect[15].y2), cv::Range(vec_samRect[15].x1, vec_samRect[15].x2));
	//vec_samBlocks[startIndex + 1].matSample = image(cv::Range(vec_samRect[12].y1, vec_samRect[12].y2), cv::Range(vec_samRect[12].x1, vec_samRect[12].x2));
	//vec_samBlocks[startIndex + 2].matSample = image(cv::Range(vec_samRect[9].y1, vec_samRect[9].y2), cv::Range(vec_samRect[9].x1, vec_samRect[9].x2));
	//vec_samBlocks[startIndex + 3].matSample = image(cv::Range(vec_samRect[16].y1, vec_samRect[16].y2), cv::Range(vec_samRect[16].x1, vec_samRect[16].x2));
	//vec_samBlocks[startIndex + 4].matSample = image(cv::Range(vec_samRect[13].y1, vec_samRect[13].y2), cv::Range(vec_samRect[13].x1, vec_samRect[13].x2));
	//vec_samBlocks[startIndex + 5].matSample = image(cv::Range(vec_samRect[10].y1, vec_samRect[10].y2), cv::Range(vec_samRect[10].x1, vec_samRect[10].x2));
	//vec_samBlocks[startIndex + 6].matSample = image(cv::Range(vec_samRect[17].y1, vec_samRect[17].y2), cv::Range(vec_samRect[17].x1, vec_samRect[17].x2));
	//vec_samBlocks[startIndex + 7].matSample = image(cv::Range(vec_samRect[14].y1, vec_samRect[14].y2), cv::Range(vec_samRect[14].x1, vec_samRect[14].x2));
	//vec_samBlocks[startIndex + 8].matSample = image(cv::Range(vec_samRect[11].y1, vec_samRect[11].y2), cv::Range(vec_samRect[11].x1, vec_samRect[11].x2));

	////case2，补充之前被遮挡的色块
	//image = cv::imread("pic_cam/cam_case2_UB.jpg");
	//cv::resize(image, image, cv::Size(640, 480));
	//startIndex = 0;											//U面
	//vec_samBlocks[startIndex + 7].matSample = image(cv::Range(vec_samRect[3].y1, vec_samRect[3].y2), cv::Range(vec_samRect[3].x1, vec_samRect[3].x2));

	////LD
	//vec_samRect = map_pic_id_samRec["LD"];
	////case1
	//image = cv::imread("pic_cam/cam_case1_LD.jpg");
	//cv::resize(image, image, cv::Size(640, 480));
	//startIndex = 36;										//L面
	//vec_samBlocks[startIndex + 0].matSample = image(cv::Range(vec_samRect[11].y1, vec_samRect[11].y2), cv::Range(vec_samRect[11].x1, vec_samRect[11].x2));
	//vec_samBlocks[startIndex + 1].matSample = image(cv::Range(vec_samRect[14].y1, vec_samRect[14].y2), cv::Range(vec_samRect[14].x1, vec_samRect[14].x2));
	//vec_samBlocks[startIndex + 2].matSample = image(cv::Range(vec_samRect[17].y1, vec_samRect[17].y2), cv::Range(vec_samRect[17].x1, vec_samRect[17].x2));
	//vec_samBlocks[startIndex + 3].matSample = image(cv::Range(vec_samRect[10].y1, vec_samRect[10].y2), cv::Range(vec_samRect[10].x1, vec_samRect[10].x2));
	//vec_samBlocks[startIndex + 4].matSample = image(cv::Range(vec_samRect[13].y1, vec_samRect[13].y2), cv::Range(vec_samRect[13].x1, vec_samRect[13].x2));
	//vec_samBlocks[startIndex + 5].matSample = image(cv::Range(vec_samRect[16].y1, vec_samRect[16].y2), cv::Range(vec_samRect[16].x1, vec_samRect[16].x2));
	//vec_samBlocks[startIndex + 6].matSample = image(cv::Range(vec_samRect[9].y1, vec_samRect[9].y2), cv::Range(vec_samRect[9].x1, vec_samRect[9].x2));
	//vec_samBlocks[startIndex + 7].matSample = image(cv::Range(vec_samRect[12].y1, vec_samRect[12].y2), cv::Range(vec_samRect[12].x1, vec_samRect[12].x2));
	//vec_samBlocks[startIndex + 8].matSample = image(cv::Range(vec_samRect[15].y1, vec_samRect[15].y2), cv::Range(vec_samRect[15].x1, vec_samRect[15].x2));
	//startIndex = 27;										//D面
	//vec_samBlocks[startIndex + 0].matSample = image(cv::Range(vec_samRect[8].y1, vec_samRect[8].y2), cv::Range(vec_samRect[8].x1, vec_samRect[8].x2));
	//vec_samBlocks[startIndex + 3].matSample = image(cv::Range(vec_samRect[5].y1, vec_samRect[5].y2), cv::Range(vec_samRect[5].x1, vec_samRect[5].x2));
	//vec_samBlocks[startIndex + 4].matSample = image(cv::Range(vec_samRect[4].y1, vec_samRect[4].y2), cv::Range(vec_samRect[4].x1, vec_samRect[4].x2));
	//vec_samBlocks[startIndex + 5].matSample = image(cv::Range(vec_samRect[3].y1, vec_samRect[3].y2), cv::Range(vec_samRect[3].x1, vec_samRect[3].x2));
	//vec_samBlocks[startIndex + 6].matSample = image(cv::Range(vec_samRect[2].y1, vec_samRect[2].y2), cv::Range(vec_samRect[2].x1, vec_samRect[2].x2));
	//vec_samBlocks[startIndex + 7].matSample = image(cv::Range(vec_samRect[1].y1, vec_samRect[1].y2), cv::Range(vec_samRect[1].x1, vec_samRect[1].x2));
	//vec_samBlocks[startIndex + 8].matSample = image(cv::Range(vec_samRect[0].y1, vec_samRect[0].y2), cv::Range(vec_samRect[0].x1, vec_samRect[0].x2));

	////case2，补充之前被遮挡的色块
	//image = cv::imread("pic_cam/cam_case2_LD.jpg");
	//cv::resize(image, image, cv::Size(640, 480));
	//startIndex = 27;										//D面
	//vec_samBlocks[startIndex + 1].matSample = image(cv::Range(vec_samRect[7].y1, vec_samRect[7].y2), cv::Range(vec_samRect[7].x1, vec_samRect[7].x2));
	//vec_samBlocks[startIndex + 2].matSample = image(cv::Range(vec_samRect[6].y1, vec_samRect[6].y2), cv::Range(vec_samRect[6].x1, vec_samRect[6].x2));

	//for (int i = 0; i < vec_samBlocks.size(); i++) {		//每个色块记录自己原始的位置
	//	vec_samBlocks[i].index = i;							//
	//}
}

string FaceToColor(char s)
{
	string str;
	switch (s) {
	case 'F':str = "#EEEE00"; break;
	case 'U':str = "#32CD32"; break;
	case 'L':str = "#FF8C00"; break;
	case 'R':str = "#FF3030"; break;
	case 'D':str = "#3A5FCD"; break;
	case 'B':str = "#FFFAFA"; break;
	}
	return str;
}

void CreateSceneFromStr(string str)
{
	for (int i = 0; i < 6; i++) {
		cv::Mat mat_res(360, 360, CV_8UC3, cv::Scalar(255, 255, 255));
		for (int j = 0; j < 9; j++) {
			string colorStr = FaceToColor(str[i * 9 + j]);
			switch (j) {
			case(0): rectangle(mat_res, cv::Rect(0, 0, 120, 120), map_color_scalar[colorStr.c_str()], -1, 1, 0);
			case(1): rectangle(mat_res, cv::Rect(120, 0, 240, 120), map_color_scalar[colorStr.c_str()], -1, 1, 0);
			case(2): rectangle(mat_res, cv::Rect(240, 0, 360, 120), map_color_scalar[colorStr.c_str()], -1, 1, 0);
			case(3): rectangle(mat_res, cv::Rect(0, 120, 120, 240), map_color_scalar[colorStr.c_str()], -1, 1, 0);
			case(4): rectangle(mat_res, cv::Rect(120, 120, 240, 240), map_color_scalar[colorStr.c_str()], -1, 1, 0);
			case(5): rectangle(mat_res, cv::Rect(240, 120, 360, 240), map_color_scalar[colorStr.c_str()], -1, 1, 0);
			case(6): rectangle(mat_res, cv::Rect(0, 240, 120, 360), map_color_scalar[colorStr.c_str()], -1, 1, 0);
			case(7): rectangle(mat_res, cv::Rect(120, 240, 240, 360), map_color_scalar[colorStr.c_str()], -1, 1, 0);
			case(8): rectangle(mat_res, cv::Rect(240, 240, 360, 360), map_color_scalar[colorStr.c_str()], -1, 1, 0);
			}
		}
		line(mat_res, cv::Point(0, 120), cv::Point(360, 120), cv::Scalar(0, 0, 0), 4, 8, 0); //绘制分隔色块的黑线，linewidth = 2, linetype = 8, 0表示非反锯齿
		line(mat_res, cv::Point(0, 240), cv::Point(360, 240), cv::Scalar(0, 0, 0), 4, 8, 0); //Scalar函数 S(x）生成灰度颜色， S(r,g,b)三通道 ,S(r,g,b,a) 三通道+透明度
		line(mat_res, cv::Point(120, 0), cv::Point(120, 360), cv::Scalar(0, 0, 0), 4, 8, 0);
		line(mat_res, cv::Point(240, 0), cv::Point(240, 360), cv::Scalar(0, 0, 0), 4, 8, 0);

		imwrite("pic_res/res_" + list_faceID[i].toStdString() + ".png", mat_res);
	}
}

/// 函数功能：从图片中采样色块，并保存到vec_samBlocks中
void sampleFromPic(cv::Mat image, QString picName) { // 数据在image中，num表示包含面的数量
	vector<SamRec> vec_samRec1, vec_samRec2; //分别存放picName指定图片所采样的两个面的采样框数据，从小到大需满足kociemba算法序号要求
	vector<vector<SamRec>> vec_samRec;	//存放picName指定图片所采样的两个面的采样框数据，从小到大需满足kociemba算法序号要求
	//vector<int> vec_specialBlockID; //存放特殊块（在case1中会被遮挡的快）的序号(0~17)
	//首先根据kociemba算法对魔方块排序的要求，将对应面的采样框按照序号放到vec_samRec临时容器中
	int num;
	if (picName == "FR") {
		//F面的采样框数据
		vec_samRec1.push_back(map_pic_id_samRec["FR"][0]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][1]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][2]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][3]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][4]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][5]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][6]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][7]);
		vec_samRec1.push_back(map_pic_id_samRec["FR"][8]);
		vec_samRec.push_back(vec_samRec1);

		vec_samRec2.push_back(map_pic_id_samRec["FR"][9]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][10]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][11]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][12]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][13]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][14]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][15]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][16]);
		vec_samRec2.push_back(map_pic_id_samRec["FR"][17]);
		vec_samRec.push_back(vec_samRec2);
		num = 2;

	}
	else if (picName == "U") {
		//U面采样框数据
		vec_samRec1.push_back(map_pic_id_samRec["U"][0]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][1]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][2]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][3]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][4]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][5]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][6]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][7]);
		vec_samRec1.push_back(map_pic_id_samRec["U"][8]);
		vec_samRec.push_back(vec_samRec1);

		num = 1;

		//U面的6,8号块会在case1中被遮挡
		//vec_specialBlockID.push_back(6);
		//vec_specialBlockID.push_back(8);
	}
	else if (picName == "BL") {
		//B面采样框数据
		vec_samRec1.push_back(map_pic_id_samRec["BL"][0]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][1]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][2]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][3]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][4]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][5]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][6]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][7]);
		vec_samRec1.push_back(map_pic_id_samRec["BL"][8]);
		vec_samRec.push_back(vec_samRec1);
		//L面采样框数据
		vec_samRec2.push_back(map_pic_id_samRec["BL"][9]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][10]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][11]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][12]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][13]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][14]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][15]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][16]);
		vec_samRec2.push_back(map_pic_id_samRec["BL"][17]);
		vec_samRec.push_back(vec_samRec2);

		num = 2;
	}
	else if (picName == "D") {
		//D面采样框数据
		vec_samRec2.push_back(map_pic_id_samRec["D"][0]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][1]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][2]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][3]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][4]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][5]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][6]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][7]);
		vec_samRec2.push_back(map_pic_id_samRec["D"][8]);
		vec_samRec.push_back(vec_samRec2);

		num = 1;

		//D面的2,6会在case1中被遮挡
		//vec_specialBlockID.push_back(2);
		//vec_specialBlockID.push_back(6);
	}

	//然后根据不同的case进行不同的识别方案
	//case1时，忽略special块，仅识别未被遮挡的块
	//case2时，忽略case1能够处理的色块，仅识别special块
	cv::Mat mat_t, mat_hsv;
	QString faceName;
	vector<SamRec> vec_samRec_t;

	for (int faceID = 0; faceID < num; faceID++) { //FR
		faceName = picName[faceID].toLower();
		vec_samRec_t = vec_samRec[faceID];
		for (int i = 0; i < vec_samRec_t.size(); i++) {
	
			//截取得到采样框内的图片并进行转换、识别
			mat_t = image(cv::Range(vec_samRec_t[i].y1, vec_samRec_t[i].y2), cv::Range(vec_samRec_t[i].x1, vec_samRec_t[i].x2));
			map_face_id_recogMat[faceName][i] = mat_t;	//将采样图保存到map，以供识别结束之后的调试使用
		}
	}
}
