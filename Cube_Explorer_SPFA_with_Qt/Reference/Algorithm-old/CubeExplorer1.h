#ifndef CUBE_EXPLORER1_H
#define CUBE_EXPLORER1_H

#include<vector>
#include<queue>
#include<string>
#include<fstream>
#include<iostream>
#include"structs.h"


using namespace std;

struct TheoryStep {
	Point_3 face;
	int distance;
};

//表示魔方解决器的一个类
class CubeExplorer1 {
private:
	vector<string> vecStrSerial;		//保存转换后的串口序列字符串

	string target = "";						//字符串，存储将要处理的普通公式序列

	int a_TheoryStrStep[2];
	TheoryStep a_TheoryStep[25];
	TheoryStep a_TheoryStep2[2][25];
	Rot a_CubeRot;
	Rot b_Rot[2];
	HandState1 a_HandState;
	HandState1 b_HandState[2];
	int a_Move[2][120];
	int b_Move[2][120];
	int output[120];
	int book[25][2][3][3][2][3][2][3][2];

	void DFS(int step, int state);
	int Charint(char inChar);
	void Bookint(void);
	void SaveMechanicalStep(void);

public:
	int a_time[2];
	int b_time[2];
	int a_StepNum[2];
	int b_StepNum[2];
	string SloveString = "";
	const string MechanicalMoveStr[10] = { "M_L1", "M_L2", "M_L3", "M_LC", "M_LO", "M_R1", "M_R2", "M_R3", "M_RC", "M_RO" };
	const string Output[21] = { "xy","#2P0T75\r\n", "#2P1T75\r\n", "#4P0T75\r\n", "#4P1T75\r\n", "#1P8T50\r\n", "#3P8T50\r\n", "#1P7T75\r\n", "#1P6T75\r\n", "#3P7T75\r\n", "#3P6T75\r\n", "#1P7T50\r\n", "#1P8T25\r\n", "#1P6T50\r\n", "#3P7T100\r\n", "#3P6T100\r\n", "#1P7T100\r\n", "#1P6T100\r\n", "#3P7T50\r\n", "#3P8T25\r\n", "#3P6T50\r\n" };
//	const string Output[21] = { "xy","#2P0T75\r\n", "#2P1T75\r\n", "#4P0T75\r\n", "#4P1T75\r\n", "#1P8T50\r\n", "#3P8T50\r\n", "#1P6T75\r\n", "#1P7T75\r\n", "#3P6T75\r\n", "#3P7T75\r\n", "#1P6T50\r\n", "#1P8T25\r\n", "#1P7T50\r\n", "#3P6T100\r\n", "#3P7T100\r\n", "#1P6T100\r\n", "#1P7T100\r\n", "#3P6T50\r\n", "#3P8T25\r\n", "#3P7T50\r\n" };
	int slove_count;           //动作数

	vector<string>& GetVecStrSerial();
	void init();
	void Reset();
	void SetTarget(string);
	void GetShortestWay();			//
};

#define MAX_CUBE_STEP 31 // 解法的每一步骤
#define MAX_CUBE_STATE 41 // 实际上24, 存储上使用6*6方便存储
#define MAX_HAND_STATE 17 // 开合/垂直水平，共16种
//#define MAX_ORIENTATION 6 // 魔方朝向数量
#define MAX_HAND 3 // 涉及手状态数组
#define MAX_ROTATION 2 // 涉及旋转方向数组
#define INF 0x7f7f7f7f


// 表示各操作权重
#define HAND_OPEN 80
#define HAND_CLOSE 50
#define HAND_TURN_CUBE_90 60//魔方整体旋转
#define HAND_TURN_CUBE_180 110
#define HAND_TWIST_CUBE_90 50 //拧魔方单面
#define HAND_TWIST_CUBE_180 90
#define HAND_TURN_ONLY_90 40
#define HAND_TURN_ONLY_180 70

#define FACE_U_ID 6
#define FACE_R_ID 1
#define FACE_L_ID 2
#define FACE_F_ID 3
#define FACE_B_ID 4
#define FACE_D_ID 5

class CubeExplorerSPFA {
public:
	void GetShortestPath();
	void SetTarget(string);
	void InitOrientation();
	string GetAnsOpSequence();
	/*CubeExplorerSPFA();
	~CubeExplorerSPFA();*/

private:
	struct Cube_Operation {
		int face;
		char dire;
		Cube_Operation(int f, char d):face(f), dire(d) {}
		Cube_Operation():face(0), dire('\0') {}
	}target[MAX_CUBE_STEP];

	struct Ori_Vector {
		int ax, ay, az;
		Ori_Vector(int x, int y, int z) : ax(x), ay(y), az(z) {}
		Ori_Vector():ax(0), ay(0), az(0) {}
		inline int GetId() {
			return (abs(ax) + ax + 5 * abs(ay) + ay + 9 * abs(az) + az) >> 1;
		}
		Ori_Vector operator* (const Ori_Vector other) const{
			Ori_Vector o = Ori_Vector(ay * other.az - az * other.ay, -ax * other.az + az * other.ax, ax * other.ay - ay * other.ax);
			if (!o.GetId()) o = Ori_Vector(ax, ay, az);
			return o;
		}
		
	}opOrien[MAX_HAND][MAX_ROTATION];

	struct Cube_State {
		Ori_Vector orien, angle;
		int id;
		void GetId() {
			id = (orien.GetId() * 6) + angle.GetId();
		}
		Cube_State(Ori_Vector o, Ori_Vector a, int i):orien(o), angle(a), id(i) {}
		Cube_State():orien(), angle(), id(0) {}
		Cube_State operator* (const Ori_Vector other)  const{
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
		Step_State(int s, Cube_State c, int h):step(s), cubeState(c), handState(h) {}
		Step_State():cubeState(), handState(0), step(0) {}
		void Out() {
			printf("step = %d ", step);
			printf("cubeState.id = %d / %d ", cubeState.id / 6, cubeState.id % 6);
			printf("handState = OC:%d / VH:%d\n", handState / 4, handState % 4);
		}
	}eOp;

	int ansTime, targetStep;
	string ansOpSequence;

	int opCheckHand[MAX_CUBE_STATE][MAX_HAND];

	int stepTime[MAX_CUBE_STEP][MAX_CUBE_STATE][MAX_HAND_STATE];
	bool vis[MAX_CUBE_STEP][MAX_CUBE_STATE][MAX_HAND_STATE];

	string opSequence[MAX_CUBE_STEP][MAX_CUBE_STATE][MAX_HAND_STATE];

	const char opHandId[MAX_HAND] = {' ', 'R', 'L'};
	const char opOrienId[MAX_ROTATION] = { '3', '1' };

	vector<string> vecStrSerial;
	const string commandOp[21] = { 
		"xy","#2P0T75\r\n", "#2P1T75\r\n", 
		"#4P0T75\r\n", "#4P1T75\r\n", "#1P8T50\r\n", 
		"#3P8T50\r\n", "#1P7T75\r\n", "#1P6T75\r\n", 
		"#3P7T75\r\n", "#3P6T75\r\n", "#1P7T50\r\n", 
		"#1P8T25\r\n", "#1P6T50\r\n", "#3P7T100\r\n", 
		"#3P6T100\r\n", "#1P7T100\r\n", "#1P6T100\r\n", 
		"#3P7T50\r\n", "#3P8T25\r\n", "#3P6T50\r\n"
	};

	int GetFaceId(char c);
	bool StateUpdate(int costTime, int step, int cubeStateId, int handState, string curOpSequence);
	int CheckFace(int cubeStateId, int face);
	void SPFA();
	void SaveMechanicalStep();
};

#endif