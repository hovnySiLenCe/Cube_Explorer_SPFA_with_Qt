#pragma once

#include<vector>
#include<queue>
#include<string>
#include<fstream>
#include<iostream>
#include<QDebug>

using namespace std;

#define MAX_CUBE_STEP 51 // 解法的每一步骤
#define MAX_CUBE_STATE 41 // 实际上24, 存储上使用6*6方便存储
#define MAX_HAND_STATE 17 // 开合/垂直水平，共16种
//#define MAX_ORIENTATION 6 // 魔方朝向数量
#define MAX_HAND 3 // 涉及手状态数组
#define MAX_ROTATION 2 // 涉及旋转方向数组
#define INF 0x7f7f7f7f

// 空 1: 62 / 2 = 31
// 带 1: 192 / 2 = 96
// ning 1: 82 / 2 = 42
// dai 2 136
// kong 2 59
// ning 2 L: 68
// ning 2 R: 79
// 10c = 27
// 10o = 38
// cc+o=80
// o+c = 53
// oo+c = 91
// ooo+c = 129

// 表示各操作权重
#define HAND_OPEN 38
#define HAND_CLOSE 27

//#define HAND_TURN_CUBE_90 96 //魔方整体旋转
//#define HAND_TURN_CUBE_180 136
#define HAND_TURN_CUBE_90 56 //魔方整体旋转
#define HAND_TURN_CUBE_180 88
//#define HAND_TWIST_CUBE_90 42 //拧魔方单面
//#define HAND_TWIST_CUBE_180 68
//#define HAND_TURN_ONLY_90 31
//#define HAND_TURN_ONLY_180 59
#define HAND_TWIST_CUBE_90 31 //拧魔方单面
#define HAND_TWIST_CUBE_180 59
#define HAND_TURN_ONLY_90 31
#define HAND_TURN_ONLY_180 59

#define FACE_U_ID 6
#define FACE_R_ID 1
#define FACE_L_ID 2
#define FACE_F_ID 3
#define FACE_B_ID 4
#define FACE_D_ID 5

struct Hand_State {
	bool left_is_tight;
	bool right_is_tight;
	//构造方法，满足CubicExplore类的构造函数中默认参数的需求
	Hand_State(bool lit = true, bool rit = true) {
		left_is_tight = lit, right_is_tight = rit;
	}
};

struct Node_List {
	int opId;
	Node_List* preNode;
	Node_List(int id = 0, Node_List* p = nullptr): opId(id), preNode(p) { }
};

class CubeExplorerSPFA {
public:
	//CubeExplorerSPFA() {
	//	InitOrientation();
	//}
	void GetShortestPath(string str);
	void SetTarget(string);
	void InitOrientation();
	void SaveMechanicalStep();

	void SetAnsOpSequence(string str);
	string GetAnsOpSequence();
	string GetAnsOpSequenceFormat();
	int GetAnsOpStepNumber();
	int GetTargetStepNumber();
	int GetAnsCostTime();
	vector<string>& GetVecStrSerial();

	int ansTime;
	string ansOpSequence;
	bool reuseFlag = false;

	Hand_State hand_state;
	//~CubeExplorerSPFA();

private:
	struct Cube_Operation {
		int face;
		char dire;
		Cube_Operation(int f, char d) :face(f), dire(d) {}
		Cube_Operation() :face(0), dire('\0') {}
	}target[MAX_CUBE_STEP];

	struct Ori_Vector {
		int ax, ay, az;
		Ori_Vector(int x, int y, int z) : ax(x), ay(y), az(z) {}
		Ori_Vector() :ax(0), ay(0), az(0) {}
		inline int GetId() {
			return (abs(ax) + ax + 5 * abs(ay) + ay + 9 * abs(az) + az) >> 1;
		}
		Ori_Vector operator* (const Ori_Vector other) const {
			Ori_Vector o = Ori_Vector(ay * other.az - az * other.ay, -ax * other.az + az * other.ax, ax * other.ay - ay * other.ax);
			if (!(o.ax+o.ay+o.az)) o = Ori_Vector(ax, ay, az);
			return o;
		}

	}opOrien[MAX_HAND][MAX_ROTATION];

	struct Cube_State {
		Ori_Vector orien, angle;
		int id;
		void GetId() {
			id = (orien.GetId() * 6) + angle.GetId();
		}
		Cube_State(Ori_Vector o, Ori_Vector a, int i) :orien(o), angle(a), id(i) {}
		Cube_State() :orien(), angle(), id(0) {}
		Cube_State operator* (const Ori_Vector other)  const {
			Ori_Vector o = orien * other, a = angle * other;
			return Cube_State(o, a, (o.GetId() * 6 + a.GetId()));
		}
	};

	struct Step_State {
		Cube_State cubeState;
		int handState;	// 0:left&right in horizontal state; 
						// 1: left in vertical state;
						// 2: right in vertical state;
		int step;
		Step_State(int s, Cube_State c, int h) :step(s), cubeState(c), handState(h) {}
		Step_State() :cubeState(), handState(0), step(0) {}
		void Out() {
			printf("step = %d ", step);
			printf("cubeState.id = %d / %d ", cubeState.id / 6, cubeState.id % 6);
			printf("handState = OC:%d / VH:%d\n", handState / 4, handState % 4);
		}
	}eOp;

	int targetStep;
	//string ansOpSequence;
	bool exeFlag;

	int opCheckHand[MAX_CUBE_STATE][MAX_HAND];
	int opHandState[MAX_HAND];

	int stepTime[MAX_CUBE_STEP][MAX_CUBE_STATE][MAX_HAND_STATE];
	bool vis[MAX_CUBE_STEP][MAX_CUBE_STATE][MAX_HAND_STATE];

	Node_List* ansPathEnd;
	Node_List pathList[MAX_CUBE_STEP][MAX_CUBE_STATE][MAX_HAND_STATE];

	string opString[29] = {
		"L1 ", "L2 ", "L3 ", "LO ", "LC ", "L9 ",
		"R1 ", "R2 ", "R3 ", "RO ", "RC ", "R9 "
	};
	/*0815
	string opSequence[MAX_CUBE_STEP][MAX_CUBE_STATE][MAX_HAND_STATE];
	*/
	const char opHandId[MAX_HAND] = { ' ', 'R', 'L' };
	const char opOrienId[MAX_ROTATION] = { '3', '1' };

	vector<string> vecStrSerial;
	// 从上到下依次为 左爪，逆时针、180和顺时针, 开/合
	const string commandOp[29] = {
		"#1P6T200\r\n",	"#1P8T200\r\n", "#1P7T200\r\n",
		"#2P0T200\r\n", "#2P1T200\r\n", "#1P9T200\r\n",

		"#3P6T200\r\n",	"#3P8T200\r\n",	"#3P7T200\r\n",
		"#4P0T200\r\n", "#4P1T200\r\n", "#3P9T200\r\n"
	};

	inline int GetFaceId(char c);
	bool StateUpdate(int costTime, int step, int cubeStateId, int handState, Node_List* preNode, int opId);
	int CheckFace(int cubeStateId, int face);
	void SPFA();
};

#define MAX_OPS_LEN 6
#define OP_1_ID 0
#define OP_2_ID 1
#define OP_3_ID 2
#define OP_O_ID 3
#define OP_C_ID 4
#define OP_TO90_ID 5