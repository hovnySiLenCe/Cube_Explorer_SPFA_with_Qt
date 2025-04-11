#ifndef STRUCTS_H
#define STRUCTS_H
#include<vector>
#include<string>
#include<fstream>
#include<iostream>

using namespace std;

//===========================================================定义宏

//===============两个机械爪都归位
#define L_0_R_0 0
//===============左爪归位，右爪不归位
#define L_0_R_1 1
//===============左爪不归位，右爪归位
#define L_1_R_0 2

#define F 0
#define R 1
#define U 2
#define B 3
#define L 4
#define D 5

#define _1 0
#define _2 1
#define _3 2

#define L1 0
#define L2 1
#define L3 2
#define LC 3
#define LO 4
#define R1 5
#define R2 6
#define R3 7
#define RC 8
#define RO 9


//================================================================旋转体结构定义
struct Rot
{
	int a[3][3];
	void Set(int row0, int num0, int row1, int num1, int row2, int num2)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				a[i][j] = 0;
			}
		}
		
		a[row0][0] = num0;
		a[row1][1] = num1;
		a[row2][2] = num2;

	}
};

extern Rot R_x1, R_x2, R_x3, R_y1, R_y2, R_y3, R_z1, R_z2, R_z3;


//==========================================================六面空间点定义
struct Point_3
{
	
	int a[3][1];
	string name;

};

extern Point_3 P_F, P_R, P_U, P_B, P_L, P_D;

extern Point_3 P_FRUBLD[6];


//===========================================================机械爪步骤结构体
struct MechanicalStep
{

	string name;
	int time;
	int num;

};

//======================步骤初始化
extern struct MechanicalStep M_L1, M_L2, M_L3, M_LC, M_LO;
extern struct MechanicalStep M_R1, M_R2, M_R3, M_RC, M_RO;
extern struct MechanicalStep M_END;


//=================================================================机械爪状态结构体
//========================定义初始机械爪状态
struct HandState1
{
	//0为机械爪闭合
	int IsLeftOpen = 0;
	int IsRightOpen = 0;

	//0为nice
	int NoLeftNice = 0;
	int NoRightNice = 0;

	void Set(int _IsLeftOpen, int _IsRightOpen, int _NoLeftNice, int _NoRightNice)
	{
		IsLeftOpen = _IsLeftOpen;
		IsRightOpen = _IsRightOpen;
		NoLeftNice = _NoLeftNice;
		NoRightNice = _NoRightNice;
	}

};


//=======================机械爪步骤组合结构体
struct MechanicalHandGroup
{
	int time = 0;
	int StepNum = 0;
	struct Rot rot;
	struct HandState1 FinalHandState;
	struct MechanicalStep steps[20];

	void Set(int _StepNum, struct MechanicalStep *_Steps, struct Rot _rot, struct HandState1  _state)
	{
		StepNum = _StepNum;
		rot = _rot;
		FinalHandState = _state;
		int i;
		for (i = 0; _Steps[i].name != "M_END"; i++)
		{
			if (i >= 20) 
			{ 
				cout << "SetError!" << endl; 
				break; 
			}

			steps[i] = _Steps[i];
		}
		steps[i] = M_END;
	}

};


extern MechanicalHandGroup MechanicalHandGroupLibrary[6][3][3][16];
void MechanicalStepint(void);
void Rotint(void);
Rot Rotmt(Rot m, Rot n);
Point_3 Rotmtp3(Rot m, Point_3 n);
void Pointint(void);
void Libraryint(void);
void Librarytimeint(void);

#endif