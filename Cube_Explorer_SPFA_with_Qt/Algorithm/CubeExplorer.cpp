#include"CubeExplorer.h"

CubeExplorer::CubeExplorer(char* cstr,HandState& hs):target(cstr),handState(hs){}

void CubeExplorer::SetTarget(string str) { target = str; }

vector<string>& CubeExplorer::GetVecStrSerial() { return vecStrSerial; }

void CubeExplorer::init() {
}


void CubeExplorer::OnR(vector<string>::iterator& iter) {
	RightTight();											//右爪夹紧
	GetLeftReadyAndTight();									//左爪调整至90/270°且夹紧
	macVec.push_back(Operation::R);							//右爪顺时针转动90°（存储到容器内，之后进行发送）
	handState.right.isReady = !handState.right.isReady;		//设置右爪状态参数

	transCnt++;
}

void CubeExplorer::On_R(vector<string>::iterator& iter) {
	RightTight();
	GetLeftReadyAndTight();
	macVec.push_back(Operation::_R);
	handState.right.isReady = !handState.right.isReady; 

	transCnt++;
}

void CubeExplorer::OnF(vector<string>::iterator& iter) {
	LeftTight();
	GetRightReadyAndTight();
	macVec.push_back(Operation::F);
	handState.left.isReady = !handState.left.isReady;

	transCnt++;
}

void CubeExplorer::On_F(vector<string>::iterator& iter) {
	LeftTight();
	GetRightReadyAndTight();
	macVec.push_back(Operation::_F);
	handState.left.isReady = !handState.left.isReady;

	transCnt++;
}

void CubeExplorer::OnRR(vector<string>::iterator& iter) {
	RightTight();											//右爪夹紧
	LeftLoose();											//左爪松开
	LeftReady();											//左爪转动至90/270°状态
	macVec.push_back(Operation::R);							//右爪顺时针转动90°
	handState.right.isReady = !handState.right.isReady;		//设置右爪状态参数
	for (auto it = iter + 1; it != strNorVec.end(); it++) {	//由于魔方整体转动后，面的定义发生变化，
		if (*it == "U") *it = "B";							//因此需要遍历之后的操作，全部调整为转变后的定义
		else if (*it == "B") *it = "D";						//
		else if (*it == "D") *it = "F";
		else if (*it == "F") *it = "U";
		else if (*it == "U'") *it = "B'";
		else if (*it == "B'") *it = "D'";
		else if (*it == "D'") *it = "F'";
		else if (*it == "F'") *it = "U'";
		else if (*it == "U2") *it = "B2";
		else if (*it == "B2") *it = "D2";
		else if (*it == "D2") *it = "F2";
		else if (*it == "F2") *it = "U2";
	}

	transCnt++;
}

void CubeExplorer::On_RR(vector<string>::iterator& iter) {
	RightTight();
	LeftLoose();
	LeftReady();
	macVec.push_back(Operation::_R);
	handState.right.isReady = !handState.right.isReady;
	for (auto it = iter + 1; it != strNorVec.end(); it++) {
		if (*it == "U") *it = "F";
		else if (*it == "B") *it = "U";
		else if (*it == "D") *it = "B";
		else if (*it == "F") *it = "D";
		else if (*it == "U'") *it = "F'";
		else if (*it == "B'") *it = "U'";
		else if (*it == "D'") *it = "B'";
		else if (*it == "F'") *it = "D'";
		else if (*it == "U2") *it = "F2";
		else if (*it == "B2") *it = "U2";
		else if (*it == "D2") *it = "B2";
		else if (*it == "F2") *it = "D2";
	}

	transCnt++;
}

void CubeExplorer::OnFF(vector<string>::iterator& iter) {
	LeftTight();
	RightLoose();
	RightReady();
	macVec.push_back(Operation::F);
	handState.left.isReady = !handState.left.isReady;
	for (auto it = iter + 1; it != strNorVec.end(); it++) {
		if (*it == "U") *it = "R";
		else if (*it == "R") *it = "D";
		else if (*it == "D") *it = "L";
		else if (*it == "L") *it = "U";
		else if (*it == "U'") *it = "R'";
		else if (*it == "R'") *it = "D'";
		else if (*it == "D'") *it = "L'";
		else if (*it == "L'") *it = "U'";
		else if (*it == "U2") *it = "R2";
		else if (*it == "R2") *it = "D2";
		else if (*it == "D2") *it = "L2";
		else if (*it == "L2") *it = "U2";
	}

	transCnt++;
}

void CubeExplorer::On_FF(vector<string>::iterator& iter) {
	LeftTight();
	RightLoose();
	RightReady();
	macVec.push_back(Operation::_F);
	handState.left.isReady = !handState.left.isReady;
	for (auto it = iter + 1; it != strNorVec.end(); it++) {
		if (*it == "U") *it = "L";
		else if (*it == "R") *it = "U";
		else if (*it == "D") *it = "R";
		else if (*it == "L") *it = "D";
		else if (*it == "U'") *it = "L'";
		else if (*it == "R'") *it = "U'";
		else if (*it == "D'") *it = "R'";
		else if (*it == "L'") *it = "D'";
		else if (*it == "U2") *it = "L2";
		else if (*it == "R2") *it = "U2";
		else if (*it == "D2") *it = "R2";
		else if (*it == "L2") *it = "D2";
	}

	transCnt++;
}

void CubeExplorer::OnRR2(vector<string>::iterator& iter) {
	RightTight();
	LeftLoose();
	LeftReady();
	macVec.push_back(Operation::R2);
	for (auto it = iter + 1; it != strNorVec.end(); it++) {
		if (*it == "U") *it = "D";
		else if (*it == "B") *it = "F";
		else if (*it == "D") *it = "U";
		else if (*it == "F") *it = "B";
		else if (*it == "U'") *it = "D'";
		else if (*it == "B'") *it = "F'";
		else if (*it == "D'") *it = "U'";
		else if (*it == "F'") *it = "B'";
		else if (*it == "U2") *it = "D2";
		else if (*it == "B2") *it = "F2";
		else if (*it == "D2") *it = "U2";
		else if (*it == "F2") *it = "B2";
	}

	transCnt++;
}

void CubeExplorer::OnFF2(vector<string>::iterator& iter) {
	LeftTight();
	RightLoose();
	RightReady();
	macVec.push_back(Operation::F2);
	for (auto it = iter + 1; it != strNorVec.end(); it++) {
		if (*it == "U") *it = "D";
		else if (*it == "R") *it = "L";
		else if (*it == "D") *it = "U";
		else if (*it == "L") *it = "R";
		else if (*it == "U'") *it = "D'";
		else if (*it == "R'") *it = "L'";
		else if (*it == "D'") *it = "U'";
		else if (*it == "L'") *it = "R'";
		else if (*it == "U2") *it = "D2";
		else if (*it == "R2") *it = "L2";
		else if (*it == "D2") *it = "U2";
		else if (*it == "L2") *it = "R2";
	}

	transCnt++;
}

void CubeExplorer::OnR2(vector<string>::iterator& iter) {
	RightTight();
	GetLeftReadyAndTight();
	macVec.push_back(Operation::R2);

	transCnt++;
}

void CubeExplorer::OnF2(vector<string>::iterator& iter) {
	LeftTight();
	GetRightReadyAndTight();
	macVec.push_back(Operation::F2);

	transCnt++;
}

void CubeExplorer::LeftLoose() {
	if (handState.left.isTight) {
		macVec.push_back(Operation::LeftLoose);
		handState.left.isTight = false;
	}
}

void CubeExplorer::LeftTight() {
	if (!handState.left.isTight) {
		if ((!handState.right.isReady) && (!handState.left.isReady)) {
			macVec.push_back(Operation::F);
		}
		macVec.push_back(Operation::LeftTight);
		handState.left.isTight = true;
	}
}

void CubeExplorer::LeftReady() {
	if (!handState.left.isReady) {
		macVec.push_back(Operation::F);
		handState.left.isReady = true;
	}
}

void CubeExplorer::RightLoose() {
	if (handState.right.isTight) {
		macVec.push_back(Operation::RightLoose);
		handState.right.isTight = false;
	}
}

void CubeExplorer::RightTight() {
	if (!handState.right.isTight) {
		if ((!handState.right.isReady) && (!handState.left.isReady)) {
			macVec.push_back(Operation::R);
		}
		macVec.push_back(Operation::RightTight);
		handState.right.isTight = true;
	}
}

void CubeExplorer::RightReady() {
	if (!handState.right.isReady) {
		macVec.push_back(Operation::R);
		handState.right.isReady = true;
	}
}

void CubeExplorer::GetLeftReadyAndTight() {
	if (handState.left.isReady) {
		if (!handState.left.isTight) {
			macVec.push_back(Operation::LeftTight);
			handState.left.isTight = true;
		}
	}
	else {
		if (handState.left.isTight) {
			macVec.push_back(Operation::LeftLoose);
			macVec.push_back(Operation::F);
			macVec.push_back(Operation::LeftTight);
		}
		else {
			macVec.push_back(Operation::F);
			macVec.push_back(Operation::LeftTight);
			handState.left.isTight = true;
		}
		handState.left.isReady = true;
	}
}

void CubeExplorer::GetRightReadyAndTight() {
	if (handState.right.isReady) {
		if (!handState.right.isTight) {
			macVec.push_back(Operation::RightTight);
			handState.right.isTight = true;
		}
	}
	else {
		if (handState.right.isTight) {
			macVec.push_back(Operation::RightLoose);
			macVec.push_back(Operation::R);
			macVec.push_back(Operation::RightTight);
		}
		else {
			macVec.push_back(Operation::R);
			macVec.push_back(Operation::RightTight);
			handState.right.isTight = true;
		}
		handState.right.isReady = true;
	}
}

void CubeExplorer::GetShortestWay() {
	string::size_type lastSpace = 0;	//记录上一次遇到空格的位置
	//将原始字符串分割为多个表示单个操作的字符串并存放到strNorVec容器中
	string::size_type i = 0;
	for (; i < target.length(); i++) {
		if (target[i] == ' ') {
			string strTemp;
			for (string::size_type index = lastSpace; index < i; index++) {
				strTemp.push_back(target[index]);
			}
			strNorVec.push_back(strTemp);
			lastSpace = i + 1;
		}
	}

	mystrNorVec = strNorVec;

	transCnt = 0;
	for (auto iter = strNorVec.begin(); iter != strNorVec.end(); iter++) {
		if (*iter == "R") OnR(iter);				//F和R面系列操作，无需魔方的整体转动就可以直接操作
		else if (*iter == "F") OnF(iter);			//
		else if (*iter == "R'") On_R(iter);			//
		else if (*iter == "F'") On_F(iter);			//
		else if (*iter == "R2") OnR2(iter);			//
		else if (*iter == "F2") OnF2(iter);			//
		else if (*iter == "B") {
			OnRR2(iter);							//通用公式“B“，首先要魔方整体沿R方向旋转180°
			OnF(iter);								//此时原来的B面成为F面，F面顺时针转动90°即可完成操作
		}
		else if (*iter == "L") {
			OnFF2(iter);
			OnR(iter);
		}
		else if (*iter == "U") {
			if (STRATEGY == 1) {
				OnFF(iter);
				OnR(iter);
			}
			else {
				//对于U、B两面的操作，有两个不同策略：1.魔方沿F面整体旋转后，右爪进行操作；2.魔方沿R面整体旋转后，左爪进行操作
				//我们需要根据机械爪的当前状态和当前操作其后紧跟的操作判断，哪一种策略得到的指令最少
				if (handState.left.isReady) {														//左爪处于90/270°状态
					if (handState.right.isReady &&													//右爪处于90/270°状态
						(iter + 1 != strNorVec.end()) &&											//紧跟的下一个操作是F、B面操作
						(*(iter + 1) == "F" || *(iter + 1) == "F'" || *(iter + 1) == "F2" ||		//
							*(iter + 1) == "B" || *(iter + 1) == "B'" || *(iter + 1) == "B2")) {		//
						   //若左右手都处于90/270°状态，那么无论先F后R或先R后F，当前操作需要的指令数是一样的。因此仅考虑当前操作之后的操作
						   //如果紧跟的操作是F面和B面操作，那么对于当前操作之后的那个操作，策略1比策略2少4个机械爪指令，因此采用策略1
						OnFF(iter);
						OnR(iter);
					}
					else {
						//若左手处于90/270°状态而右手处于0/180°状态，那么对于当前操作，策略2比策略1少4个机械爪指令
						//若当前操作其后紧跟的那个操作不是对F、B面的操作，那么策略2得到的机械爪指令不大于策略1
						//（当紧跟的是R、L面操作时，策略2比策略1少4个指令；当紧跟的是D面操作时，两个策略等效），因此采用策略2
						On_RR(iter);
						OnF(iter);
					}
				}
				else {
					//若左手处于0/180°状态，对于当前操作，策略1比策略2少4个指令
					//其后紧跟的指令，若为L、R面操作，则策略1比策略2少4个指令，这两个因素相互抵消；若为F、B面操作，策略1比策略2少4个指令
					//只要左手处于这个状态，就选择策略1
					OnFF(iter);
					OnR(iter);
				}
			}
			
			
		}
		else if (*iter == "D") {
			if (STRATEGY == 1) {
				OnRR(iter);
				OnF(iter);
			}
			else {
				if (handState.left.isReady) {
					if (handState.right.isReady &&
						(iter + 1 != strNorVec.end()) &&
						(*(iter + 1) == "F" || *(iter + 1) == "F'" || *(iter + 1) == "F2" || *(iter + 1) == "B" || *(iter + 1) == "B'" || *(iter + 1) == "B2")) {
						On_FF(iter);
						OnR(iter);
					}
					else {
						OnRR(iter);
						OnF(iter);
					}
				}
				else {
					On_FF(iter);
					OnR(iter);
				}
			}
		}
		else if (*iter == "B'") {
			OnRR2(iter);
			On_F(iter);
		}
		else if (*iter == "L'") {
			OnFF2(iter);
			On_R(iter);
		}
		else if (*iter == "U'") {
			if (STRATEGY == 1) {
				OnFF(iter);
				On_R(iter);
			}
			else {
				if (handState.left.isReady) {
					if (handState.right.isReady &&
						(iter + 1 != strNorVec.end()) &&
						(*(iter + 1) == "F" || *(iter + 1) == "F'" || *(iter + 1) == "F2" || *(iter + 1) == "B" || *(iter + 1) == "B'" || *(iter + 1) == "B2")) {
						OnFF(iter);
						On_R(iter);
					}
					else {
						On_RR(iter);
						On_F(iter);
					}
				}
				else {
					OnFF(iter);
					On_R(iter);
				}
			}
		}
		else if (*iter == "D'") {
			if (STRATEGY == 1) {
				OnRR(iter);
				On_F(iter);
			}
			else {
				if (handState.left.isReady) {
					if (handState.right.isReady &&
						(iter + 1 != strNorVec.end()) &&
						(*(iter + 1) == "F" || *(iter + 1) == "F'" || *(iter + 1) == "F2" || *(iter + 1) == "B" || *(iter + 1) == "B'" || *(iter + 1) == "B2")) {
						On_FF(iter);
						On_R(iter);
					}
					else {
						OnRR(iter);
						On_F(iter);
					}
				}
				else {
					On_FF(iter);
					On_R(iter);
				}
			}
		}
		else if (*iter == "B2") {
			OnRR2(iter);
			OnF2(iter);
		}
		else if (*iter == "L2") {
			OnFF2(iter);
			OnR2(iter);
		}
		else if (*iter == "U2") {
			if (STRATEGY == 1) {
				OnFF(iter);
				OnR2(iter);
			}
			else {
				if (handState.left.isReady) {
					if (handState.right.isReady &&
						(iter + 1 != strNorVec.end()) &&
						(*(iter + 1) == "F" || *(iter + 1) == "F'" || *(iter + 1) == "F2" || *(iter + 1) == "B" || *(iter + 1) == "B'" || *(iter + 1) == "B2")) {
						OnFF(iter);
						OnR2(iter);
					}
					else {
						On_RR(iter);
						OnF2(iter);
					}
				}
				else {
					OnFF(iter);
					OnR2(iter);
				}
			}
		}
		else if (*iter == "D2") {
			if (STRATEGY == 1) {
				OnRR(iter);
				OnF2(iter);
			}
			else {
				if (handState.left.isReady) {
					if (handState.right.isReady &&
						(iter + 1 != strNorVec.end()) &&
						(*(iter + 1) == "F" || *(iter + 1) == "F'" || *(iter + 1) == "F2" || *(iter + 1) == "B" || *(iter + 1) == "B'" || *(iter + 1) == "B2")) {
						On_FF(iter);
						OnR2(iter);
					}
					else {
						OnRR(iter);
						OnF2(iter);
					}
				}
				else {
					On_FF(iter);
					OnR2(iter);
				}
			}
		}
	}

	slove_count = 0;
	for (auto iter = macVec.begin(); iter != macVec.end(); iter++) {
		switch (*iter) {
		case Operation::F:
			vecStrSerial.push_back("#1P6T200\r\n");
			SloveString += "L1 ";
			break;
		case Operation::_F:
			vecStrSerial.push_back("#1P7T200\r\n");
			SloveString += "L2 ";
			break;
		case Operation::F2:
			vecStrSerial.push_back("#1P8T200\r\n");
			SloveString += "L3 ";
			break;
		case Operation::LeftLoose:
			vecStrSerial.push_back("#2P0T200\r\n");
			SloveString += "LO ";
			break;
		case Operation::LeftTight:
			vecStrSerial.push_back("#2P1T200\r\n");
			SloveString += "LC ";
			break;
		case Operation::R:
			vecStrSerial.push_back("#3P6T200\r\n");
			SloveString += "R1 ";
			break;
		case Operation::_R:
			vecStrSerial.push_back("#3P7T200\r\n");
			SloveString += "R2 ";
			break;
		case Operation::R2:
			vecStrSerial.push_back("#3P8T200\r\n");
			SloveString += "R3 ";
			break;
		case Operation::RightLoose:
			vecStrSerial.push_back("#4P0T200\r\n");
			SloveString += "RO ";
			break;
		case Operation::RightTight:
			vecStrSerial.push_back("#4P1T200\r\n");
			SloveString += "RC ";
			break;
		}
		slove_count++;
	}
}

void CubeExplorer::ShowOperations() {
	int cnt = 0;
	for (auto iter = macVec.cbegin(); iter != macVec.cend(); iter++) {
		if (*iter != Operation::LeftLoose&&*iter != Operation::LeftTight&&*iter != Operation::RightLoose&&*iter != Operation::RightTight) {
			cnt++;
		}
		cout << *iter <<"   ";
	}
	cout << endl;
	cout << "Final operations' count: " << cnt << endl;
	cout << "All operations' count: " << macVec.size() << endl;
	cout << endl;
}

void CubeExplorer::Reset() {
	handState = HandState(true, true, true, true);
	strNorVec.clear();
	macVec.clear();
	vecStrSerial.clear();
	vecStrSerial.push_back("#2P1T200\r\n");
	vecStrSerial.push_back("#4P1T200\r\n");
//	vecStrSerial.push_back("#5P0T200\r\n");	// clamp_open
}

ostream& operator<<(ostream& os, const Operation& oper) {
	switch (oper) {
	case Operation::F:
		os << "F";
		break;
	case Operation::F2:
		os << "F2";
		break;
	case Operation::LeftLoose:
		os << "LeftLoose";
		break;
	case Operation::LeftTight:
		os << "LeftTight";
		break;
	case Operation::R:
		os << "R";
		break;
	case Operation::R2:
		os << "R2";
		break;
	case Operation::RightLoose:
		os << "RightLoose";
		break;
	case Operation::RightTight:
		os << "RightTight";
		break;
	case Operation::_F:
		os << "_F";
		break;
	case Operation::_R:
		os << "_R";
		break;
	}
	return os;
}



//!!!!

void CubeExplorer::dfs()
{
	if (operation_count > target_cnt)// 目前步数超出当前最短步数
	{
		return;
	}

	if (flag == 0) ++flag;
	else ++myiter;

	if (myiter == mystrNorVec.end()) // 搜索到末尾
	{
		macVec.swap(vector<Operation>());
		macVec.assign(mymacVec.cbegin(), mymacVec.cend());

		++path_count;        // 搜索得到的路径数 +1
		length_of_path.push_back(operation_count);
		target_cnt = operation_count;  // 最短步数为当前步数

		if (myiter != mystrNorVec.begin()) --myiter;

		return;
	}



	if (handState.left.isReady == true)
	{
		if (handState.right.isReady == true)
		{
			if (*myiter == "R") { R11_1(); }
			else if (*myiter == "R'") { _R11_1(); }
			else if (*myiter == "R2") { R211_1(); }

			else if (*myiter == "F") { F11_1(); }
			else if (*myiter == "F'") { _F11_1(); }
			else if (*myiter == "F2") { F211_1(); }

			else if (*myiter == "B") { B11_1(); }
			else if (*myiter == "B'") { _B11_1(); }
			else if (*myiter == "B2") { B211_1(); }

			else if (*myiter == "L") { L11_1(); }
			else if (*myiter == "L'") { _L11_1(); }
			else if (*myiter == "L2") { L211_1(); }

			else if (*myiter == "U") { U11_1();    U11_2(); }
			else if (*myiter == "U'") { _U11_1();  _U11_2(); }
			else if (*myiter == "U2") { U211_1();  U211_2(); }

			else if (*myiter == "D") { D11_1();    D11_2(); }
			else if (*myiter == "D'") { _D11_1();  _D11_2(); }
			else if (*myiter == "D2") { D211_1();  D211_2(); }
		}
		else
		{
			if (*myiter == "R") { R10_1(); }
			else if (*myiter == "R'") { _R10_1(); }
			else if (*myiter == "R2") { R210_1(); }

			else if (*myiter == "F") { F10_1(); }
			else if (*myiter == "F'") { _F10_1(); }
			else if (*myiter == "F2") { F210_1(); }

			else if (*myiter == "B") { B10_1(); }
			else if (*myiter == "B'") { _B10_1(); }
			else if (*myiter == "B2") { B210_1(); }

			else if (*myiter == "L") { L10_1(); }
			else if (*myiter == "L'") { _L10_1(); }
			else if (*myiter == "L2") { L210_1(); }

			else if (*myiter == "U") { U10_1();    U10_2(); }
			else if (*myiter == "U'") { _U10_1();  _U10_2(); }
			else if (*myiter == "U2") { U210_1();  U210_2(); }

			else if (*myiter == "D") { D10_1();    D10_2(); }
			else if (*myiter == "D'") { _D10_1();  _D10_2(); }
			else if (*myiter == "D2") { D210_1();  D210_2(); }
		}
	}
	else
	{
		if (handState.right.isReady)
		{
			if (*myiter == "R") { R01_1(); }
			else if (*myiter == "R'") { _R01_1(); }
			else if (*myiter == "R2") { R201_1(); }

			else if (*myiter == "F") { F01_1(); }
			else if (*myiter == "F'") { _F01_1(); }
			else if (*myiter == "F2") { F201_1(); }

			else if (*myiter == "B") { B01_1(); }
			else if (*myiter == "B'") { _B01_1(); }
			else if (*myiter == "B2") { B201_1(); }

			else if (*myiter == "L") { L01_1(); }
			else if (*myiter == "L'") { _L01_1(); }
			else if (*myiter == "L2") { L201_1(); }

			else if (*myiter == "U") { U01_1();    U01_2(); }
			else if (*myiter == "U'") { _U01_1();  _U01_2(); }
			else if (*myiter == "U2") { U201_1();  U201_2(); }

			else if (*myiter == "D") { D01_1();    D01_2(); }
			else if (*myiter == "D'") { _D01_1();  _D01_2(); }
			else if (*myiter == "D2") { D201_1();  D201_2(); }
		}
		else
		{
			cout << "error: dfs 00 at the same time";
		}
	}

	if (myiter != mystrNorVec.begin()) --myiter;
}
void CubeExplorer::myshortestway()
{

	myiter = mystrNorVec.begin(); // myiter 指向序列头部
	target_cnt = macVec.size(); // 目前最短步骤
	operation_count = 0; // 当前步骤数为0
	path_count = 0;     // 搜索过的路径条数
	flag = 0;

	handState.left.isReady = true;
	handState.right.isReady = true;
	handState.left.isTight = true;
	handState.left.isTight = true;

	macVec.swap(vector<Operation>());
	macVec.assign(mymacVec.cbegin(), mymacVec.cend());

	dfs();  //得到最短序列 放在mymacVec中

	for (auto iter = macVec.begin(); iter != macVec.end(); iter++) {
		switch (*iter) {
		case Operation::F:
			vecStrSerial.push_back("#1P6T200\r\n");
			break;
		case Operation::_F:
			vecStrSerial.push_back("#1P7T200\r\n");
			break;
		case Operation::F2:
			vecStrSerial.push_back("#1P8T200\r\n");
			break;
		case Operation::LeftLoose:
			vecStrSerial.push_back("#2P0T200\r\n");
			break;
		case Operation::LeftTight:
			vecStrSerial.push_back("#2P1T200\r\n");
			break;
		case Operation::R:
			vecStrSerial.push_back("#3P6T200\r\n");
			break;
		case Operation::_R:
			vecStrSerial.push_back("#3P7T200\r\n");
			break;
		case Operation::R2:
			vecStrSerial.push_back("#3P8T200\r\n");
			break;
		case Operation::RightLoose:
			vecStrSerial.push_back("#4P0T200\r\n");
			break;
		case Operation::RightTight:
			vecStrSerial.push_back("#4P1T200\r\n");
			break;
		}
	}
}
// 旋转面改变
void CubeExplorer::spinR()
{
	for (auto it = myiter + 1; it != mystrNorVec.end(); it++) {	//由于魔方整体转动后，面的定义发生变化，
		if (*it == "U") *it = "B";							//因此需要遍历之后的操作，全部调整为转变后的定义
		else if (*it == "B") *it = "D";						//
		else if (*it == "D") *it = "F";
		else if (*it == "F") *it = "U";
		else if (*it == "U'") *it = "B'";
		else if (*it == "B'") *it = "D'";
		else if (*it == "D'") *it = "F'";
		else if (*it == "F'") *it = "U'";
		else if (*it == "U2") *it = "B2";
		else if (*it == "B2") *it = "D2";
		else if (*it == "D2") *it = "F2";
		else if (*it == "F2") *it = "U2";
	}
}
void CubeExplorer::spin_R()
{
	for (auto it = myiter + 1; it != mystrNorVec.end(); it++) {
		if (*it == "U") *it = "F";
		else if (*it == "B") *it = "U";
		else if (*it == "D") *it = "B";
		else if (*it == "F") *it = "D";
		else if (*it == "U'") *it = "F'";
		else if (*it == "B'") *it = "U'";
		else if (*it == "D'") *it = "B'";
		else if (*it == "F'") *it = "D'";
		else if (*it == "U2") *it = "F2";
		else if (*it == "B2") *it = "U2";
		else if (*it == "D2") *it = "B2";
		else if (*it == "F2") *it = "D2";
	}
}
void CubeExplorer::spinR2()
{
	for (auto it = myiter + 1; it != mystrNorVec.end(); it++) {
		if (*it == "U") *it = "D";
		else if (*it == "B")  *it = "F";
		else if (*it == "D") *it = "U";
		else if (*it == "F")  *it = "B";
		else if (*it == "U'") *it = "D'";
		else if (*it == "B'") *it = "F'";
		else if (*it == "D'") *it = "U'";
		else if (*it == "F'") *it = "B'";
		else if (*it == "U2") *it = "D2";
		else if (*it == "B2") *it = "F2";
		else if (*it == "D2") *it = "U2";
		else if (*it == "F2") *it = "B2";
	}
}
void CubeExplorer::spinF()
{
	for (auto it = myiter + 1; it != mystrNorVec.end(); it++) {
		if (*it == "U") *it = "R";
		else if (*it == "R") *it = "D";
		else if (*it == "D") *it = "L";
		else if (*it == "L") *it = "U";
		else if (*it == "U'") *it = "R'";
		else if (*it == "R'") *it = "D'";
		else if (*it == "D'") *it = "L'";
		else if (*it == "L'") *it = "U'";
		else if (*it == "U2") *it = "R2";
		else if (*it == "R2") *it = "D2";
		else if (*it == "D2") *it = "L2";
		else if (*it == "L2") *it = "U2";
	}
}
void CubeExplorer::spin_F()
{
	for (auto it = myiter + 1; it != mystrNorVec.end(); it++) {
		if (*it == "U") *it = "L";
		else if (*it == "R") *it = "U";
		else if (*it == "D") *it = "R";
		else if (*it == "L") *it = "D";
		else if (*it == "U'") *it = "L'";
		else if (*it == "R'") *it = "U'";
		else if (*it == "D'") *it = "R'";
		else if (*it == "L'") *it = "D'";
		else if (*it == "U2") *it = "L2";
		else if (*it == "R2") *it = "U2";
		else if (*it == "D2") *it = "R2";
		else if (*it == "L2") *it = "D2";
	}
}
void CubeExplorer::spinF2()
{
	for (auto it = myiter + 1; it != mystrNorVec.end(); it++) {
		if (*it == "U") *it = "D";
		else if (*it == "R")  *it = "L";
		else if (*it == "D")  *it = "U";
		else if (*it == "L")  *it = "R";
		else if (*it == "U'")  *it = "D'";
		else if (*it == "R'")  *it = "L'";
		else if (*it == "D'") *it = "U'";
		else if (*it == "L'") *it = "R'";
		else if (*it == "U2") *it = "D2";
		else if (*it == "R2") *it = "L2";
		else if (*it == "D2") *it = "U2";
		else if (*it == "L2") *it = "R2";
	}
}
// R 
void CubeExplorer::R11_1()
{
	operation_count += 1; // R11_1 需要一个动作
	mymacVec.push_back(Operation::R); // 填入动作命令
	handState.right.isReady = false; // 修改状态
	dfs();                         //递归
	operation_count -= 1;   // 动作数减1
	mymacVec.pop_back();    //弹出动作命令
	handState.right.isReady = true;
}
void CubeExplorer::R01_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::R);
	handState.left.isReady = true;
	handState.right.isReady = false;
	dfs();
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	operation_count -= 4;
	handState.left.isReady = false;
	handState.right.isReady = true;

}
void CubeExplorer::R10_1()
{
	operation_count += 1;
	mymacVec.push_back(Operation::R);
	handState.right.isReady = true;
	dfs();
	mymacVec.pop_back();
	operation_count -= 1;
	handState.right.isReady = false;
}

// _R
void CubeExplorer::_R11_1()
{
	operation_count += 1; // R11_1 需要一个动作
	mymacVec.push_back(Operation::_R); // 填入动作命令
	handState.right.isReady = false; // 修改状态
	dfs();                         //递归
	operation_count -= 1;   // 动作数减1
	mymacVec.pop_back();    //弹出动作命令
	handState.right.isReady = true;
}
void CubeExplorer::_R01_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::_R);
	handState.left.isReady = true;
	handState.right.isReady = false;
	dfs();
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	operation_count -= 4;
	handState.left.isReady = false;
	handState.right.isReady = true;

}
void CubeExplorer::_R10_1()
{
	operation_count += 1;
	mymacVec.push_back(Operation::_R);
	handState.right.isReady = true;
	dfs();
	mymacVec.pop_back();
	operation_count -= 1;
	handState.right.isReady = false;
}
//R2
void CubeExplorer::R211_1()
{
	operation_count += 1;
	mymacVec.push_back(Operation::R2);
	dfs();
	operation_count -= 1;
	mymacVec.pop_back();
}
void CubeExplorer::R201_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::R2);
	handState.left.isReady = true;
	dfs();
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	operation_count -= 4;
	handState.left.isReady = false;
}
void CubeExplorer::R210_1()
{
	operation_count += 1;
	mymacVec.push_back(Operation::R2);
	dfs();
	mymacVec.pop_back();
	operation_count -= 1;
}

// F
void CubeExplorer::F11_1()
{
	operation_count += 1;
	mymacVec.push_back(Operation::F);
	handState.left.isReady = false;
	dfs();
	operation_count -= 1;
	mymacVec.pop_back();
	handState.left.isReady = true;
}
void CubeExplorer::F01_1()
{
	operation_count += 1;
	mymacVec.push_back(Operation::F);
	handState.left.isReady = true;
	dfs();
	mymacVec.pop_back();
	operation_count -= 1;
	handState.left.isReady = false;
}
void CubeExplorer::F10_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::F);
	handState.right.isReady = true;
	handState.left.isReady = false;
	dfs();
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	operation_count -= 4;
	handState.right.isReady = false;
	handState.left.isReady = true;

}
//_F
void CubeExplorer::_F11_1()
{
	operation_count += 1;
	mymacVec.push_back(Operation::_F);
	handState.left.isReady = false;
	dfs();
	operation_count -= 1;
	mymacVec.pop_back();
	handState.left.isReady = true;
}
void CubeExplorer::_F01_1()
{
	operation_count += 1;
	mymacVec.push_back(Operation::_F);
	handState.left.isReady = true;
	dfs();
	mymacVec.pop_back();
	operation_count -= 1;
	handState.left.isReady = false;
}
void CubeExplorer::_F10_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::_F);
	handState.right.isReady = true;
	handState.left.isReady = false;
	dfs();
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	operation_count -= 4;
	handState.right.isReady = false;
	handState.left.isReady = true;
}
//F2
void CubeExplorer::F211_1()
{
	operation_count += 1;
	mymacVec.push_back(Operation::F2);
	dfs();
	operation_count -= 1;
	mymacVec.pop_back();
}
void CubeExplorer::F201_1()
{
	operation_count += 1;
	mymacVec.push_back(Operation::F2);
	dfs();
	mymacVec.pop_back();
	operation_count -= 1;
}
void CubeExplorer::F210_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::F2);
	handState.right.isReady = true;
	dfs();
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	operation_count -= 4;
	handState.right.isReady = false;
}
//B
void CubeExplorer::B11_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::R2);//****
	spinR2();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::F);
	handState.left.isReady = false;

	dfs();

	spinR2();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.left.isReady = true;
}
void CubeExplorer::B01_1()
{
	operation_count += 5;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::R2);// ****
	spinR2();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::F);

	dfs();

	spinR2();
	operation_count -= 5;
	for (int i = 1; i <= 5; ++i) mymacVec.pop_back();

}
void CubeExplorer::B10_1()
{
	operation_count += 7;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::R2); //****
	spinR2();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::F);
	handState.left.isReady = false;
	handState.right.isReady = true;

	dfs();

	spinR2();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.left.isReady = true;
	handState.right.isReady = false;
}
//_B
void CubeExplorer::_B11_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::R2);//****
	spinR2();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::_F);
	handState.left.isReady = false;

	dfs();

	spinR2();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.left.isReady = true;
}
void CubeExplorer::_B01_1()
{
	operation_count += 5;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::R2);// ****
	spinR2();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::_F);

	dfs();

	spinR2();
	operation_count -= 5;
	for (int i = 1; i <= 5; ++i) mymacVec.pop_back();
}
void CubeExplorer::_B10_1()
{
	operation_count += 7;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::R2); //****
	spinR2();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::_F);
	handState.left.isReady = false;
	handState.right.isReady = true;

	dfs();

	spinR2();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.left.isReady = true;
	handState.right.isReady = false;
}//
 //B2
void CubeExplorer::B211_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::R2); //****
	spinR2();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::F2);

	dfs();

	spinR2();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
}
void CubeExplorer::B201_1()
{
	operation_count += 5;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::R2);// ****
	spinR2();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::F2);
	handState.left.isReady = true;

	dfs();

	spinR2();
	operation_count -= 5;
	for (int i = 1; i <= 5; ++i) mymacVec.pop_back();
	handState.left.isReady = false;
}
void CubeExplorer::B210_1()
{
	operation_count += 7;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::R2); //****
	spinR2();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::F2);
	handState.right.isReady = true;

	dfs();

	spinR2();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.right.isReady = false;
}
//L
void CubeExplorer::L11_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::F2);//****
	spinF2();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::R);
	handState.right.isReady = false;

	dfs();

	spinF2();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.right.isReady = true;
}
void CubeExplorer::L01_1()
{
	operation_count += 7;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::F2); //****
	spinF2();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::R);
	handState.left.isReady = true;
	handState.right.isReady = false;

	dfs();

	spinF2();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.left.isReady = false;
	handState.right.isReady = true;
}
void CubeExplorer::L10_1()
{
	operation_count += 5;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::F2);// ****
	spinF2();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::R);

	dfs();

	spinF2();
	operation_count -= 5;
	for (int i = 1; i <= 5; ++i) mymacVec.pop_back();

}
//_L
void CubeExplorer::_L11_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::F2);//****
	spinF2();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::_R);
	handState.right.isReady = false;

	dfs();

	spinF2();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.right.isReady = true;
}
void CubeExplorer::_L01_1()
{
	operation_count += 7;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::F2); //****
	spinF2();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::_R);
	handState.left.isReady = true;
	handState.right.isReady = false;

	dfs();

	spinF2();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.left.isReady = false;
	handState.right.isReady = true;
}
void CubeExplorer::_L10_1()
{
	operation_count += 5;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::F2);// ****
	spinF2();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::_R);

	dfs();

	spinF2();
	operation_count -= 5;
	for (int i = 1; i <= 5; ++i) mymacVec.pop_back();
}
//L2
void CubeExplorer::L211_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::F2);//****
	spinF2();

	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::R2);

	dfs();

	spinF2();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
}
void CubeExplorer::L201_1()
{
	operation_count += 7;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::F2); //****
	spinF2();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::R2);
	handState.left.isReady = true;

	dfs();

	spinF2();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.left.isReady = false;
}
void CubeExplorer::L210_1()
{
	operation_count += 5;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::F2);// ****
	spinF2();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::R2);
	handState.right.isReady = true;

	dfs();

	spinF2();
	operation_count -= 5;
	for (int i = 1; i <= 5; ++i) mymacVec.pop_back();
	handState.right.isReady = false;
}
//U 左手执行
void CubeExplorer::U11_1()
{
	operation_count += 7;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::_R);//****
	spin_R();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::F);
	handState.left.isReady = false;

	dfs();

	spinR();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.left.isReady = true;
}
void CubeExplorer::U01_1()
{
	operation_count += 8;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::_R);//****
	spin_R();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::F);

	dfs();

	spinR();
	operation_count -= 8;
	for (int i = 1; i <= 8; ++i) mymacVec.pop_back();
}
void CubeExplorer::U10_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::_R);//****
	spin_R();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::F);
	handState.left.isReady = false;
	handState.right.isReady = true;

	dfs();

	spinR();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.left.isReady = true;
	handState.right.isReady = false;
}
//_U 左
void CubeExplorer::_U11_1()
{
	operation_count += 7;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::_R);//****
	spin_R();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::_F);
	handState.left.isReady = false;

	dfs();

	spinR();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.left.isReady = true;
}
void CubeExplorer::_U01_1()
{
	operation_count += 8;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::_R);//****
	spin_R();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::_F);

	dfs();

	spinR();
	operation_count -= 8;
	for (int i = 1; i <= 8; ++i) mymacVec.pop_back();
}
void CubeExplorer::_U10_1()
{

	operation_count += 4;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::_R);//****
	spin_R();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::_F);
	handState.left.isReady = false;
	handState.right.isReady = true;

	dfs();

	spinR();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.left.isReady = true;
	handState.right.isReady = false;
}
//U2 左
void CubeExplorer::U211_1()
{
	operation_count += 7;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::_R);//****
	spin_R();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::F2);

	dfs();

	spinR();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
}
void CubeExplorer::U201_1()
{
	operation_count += 8;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::_R);//****
	spin_R();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::F2);

	dfs();

	spinR();
	operation_count -= 8;
	for (int i = 1; i <= 8; ++i) mymacVec.pop_back();
}
void CubeExplorer::U210_1()
{

	operation_count += 4;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::_R);//****
	spin_R();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::_F);
	handState.right.isReady = true;

	dfs();

	spinR();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.right.isReady = false;
}
//U 右
void CubeExplorer::U11_2()
{
	operation_count += 7;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::F);//****
	spinF();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::R);
	handState.right.isReady = false;

	dfs();

	spin_F();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.right.isReady = true;
}
void CubeExplorer::U01_2()
{
	operation_count += 4;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::F);//****
	spinF();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::R);
	handState.left.isReady = true;
	handState.right.isReady = false;

	dfs();

	spin_F();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.left.isReady = false;
	handState.right.isReady = true;
}
void CubeExplorer::U10_2()
{
	operation_count += 8;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::F);//****
	spinF();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::R);

	dfs();

	spin_F();
	operation_count -= 8;
	for (int i = 1; i <= 8; ++i) mymacVec.pop_back();
}
//_U 右
void CubeExplorer::_U11_2()
{
	operation_count += 7;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::F);//****
	spinF();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::_R);
	handState.right.isReady = false;

	dfs();

	spin_F();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.right.isReady = true;
}
void CubeExplorer::_U01_2()
{
	operation_count += 4;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::F);//****
	spinF();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::_R);
	handState.left.isReady = true;
	handState.right.isReady = false;

	dfs();

	spin_F();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.left.isReady = false;
	handState.right.isReady = true;
}
void CubeExplorer::_U10_2()
{
	operation_count += 8;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::F);//****
	spinF();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::_R);

	dfs();

	spin_F();
	operation_count -= 8;
	for (int i = 1; i <= 8; ++i) mymacVec.pop_back();
}
//U2 右
void CubeExplorer::U211_2()
{
	operation_count += 7;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::F);//****
	spinF();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::R2);

	dfs();

	spin_F();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
}
void CubeExplorer::U201_2()
{
	operation_count += 4;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::F);//****
	spinF();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::R2);
	handState.left.isReady = true;

	dfs();

	spin_F();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.left.isReady = false;
}
void CubeExplorer::U210_2()
{
	operation_count += 8;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::F);//****
	spinF();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::R2);
	handState.right.isReady = true;

	dfs();

	spin_F();
	operation_count -= 8;
	for (int i = 1; i <= 8; ++i) mymacVec.pop_back();
	handState.right.isReady = false;
}

//D 左
void CubeExplorer::D11_1()
{
	operation_count += 7;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::R);//****
	spinR();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::F);
	handState.left.isReady = false;

	dfs();

	spin_R();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.left.isReady = true;
}
void CubeExplorer::D01_1()
{
	operation_count += 8;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::R);//****
	spinR();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::F);

	dfs();

	spin_R();
	operation_count -= 8;
	for (int i = 1; i <= 8; ++i) mymacVec.pop_back();
}
void CubeExplorer::D10_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::R);//****
	spinR();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::F);
	handState.left.isReady = false;
	handState.right.isReady = true;

	dfs();

	spin_R();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.left.isReady = true;
	handState.right.isReady = false;
}
//_D 左
void CubeExplorer::_D11_1()
{
	operation_count += 7;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::R);//****
	spinR();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::_F);
	handState.left.isReady = false;

	dfs();

	spin_R();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.left.isReady = true;
}
void CubeExplorer::_D01_1()
{
	operation_count += 8;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::R);//****
	spinR();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::_F);

	dfs();

	spin_R();
	operation_count -= 8;
	for (int i = 1; i <= 8; ++i) mymacVec.pop_back();
}
void CubeExplorer::_D10_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::R);//****
	spinR();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::_F);
	handState.left.isReady = false;
	handState.right.isReady = true;

	dfs();

	spin_R();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.left.isReady = true;
	handState.right.isReady = false;
}
//D2 左
void CubeExplorer::D211_1()
{
	operation_count += 7;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::R);//****
	spinR();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::F2);

	dfs();

	spin_R();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
}
void CubeExplorer::D201_1()
{
	operation_count += 8;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::R);//****
	spinR();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::F2);
	handState.left.isReady = true;

	dfs();

	spin_R();
	operation_count -= 8;
	for (int i = 1; i <= 8; ++i) mymacVec.pop_back();
	handState.left.isReady = false;
}
void CubeExplorer::D210_1()
{
	operation_count += 4;
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::R);//****
	spinR();
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::F2);
	handState.right.isReady = true;

	dfs();

	spin_R();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.right.isReady = false;
}
//D 右
void CubeExplorer::D11_2()
{
	operation_count += 7;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::_F);//****
	spin_F();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::R);
	handState.right.isReady = false;

	dfs();

	spinF();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.right.isReady = true;
}
void CubeExplorer::D01_2()
{
	operation_count += 4;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::_F);//****
	spin_F();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::R);
	handState.left.isReady = true;
	handState.right.isReady = false;

	dfs();

	spinF();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.left.isReady = false;
	handState.right.isReady = true;
}
void CubeExplorer::D10_2()
{
	operation_count += 8;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::_F);//****
	spin_F();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::R);

	dfs();

	spinF();
	operation_count -= 8;
	for (int i = 1; i <= 8; ++i) mymacVec.pop_back();
}
//_D 右
void CubeExplorer::_D11_2()
{
	operation_count += 7;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::_F);//****
	spin_F();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::_R);
	handState.right.isReady = false;

	dfs();

	spinF();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
	handState.right.isReady = true;
}
void CubeExplorer::_D01_2()
{
	operation_count += 4;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::_F);//****
	spin_F();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::_R);
	handState.left.isReady = true;
	handState.right.isReady = false;

	dfs();

	spinF();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.left.isReady = false;
	handState.right.isReady = true;
}
void CubeExplorer::_D10_2()
{
	operation_count += 8;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::_F);//****
	spin_F();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::_R);

	dfs();

	spinF();
	operation_count -= 8;
	for (int i = 1; i <= 8; ++i) mymacVec.pop_back();
}
//D2 右
void CubeExplorer::D211_2()
{
	operation_count += 7;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::_F);//****
	spin_F();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::R2);

	dfs();

	spinF();
	operation_count -= 7;
	for (int i = 1; i <= 7; ++i) mymacVec.pop_back();
}
void CubeExplorer::D201_2()
{
	operation_count += 4;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::_F);//****
	spin_F();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::R2);
	handState.left.isReady = true;

	dfs();

	spinF();
	operation_count -= 4;
	for (int i = 1; i <= 4; ++i) mymacVec.pop_back();
	handState.left.isReady = false;
}
void CubeExplorer::D210_2()
{
	operation_count += 8;
	mymacVec.push_back(Operation::RightLoose);
	mymacVec.push_back(Operation::R);
	mymacVec.push_back(Operation::_F);//****
	spin_F();
	mymacVec.push_back(Operation::RightTight);
	mymacVec.push_back(Operation::LeftLoose);
	mymacVec.push_back(Operation::F);
	mymacVec.push_back(Operation::LeftTight);
	mymacVec.push_back(Operation::R2);
	handState.right.isReady = true;

	dfs();

	spinF();
	operation_count -= 8;
	for (int i = 1; i <= 8; ++i) mymacVec.pop_back();
	handState.right.isReady = false;
}