#ifndef CUBE_EXPLORER_H
#define CUBE_EXPLORER_H

#define STRATEGY 0

#include<vector>
#include<string>
#include<fstream>
#include<iostream>

//表示某个机械手状态的结构，作为HandState的成员
struct HandStateForOne {
	bool isReady = true;	//机械手是否处于正确的方向（即初始状态的U-D方向，不妨碍另一只机械手的转动）
	bool isTight = true;	//机械手是否夹紧
};

//表示机械手状态的结构，其中left表示左手，right表示右手
struct HandState {
	HandStateForOne left;
	HandStateForOne right;
	//构造方法，满足CubicExplore类的构造函数中默认参数的需求
	HandState(bool lr=true,bool lt=true,bool rr=true,bool rt=true){
		left.isReady = lr;
		left.isTight = lt;
		right.isReady = rr;
		right.isTight = rt;
	}
};

//表示机械手各操作的枚举
enum class Operation {
	R,		//右手顺时针转动
	_R ,	//右手逆时针转动
	F,		//左手顺时针转动
	_F ,	//左手逆时针转动
	R2 ,	//R面转动180°
	F2 ,	//F面转动180°
	LeftLoose,	//左手松开		
	LeftTight,	//左手夹紧
	RightLoose,	//右手松开
	RightTight	//右手夹紧
};

using namespace std;

ostream& operator<<(ostream&, const Operation&);

//表示魔方解决器的一个类
class CubeExplorer {
private:
	vector<string> vecStrSerial;		//保存转换后的串口序列字符串
	vector<string> strNorVec;			//string容器，保存分割后得到的一个个表示单个公式操作的字符串
	vector<Operation> macVec;			//自定义枚举类型Operation容器，保存转换后得到的机械手操作序列
	string target;						//字符串，存储将要处理的普通公式序列
public:
	string SloveString = "";
	HandState handState;	//自定义类HandState类型，表示当前机械手状态
	int transCnt = 0;					//记录魔方翻转次数
	
	CubeExplorer(char* cstr="", HandState& hs=HandState(true,true,true,true));
	
	friend ostream& operator<<(ostream&, const Operation&);		//友元操作符重载函数支持Operation类型和cout的直接使用，主要用于调试

	void SetTarget(string);
	vector<string>& GetVecStrSerial();

	//以下操作定义针对魔方本身，封装机械爪操作
	void OnR(vector<string>::iterator&);		//R面顺时针转动
	void On_R(vector<string>::iterator&);		//R面逆时针转动
	void OnR2(vector<string>::iterator&);		//R面转动180°
	void OnF(vector<string>::iterator&);		//F面类似
	void On_F(vector<string>::iterator&);		//
	void OnF2(vector<string>::iterator&);		//
	void OnRR(vector<string>::iterator&);		//魔方沿R面顺时针方向整体转动
	void On_RR(vector<string>::iterator&);		//魔方沿R面逆时针方向整体转动
	void OnRR2(vector<string>::iterator&);		//魔方沿R面逆时针方向整体转动180°
	void OnFF(vector<string>::iterator&);		//F类似
	void On_FF(vector<string>::iterator&);		//
	void OnFF2(vector<string>::iterator&);		//

	void GetLeftReadyAndTight();	//使左手复位至夹紧且U-D状态(90/270°状态)
	void GetRightReadyAndTight();	//使右手复位至夹紧且U-D状态(90/270°状态)
	void LeftLoose();				//松开左手
	void LeftTight();				//夹紧左手
	void LeftReady();				//左手复位至U-D状态(90/270°状态)
	void RightLoose();				//松开右手
	void RightTight();				//夹紧右手
	void RightReady();				//右手复位至U-D状态(90/270°状态)
	void GetShortestWay();			//核心成员函数，将target中存储的普通公式序列转换为机械手操作序列并存储到macVec容器中
	void ShowOperations();			//打印操作序列，主要用于调试和测试
	void Reset();
	void init();

	//!!!!
	int flag;
	vector<string> mystrNorVec;			//string容器，保存分割后得到的一个个表示单个公式操作的字符串
	vector<Operation> mymacVec;			//自定义枚举类型Operation容器，保存转换后得到的机械手操作序列
	vector<string>::iterator myiter;  // 指向工作位置
	vector< int > length_of_path;
	int target_cnt;           // 记录目前最短步骤
	int operation_count;       // 记录当前步骤数
	int path_count;           //搜索过的路径条数
	int slove_count;           //动作数

	void dfs();  //递归体
	void myshortestway();
	// 空转面改变函数
	void CubeExplorer::spinR();
	void CubeExplorer::spin_R();
	void CubeExplorer::spinR2();
	void CubeExplorer::spinF();
	void CubeExplorer::spin_F();
	void CubeExplorer::spinF2();

	// 状态动作函数层
	void CubeExplorer::R11_1();
	void CubeExplorer::R01_1();
	void CubeExplorer::R10_1();
	void CubeExplorer::_R11_1();
	void CubeExplorer::_R01_1();
	void CubeExplorer::_R10_1();
	void CubeExplorer::R211_1();
	void CubeExplorer::R201_1();
	void CubeExplorer::R210_1();

	void CubeExplorer::F11_1();
	void CubeExplorer::F01_1();
	void CubeExplorer::F10_1();
	void CubeExplorer::_F11_1();
	void CubeExplorer::_F01_1();
	void CubeExplorer::_F10_1();
	void CubeExplorer::F211_1();
	void CubeExplorer::F201_1();
	void CubeExplorer::F210_1();

	void CubeExplorer::B11_1();
	void CubeExplorer::B01_1();
	void CubeExplorer::B10_1();
	void CubeExplorer::_B11_1();
	void CubeExplorer::_B01_1();
	void CubeExplorer::_B10_1();
	void CubeExplorer::B211_1();
	void CubeExplorer::B201_1();
	void CubeExplorer::B210_1();

	void CubeExplorer::L11_1();
	void CubeExplorer::L01_1();
	void CubeExplorer::L10_1();
	void CubeExplorer::_L11_1();
	void CubeExplorer::_L01_1();
	void CubeExplorer::_L10_1();
	void CubeExplorer::L211_1();
	void CubeExplorer::L201_1();
	void CubeExplorer::L210_1();

	void CubeExplorer::U11_1();
	void CubeExplorer::U01_1();
	void CubeExplorer::U10_1();
	void CubeExplorer::_U11_1();
	void CubeExplorer::_U01_1();
	void CubeExplorer::_U10_1();
	void CubeExplorer::U211_1();
	void CubeExplorer::U201_1();
	void CubeExplorer::U210_1();

	void CubeExplorer::U11_2();
	void CubeExplorer::U01_2();
	void CubeExplorer::U10_2();
	void CubeExplorer::_U11_2();
	void CubeExplorer::_U01_2();
	void CubeExplorer::_U10_2();
	void CubeExplorer::U211_2();
	void CubeExplorer::U201_2();
	void CubeExplorer::U210_2();

	void CubeExplorer::D11_1();
	void CubeExplorer::D01_1();
	void CubeExplorer::D10_1();
	void CubeExplorer::_D11_1();
	void CubeExplorer::_D01_1();
	void CubeExplorer::_D10_1();
	void CubeExplorer::D211_1();
	void CubeExplorer::D201_1();
	void CubeExplorer::D210_1();

	void CubeExplorer::D11_2();
	void CubeExplorer::D01_2();
	void CubeExplorer::D10_2();
	void CubeExplorer::_D11_2();
	void CubeExplorer::_D01_2();
	void CubeExplorer::_D10_2();
	void CubeExplorer::D211_2();
	void CubeExplorer::D201_2();
	void CubeExplorer::D210_2();
};


#endif