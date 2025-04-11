#include"structs.h"


//========================================================机械爪全局变量
struct MechanicalStep M_L1, M_L2, M_L3, M_LC, M_LO;
struct MechanicalStep M_R1, M_R2, M_R3, M_RC, M_RO;
struct MechanicalStep M_END;
#define CLOSE 0
#define OPEN 1


//========================================================运动时间常量
const int HandCloseTime = 500;
const int HandOpenTime = 500;
const int HandMove90 = 300;
const int HandMove180 = 300;
const int DelayBetweenSteps = 200;
int timeair = 120;
int timezb90 = 120;
int timezb180 = 180;
int timezk90 = 60;
int timefz90 = 180;
int timefz180 = 240;


//========================================================旋转矩阵
Rot R_x1, R_x2, R_x3, R_y1, R_y2, R_y3, R_z1, R_z2, R_z3;


//========================================================六面中心点坐标
Point_3 P_F, P_R, P_U, P_B, P_L, P_D;
Point_3 P_FRUBLD[6];

MechanicalHandGroup MechanicalHandGroupLibrary[6][3][3][16];
//===========================================================================函数

//========================================================机械爪步骤初始化
void MechanicalStepint(void)
{
	//========name
	M_L1.name = "L1";
	M_L2.name = "L2";
	M_L3.name = "L3";
	M_LC.name = "LC";
	M_LO.name = "LO";

	M_R1.name = "R1";
	M_R2.name = "R2";
	M_R3.name = "R3";
	M_RC.name = "RC";
	M_RO.name = "RO";

	M_END.name = "M_END";


	//========time
	M_L1.time = HandMove90;
	M_L2.time = HandMove180;
	M_L3.time = HandMove90;
	M_LC.time = HandCloseTime;
	M_LO.time = HandOpenTime;

	M_R1.time = HandMove90;
	M_R2.time = HandMove180;
	M_R3.time = HandMove90;
	M_RC.time = HandCloseTime;
	M_RO.time = HandOpenTime;
	M_END.time = 0;


	//========num
	M_L1.num = L1;
	M_L2.num = L2;
	M_L3.num = L3;
	M_LC.num = LC;
	M_LO.num = LO;

	M_R1.num = R1;
	M_R2.num = R2;
	M_R3.num = R3;
	M_RC.num = RC;
	M_RO.num = RO;

	M_END.num = -1;
}


//=============================================================旋转矩阵初始化
void Rotint(void)
{
	R_x1.a[0][0] = 1;
	R_x1.a[1][2] = 1;
	R_x1.a[2][1] = -1;
	R_x2 = Rotmt(R_x1, R_x1);
	R_x3 = Rotmt(R_x1, R_x2);

	R_y1.a[0][2] = -1;
	R_y1.a[1][1] = 1;
	R_y1.a[2][0] = 1;
	R_y2 = Rotmt(R_y1, R_y1);
	R_y3 = Rotmt(R_y1, R_y2);

	R_z1.a[0][1] = 1;
	R_z1.a[1][0] = -1;
	R_z1.a[2][2] = 1;
	R_z2 = Rotmt(R_z1, R_z1);
	R_z3 = Rotmt(R_z1, R_z2);

}


//===========================================================矩阵运算
Rot Rotmt(Rot m, Rot n)
{
	Rot temp;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp.a[i][j] = 0;
		}
	}

	for (int k = 0; k < 3; k++)
	{
		int i;
		for (i = 0; i < 3; i++)
		{
			if (m.a[k][i] != 0)
				break;
		}

		int j;
		for (j = 0; j < 3; j++)
		{
			if (n.a[i][j] != 0)
				break;
		}
		
		if (m.a[k][i] == n.a[i][j])
			temp.a[k][j] = 1;	
		else
			temp.a[k][j] = -1;
	}

	return temp;
}


Point_3 Rotmtp3(Rot m, Point_3 n)
{
	Point_3 temp;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			temp.a[i][j] = m.a[i][0] * n.a[0][j] + m.a[i][1] * n.a[1][j] + m.a[i][2] * n.a[2][j];
		}
	}

	return temp;
}


//================================================================六面中心点空间坐标初始化
void Pointint(void)
{
	P_F.a[0][0] = 1;
	P_F.a[1][0] = 0;
	P_F.a[2][0] = 0;
	P_F.name = "F";
	P_FRUBLD[F] = P_F;

	P_R.a[0][0] = 0;
	P_R.a[1][0] = 1;
	P_R.a[2][0] = 0;
	P_R.name = "R";
	P_FRUBLD[R] = P_R;

	P_U.a[0][0] = 0;
	P_U.a[1][0] = 0;
	P_U.a[2][0] = 1;
	P_U.name = "U";
	P_FRUBLD[U] = P_U;

	P_B.a[0][0] = -1;
	P_B.a[1][0] = 0;
	P_B.a[2][0] = 0;
	P_B.name = "B";
	P_FRUBLD[B] = P_B;

	P_L.a[0][0] = 0;
	P_L.a[1][0] = -1;
	P_L.a[2][0] = 0;
	P_L.name = "L";
	P_FRUBLD[L] = P_L;

	P_D.a[0][0] = 0;
	P_D.a[1][0] = 0;
	P_D.a[2][0] = -1;
	P_D.name = "D";
	P_FRUBLD[D] = P_D;
}


//==================================================================初始化操作库
Rot temprot;
HandState1 tempstate;

void F1_L0R0int(void)
{
	//F1_L0R0_0
	MechanicalStep F1_L0R0_0[] = { M_L1, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][0].Set(1, F1_L0R0_0, temprot, tempstate);
	//F1_L0R0_1
	MechanicalStep F1_L0R0_1[] = { M_LO, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][1].Set(4, F1_L0R0_1, temprot, tempstate);
	//F1_L0R0_2
	MechanicalStep F1_L0R0_2[] = { M_RO, M_L3, M_RC, M_L1, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][2].Set(4, F1_L0R0_2, temprot, tempstate);
	//F1_L0R0_3
	MechanicalStep F1_L0R0_3[] = { M_RO, M_L1, M_RC, M_L1, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][3].Set(4, F1_L0R0_3, temprot, tempstate);
	//F1_L0R0_4
	MechanicalStep F1_L0R0_4[] = { M_RO, M_L2, M_RC, M_L1, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][4].Set(4, F1_L0R0_4, temprot, tempstate);
	//F1_L0R0_5
	MechanicalStep F1_L0R0_5[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][5].Set(11, F1_L0R0_5, temprot, tempstate);
	//F1_L0R0_6
	MechanicalStep F1_L0R0_6[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][6].Set(11, F1_L0R0_6, temprot, tempstate);
	//F1_L0R0_7
	MechanicalStep F1_L0R0_7[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][7].Set(12, F1_L0R0_7, temprot, tempstate);
	//F1_L0R0_8
	MechanicalStep F1_L0R0_8[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L3, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][8].Set(12, F1_L0R0_8, temprot, tempstate);
	//F1_L0R0_9
	MechanicalStep F1_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][9].Set(12, F1_L0R0_9, temprot, tempstate);
	//F1_L0R0_10
	MechanicalStep F1_L0R0_10[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][10].Set(12, F1_L0R0_10, temprot, tempstate);
	//F1_L0R0_11
	MechanicalStep F1_L0R0_11[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][11].Set(13, F1_L0R0_11, temprot, tempstate);
	//F1_L0R0_12
	MechanicalStep F1_L0R0_12[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][12].Set(15, F1_L0R0_12, temprot, tempstate);
	//F1_L0R0_13
	MechanicalStep F1_L0R0_13[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][13].Set(15, F1_L0R0_13, temprot, tempstate);
	//F1_L0R0_14
	MechanicalStep F1_L0R0_14[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][14].Set(16, F1_L0R0_14, temprot, tempstate);
	//F1_L0R0_15
	MechanicalStep F1_L0R0_15[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_1][L_0_R_0][15].Set(16, F1_L0R0_15, temprot, tempstate);
}
void F2_L0R0int(void)
{
	//F2_L0R0_0
	MechanicalStep F2_L0R0_0[] = { M_L2, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][0].Set(1, F2_L0R0_0, temprot, tempstate);
	//F2_L0R0_1
	MechanicalStep F2_L0R0_1[] = { M_RO, M_L2, M_RC, M_L2, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][1].Set(4, F2_L0R0_1, temprot, tempstate);
	//F2_L0R0_2
	MechanicalStep F2_L0R0_2[] = { M_LO, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][2].Set(4, F2_L0R0_2, temprot, tempstate);
	//F2_L0R0_3
	MechanicalStep F2_L0R0_3[] = { M_RO, M_L3, M_RC, M_L2, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][3].Set(4, F2_L0R0_3, temprot, tempstate);
	//F2_L0R0_4
	MechanicalStep F2_L0R0_4[] = { M_RO, M_L1, M_RC, M_L2, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][4].Set(4, F2_L0R0_4, temprot, tempstate);
	//F2_L0R0_5
	MechanicalStep F2_L0R0_5[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][5].Set(11, F2_L0R0_5, temprot, tempstate);
	//F2_L0R0_6
	MechanicalStep F2_L0R0_6[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][6].Set(11, F2_L0R0_6, temprot, tempstate);
	//F2_L0R0_7
	MechanicalStep F2_L0R0_7[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L3, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][7].Set(12, F2_L0R0_7, temprot, tempstate);
	//F2_L0R0_8
	MechanicalStep F2_L0R0_8[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][8].Set(12, F2_L0R0_8, temprot, tempstate);
	//F2_L0R0_9
	MechanicalStep F2_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][9].Set(12, F2_L0R0_9, temprot, tempstate);
	//F2_L0R0_10
	MechanicalStep F2_L0R0_10[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][10].Set(12, F2_L0R0_10, temprot, tempstate);
	//F2_L0R0_11
	MechanicalStep F2_L0R0_11[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][11].Set(13, F2_L0R0_11, temprot, tempstate);
	//F2_L0R0_12
	MechanicalStep F2_L0R0_12[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][12].Set(15, F2_L0R0_12, temprot, tempstate);
	//F2_L0R0_13
	MechanicalStep F2_L0R0_13[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][13].Set(15, F2_L0R0_13, temprot, tempstate);
	//F2_L0R0_14
	MechanicalStep F2_L0R0_14[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][14].Set(16, F2_L0R0_14, temprot, tempstate);
	//F2_L0R0_15
	MechanicalStep F2_L0R0_15[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_0][15].Set(16, F2_L0R0_15, temprot, tempstate);
}
void F3_L0R0int(void)
{
	//F3_L0R0_0
	MechanicalStep F3_L0R0_0[] = { M_L3, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][0].Set(1, F3_L0R0_0, temprot, tempstate);
	//F3_L0R0_1
	MechanicalStep F3_L0R0_1[] = { M_LO, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][1].Set(4, F3_L0R0_1, temprot, tempstate);
	//F3_L0R0_2
	MechanicalStep F3_L0R0_2[] = { M_RO, M_L3, M_RC, M_L3, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][2].Set(4, F3_L0R0_2, temprot, tempstate);
	//F3_L0R0_3
	MechanicalStep F3_L0R0_3[] = { M_RO, M_L1, M_RC, M_L3, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][3].Set(4, F3_L0R0_3, temprot, tempstate);
	//F3_L0R0_4
	MechanicalStep F3_L0R0_4[] = { M_RO, M_L2, M_RC, M_L3, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][4].Set(4, F3_L0R0_4, temprot, tempstate);
	//F3_L0R0_5
	MechanicalStep F3_L0R0_5[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][5].Set(11, F3_L0R0_5, temprot, tempstate);
	//F3_L0R0_6
	MechanicalStep F3_L0R0_6[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][6].Set(11, F3_L0R0_6, temprot, tempstate);
	//F3_L0R0_7
	MechanicalStep F3_L0R0_7[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L3, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][7].Set(12, F3_L0R0_7, temprot, tempstate);
	//F3_L0R0_8
	MechanicalStep F3_L0R0_8[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][8].Set(12, F3_L0R0_8, temprot, tempstate);
	//F3_L0R0_9
	MechanicalStep F3_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][9].Set(12, F3_L0R0_9, temprot, tempstate);
	//F3_L0R0_10
	MechanicalStep F3_L0R0_10[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][10].Set(12, F3_L0R0_10, temprot, tempstate);
	//F3_L0R0_11
	MechanicalStep F3_L0R0_11[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][11].Set(13, F3_L0R0_11, temprot, tempstate);
	//F3_L0R0_12
	MechanicalStep F3_L0R0_12[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][12].Set(15, F3_L0R0_12, temprot, tempstate);
	//F3_L0R0_13
	MechanicalStep F3_L0R0_13[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][13].Set(15, F3_L0R0_13, temprot, tempstate);
	//F3_L0R0_14
	MechanicalStep F3_L0R0_14[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][14].Set(16, F3_L0R0_14, temprot, tempstate);
	//F3_L0R0_15
	MechanicalStep F3_L0R0_15[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_3][L_0_R_0][15].Set(16, F3_L0R0_15, temprot, tempstate);
}
void R1_L0R0int(void)
{
	//R1_L0R0_0
	MechanicalStep R1_L0R0_0[] = { M_R1, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][0].Set(1, R1_L0R0_0, temprot, tempstate);
	//R1_L0R0_1
	MechanicalStep R1_L0R0_1[] = { M_LO, M_R3, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][1].Set(4, R1_L0R0_1, temprot, tempstate);
	//R1_L0R0_2
	MechanicalStep R1_L0R0_2[] = { M_LO, M_R1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][2].Set(4, R1_L0R0_2, temprot, tempstate);
	//R1_L0R0_3
	MechanicalStep R1_L0R0_3[] = { M_RO, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][3].Set(4, R1_L0R0_3, temprot, tempstate);
	//R1_L0R0_4
	MechanicalStep R1_L0R0_4[] = { M_LO, M_R2, M_LC, M_R1, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][4].Set(4, R1_L0R0_4, temprot, tempstate);
	//R1_L0R0_5
	MechanicalStep R1_L0R0_5[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][5].Set(11, R1_L0R0_5, temprot, tempstate);
	//R1_L0R0_6
	MechanicalStep R1_L0R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][6].Set(11, R1_L0R0_6, temprot, tempstate);
	//R1_L0R0_7
	MechanicalStep R1_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][7].Set(12, R1_L0R0_7, temprot, tempstate);
	//R1_L0R0_8
	MechanicalStep R1_L0R0_8[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][8].Set(12, R1_L0R0_8, temprot, tempstate);
	//R1_L0R0_9
	MechanicalStep R1_L0R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][9].Set(12, R1_L0R0_9, temprot, tempstate);
	//R1_L0R0_10
	MechanicalStep R1_L0R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][10].Set(12, R1_L0R0_10, temprot, tempstate);
	//R1_L0R0_11
	MechanicalStep R1_L0R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][11].Set(13, R1_L0R0_11, temprot, tempstate);
	//R1_L0R0_12
	MechanicalStep R1_L0R0_12[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][12].Set(15, R1_L0R0_12, temprot, tempstate);
	//R1_L0R0_13
	MechanicalStep R1_L0R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][13].Set(15, R1_L0R0_13, temprot, tempstate);
	//R1_L0R0_14
	MechanicalStep R1_L0R0_14[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][14].Set(16, R1_L0R0_14, temprot, tempstate);
	//R1_L0R0_15
	MechanicalStep R1_L0R0_15[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_0][15].Set(16, R1_L0R0_15, temprot, tempstate);
}
void R2_L0R0int(void)
{
	//R2_L0R0_0
	MechanicalStep R2_L0R0_0[] = { M_R2, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][0].Set(1, R2_L0R0_0, temprot, tempstate);
	//R2_L0R0_1
	MechanicalStep R2_L0R0_1[] = { M_LO, M_R2, M_LC, M_R2, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][1].Set(4, R2_L0R0_1, temprot, tempstate);
	//R2_L0R0_2
	MechanicalStep R2_L0R0_2[] = { M_LO, M_R3, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][2].Set(4, R2_L0R0_2, temprot, tempstate);
	//R2_L0R0_3
	MechanicalStep R2_L0R0_3[] = { M_LO, M_R1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][3].Set(4, R2_L0R0_3, temprot, tempstate);
	//R2_L0R0_4
	MechanicalStep R2_L0R0_4[] = { M_RO, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][4].Set(4, R2_L0R0_4, temprot, tempstate);
	//R2_L0R0_5
	MechanicalStep R2_L0R0_5[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][5].Set(11, R2_L0R0_5, temprot, tempstate);
	//R2_L0R0_6
	MechanicalStep R2_L0R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][6].Set(11, R2_L0R0_6, temprot, tempstate);
	//R2_L0R0_7
	MechanicalStep R2_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][7].Set(12, R2_L0R0_7, temprot, tempstate);
	//R2_L0R0_8
	MechanicalStep R2_L0R0_8[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][8].Set(12, R2_L0R0_8, temprot, tempstate);
	//R2_L0R0_9
	MechanicalStep R2_L0R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][9].Set(12, R2_L0R0_9, temprot, tempstate);
	//R2_L0R0_10
	MechanicalStep R2_L0R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][10].Set(12, R2_L0R0_10, temprot, tempstate);
	//R2_L0R0_11
	MechanicalStep R2_L0R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][11].Set(13, R2_L0R0_11, temprot, tempstate);
	//R2_L0R0_12
	MechanicalStep R2_L0R0_12[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][12].Set(15, R2_L0R0_12, temprot, tempstate);
	//R2_L0R0_13
	MechanicalStep R2_L0R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][13].Set(15, R2_L0R0_13, temprot, tempstate);
	//R2_L0R0_14
	MechanicalStep R2_L0R0_14[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][14].Set(16, R2_L0R0_14, temprot, tempstate);
	//R2_L0R0_15
	MechanicalStep R2_L0R0_15[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_0][15].Set(16, R2_L0R0_15, temprot, tempstate);
}
void R3_L0R0int(void)
{
	//R3_L0R0_0
	MechanicalStep R3_L0R0_0[] = { M_R3, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][0].Set(1, R3_L0R0_0, temprot, tempstate);
	//R3_L0R0_1
	MechanicalStep R3_L0R0_1[] = { M_LO, M_R3, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][1].Set(4, R3_L0R0_1, temprot, tempstate);
	//R3_L0R0_2
	MechanicalStep R3_L0R0_2[] = { M_LO, M_R1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][2].Set(4, R3_L0R0_2, temprot, tempstate);
	//R3_L0R0_3
	MechanicalStep R3_L0R0_3[] = { M_RO, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][3].Set(4, R3_L0R0_3, temprot, tempstate);
	//R3_L0R0_4
	MechanicalStep R3_L0R0_4[] = { M_LO, M_R2, M_LC, M_R3, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][4].Set(4, R3_L0R0_4, temprot, tempstate);
	//R3_L0R0_5
	MechanicalStep R3_L0R0_5[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][5].Set(11, R3_L0R0_5, temprot, tempstate);
	//R3_L0R0_6
	MechanicalStep R3_L0R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][6].Set(11, R3_L0R0_6, temprot, tempstate);
	//R3_L0R0_7
	MechanicalStep R3_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][7].Set(12, R3_L0R0_7, temprot, tempstate);
	//R3_L0R0_8
	MechanicalStep R3_L0R0_8[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][8].Set(12, R3_L0R0_8, temprot, tempstate);
	//R3_L0R0_9
	MechanicalStep R3_L0R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][9].Set(12, R3_L0R0_9, temprot, tempstate);
	//R3_L0R0_10
	MechanicalStep R3_L0R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][10].Set(12, R3_L0R0_10, temprot, tempstate);
	//R3_L0R0_11
	MechanicalStep R3_L0R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][11].Set(13, R3_L0R0_11, temprot, tempstate);
	//R3_L0R0_12
	MechanicalStep R3_L0R0_12[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][12].Set(15, R3_L0R0_12, temprot, tempstate);
	//R3_L0R0_13
	MechanicalStep R3_L0R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][13].Set(15, R3_L0R0_13, temprot, tempstate);
	//R3_L0R0_14
	MechanicalStep R3_L0R0_14[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][14].Set(16, R3_L0R0_14, temprot, tempstate);
	//R3_L0R0_15
	MechanicalStep R3_L0R0_15[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_0][15].Set(16, R3_L0R0_15, temprot, tempstate);
}
void U1_L0R0int(void)
{
	//U1_L0R0_0
	MechanicalStep U1_L0R0_0[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][0].Set(7, U1_L0R0_0, temprot, tempstate);
	//U1_L0R0_1
	MechanicalStep U1_L0R0_1[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][1].Set(7, U1_L0R0_1, temprot, tempstate);
	//U1_L0R0_2
	MechanicalStep U1_L0R0_2[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][2].Set(8, U1_L0R0_2, temprot, tempstate);
	//U1_L0R0_3
	MechanicalStep U1_L0R0_3[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][3].Set(8, U1_L0R0_3, temprot, tempstate);
	//U1_L0R0_4
	MechanicalStep U1_L0R0_4[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][4].Set(8, U1_L0R0_4, temprot, tempstate);
	//U1_L0R0_5
	MechanicalStep U1_L0R0_5[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][5].Set(8, U1_L0R0_5, temprot, tempstate);
	//U1_L0R0_6
	MechanicalStep U1_L0R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][6].Set(8, U1_L0R0_6, temprot, tempstate);
	//U1_L0R0_7
	MechanicalStep U1_L0R0_7[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][7].Set(8, U1_L0R0_7, temprot, tempstate);
	//U1_L0R0_8
	MechanicalStep U1_L0R0_8[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][8].Set(8, U1_L0R0_8, temprot, tempstate);
	//U1_L0R0_9
	MechanicalStep U1_L0R0_9[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_L1, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][9].Set(8, U1_L0R0_9, temprot, tempstate);
	//U1_L0R0_10
	MechanicalStep U1_L0R0_10[] = { M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][10].Set(9, U1_L0R0_10, temprot, tempstate);
	//U1_L0R0_11
	MechanicalStep U1_L0R0_11[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][11].Set(9, U1_L0R0_11, temprot, tempstate);
	//U1_L0R0_12
	MechanicalStep U1_L0R0_12[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][12].Set(12, U1_L0R0_12, temprot, tempstate);
	//U1_L0R0_13
	MechanicalStep U1_L0R0_13[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][13].Set(12, U1_L0R0_13, temprot, tempstate);
	//U1_L0R0_14
	MechanicalStep U1_L0R0_14[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][14].Set(12, U1_L0R0_14, temprot, tempstate);
	//U1_L0R0_15
	MechanicalStep U1_L0R0_15[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_1][L_0_R_0][15].Set(12, U1_L0R0_15, temprot, tempstate);
}
void U2_L0R0int(void)
{
	//U2_L0R0_0
	MechanicalStep U2_L0R0_0[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][0].Set(7, U2_L0R0_0, temprot, tempstate);
	//U2_L0R0_1
	MechanicalStep U2_L0R0_1[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][1].Set(7, U2_L0R0_1, temprot, tempstate);
	//U2_L0R0_2
	MechanicalStep U2_L0R0_2[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_L2, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][2].Set(8, U2_L0R0_2, temprot, tempstate);
	//U2_L0R0_3
	MechanicalStep U2_L0R0_3[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][3].Set(8, U2_L0R0_3, temprot, tempstate);
	//U2_L0R0_4
	MechanicalStep U2_L0R0_4[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][4].Set(8, U2_L0R0_4, temprot, tempstate);
	//U2_L0R0_5
	MechanicalStep U2_L0R0_5[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][5].Set(8, U2_L0R0_5, temprot, tempstate);
	//U2_L0R0_6
	MechanicalStep U2_L0R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][6].Set(8, U2_L0R0_6, temprot, tempstate);
	//U2_L0R0_7
	MechanicalStep U2_L0R0_7[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][7].Set(8, U2_L0R0_7, temprot, tempstate);
	//U2_L0R0_8
	MechanicalStep U2_L0R0_8[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][8].Set(8, U2_L0R0_8, temprot, tempstate);
	//U2_L0R0_9
	MechanicalStep U2_L0R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][9].Set(8, U2_L0R0_9, temprot, tempstate);
	//U2_L0R0_10
	MechanicalStep U2_L0R0_10[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][10].Set(9, U2_L0R0_10, temprot, tempstate);
	//U2_L0R0_11
	MechanicalStep U2_L0R0_11[] = { M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][11].Set(9, U2_L0R0_11, temprot, tempstate);
	//U2_L0R0_12
	MechanicalStep U2_L0R0_12[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][12].Set(12, U2_L0R0_12, temprot, tempstate);
	//U2_L0R0_13
	MechanicalStep U2_L0R0_13[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][13].Set(12, U2_L0R0_13, temprot, tempstate);
	//U2_L0R0_14
	MechanicalStep U2_L0R0_14[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][14].Set(12, U2_L0R0_14, temprot, tempstate);
	//U2_L0R0_15
	MechanicalStep U2_L0R0_15[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_0][15].Set(12, U2_L0R0_15, temprot, tempstate);
}
void U3_L0R0int(void)
{
	//U3_L0R0_0
	MechanicalStep U3_L0R0_0[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][0].Set(7, U3_L0R0_0, temprot, tempstate);
	//U3_L0R0_1
	MechanicalStep U3_L0R0_1[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][1].Set(7, U3_L0R0_1, temprot, tempstate);
	//U3_L0R0_2
	MechanicalStep U3_L0R0_2[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][2].Set(8, U3_L0R0_2, temprot, tempstate);
	//U3_L0R0_3
	MechanicalStep U3_L0R0_3[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][3].Set(8, U3_L0R0_3, temprot, tempstate);
	//U3_L0R0_4
	MechanicalStep U3_L0R0_4[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][4].Set(8, U3_L0R0_4, temprot, tempstate);
	//U3_L0R0_5
	MechanicalStep U3_L0R0_5[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][5].Set(8, U3_L0R0_5, temprot, tempstate);
	//U3_L0R0_6
	MechanicalStep U3_L0R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][6].Set(8, U3_L0R0_6, temprot, tempstate);
	//U3_L0R0_7
	MechanicalStep U3_L0R0_7[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][7].Set(8, U3_L0R0_7, temprot, tempstate);
	//U3_L0R0_8
	MechanicalStep U3_L0R0_8[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_L3, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][8].Set(8, U3_L0R0_8, temprot, tempstate);
	//U3_L0R0_9
	MechanicalStep U3_L0R0_9[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][9].Set(8, U3_L0R0_9, temprot, tempstate);
	//U3_L0R0_10
	MechanicalStep U3_L0R0_10[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][10].Set(9, U3_L0R0_10, temprot, tempstate);
	//U3_L0R0_11
	MechanicalStep U3_L0R0_11[] = { M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][11].Set(9, U3_L0R0_11, temprot, tempstate);
	//U3_L0R0_12
	MechanicalStep U3_L0R0_12[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][12].Set(12, U3_L0R0_12, temprot, tempstate);
	//U3_L0R0_13
	MechanicalStep U3_L0R0_13[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][13].Set(12, U3_L0R0_13, temprot, tempstate);
	//U3_L0R0_14
	MechanicalStep U3_L0R0_14[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][14].Set(12, U3_L0R0_14, temprot, tempstate);
	//U3_L0R0_15
	MechanicalStep U3_L0R0_15[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_3][L_0_R_0][15].Set(12, U3_L0R0_15, temprot, tempstate);
}
void B1_L0R0int(void)
{
	//B1_L0R0_0
	MechanicalStep B1_L0R0_0[] = { M_LO, M_R2, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][0].Set(4, B1_L0R0_0, temprot, tempstate);
	//B1_L0R0_1
	MechanicalStep B1_L0R0_1[] = { M_LO, M_R2, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][1].Set(5, B1_L0R0_1, temprot, tempstate);
	//B1_L0R0_2
	MechanicalStep B1_L0R0_2[] = { M_RO, M_L2, M_RC, M_LO, M_R2, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][2].Set(8, B1_L0R0_2, temprot, tempstate);
	//B1_L0R0_3
	MechanicalStep B1_L0R0_3[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][3].Set(8, B1_L0R0_3, temprot, tempstate);
	//B1_L0R0_4
	MechanicalStep B1_L0R0_4[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][4].Set(8, B1_L0R0_4, temprot, tempstate);
	//B1_L0R0_5
	MechanicalStep B1_L0R0_5[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][5].Set(9, B1_L0R0_5, temprot, tempstate);
	//B1_L0R0_6
	MechanicalStep B1_L0R0_6[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][6].Set(9, B1_L0R0_6, temprot, tempstate);
	//B1_L0R0_7
	MechanicalStep B1_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][7].Set(11, B1_L0R0_7, temprot, tempstate);
	//B1_L0R0_8
	MechanicalStep B1_L0R0_8[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][8].Set(11, B1_L0R0_8, temprot, tempstate);
	//B1_L0R0_9
	MechanicalStep B1_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][9].Set(11, B1_L0R0_9, temprot, tempstate);
	//B1_L0R0_10
	MechanicalStep B1_L0R0_10[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][10].Set(12, B1_L0R0_10, temprot, tempstate);
	//B1_L0R0_11
	MechanicalStep B1_L0R0_11[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][11].Set(12, B1_L0R0_11, temprot, tempstate);
	//B1_L0R0_12
	MechanicalStep B1_L0R0_12[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][12].Set(12, B1_L0R0_12, temprot, tempstate);
	//B1_L0R0_13
	MechanicalStep B1_L0R0_13[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][13].Set(12, B1_L0R0_13, temprot, tempstate);
	//B1_L0R0_14
	MechanicalStep B1_L0R0_14[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][14].Set(15, B1_L0R0_14, temprot, tempstate);
	//B1_L0R0_15
	MechanicalStep B1_L0R0_15[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_1][L_0_R_0][15].Set(15, B1_L0R0_15, temprot, tempstate);
}
void B2_L0R0int(void)
{
	//B2_L0R0_0
	MechanicalStep B2_L0R0_0[] = { M_LO, M_R2, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][0].Set(4, B2_L0R0_0, temprot, tempstate);
	//B2_L0R0_1
	MechanicalStep B2_L0R0_1[] = { M_LO, M_R2, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][1].Set(5, B2_L0R0_1, temprot, tempstate);
	//B2_L0R0_2
	MechanicalStep B2_L0R0_2[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][2].Set(8, B2_L0R0_2, temprot, tempstate);
	//B2_L0R0_3
	MechanicalStep B2_L0R0_3[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][3].Set(8, B2_L0R0_3, temprot, tempstate);
	//B2_L0R0_4
	MechanicalStep B2_L0R0_4[] = { M_RO, M_L2, M_RC, M_LO, M_R2, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][4].Set(8, B2_L0R0_4, temprot, tempstate);
	//B2_L0R0_5
	MechanicalStep B2_L0R0_5[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][5].Set(9, B2_L0R0_5, temprot, tempstate);
	//B2_L0R0_6
	MechanicalStep B2_L0R0_6[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][6].Set(9, B2_L0R0_6, temprot, tempstate);
	//B2_L0R0_7
	MechanicalStep B2_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][7].Set(11, B2_L0R0_7, temprot, tempstate);
	//B2_L0R0_8
	MechanicalStep B2_L0R0_8[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][8].Set(11, B2_L0R0_8, temprot, tempstate);
	//B2_L0R0_9
	MechanicalStep B2_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][9].Set(11, B2_L0R0_9, temprot, tempstate);
	//B2_L0R0_10
	MechanicalStep B2_L0R0_10[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][10].Set(12, B2_L0R0_10, temprot, tempstate);
	//B2_L0R0_11
	MechanicalStep B2_L0R0_11[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][11].Set(12, B2_L0R0_11, temprot, tempstate);
	//B2_L0R0_12
	MechanicalStep B2_L0R0_12[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][12].Set(12, B2_L0R0_12, temprot, tempstate);
	//B2_L0R0_13
	MechanicalStep B2_L0R0_13[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][13].Set(12, B2_L0R0_13, temprot, tempstate);
	//B2_L0R0_14
	MechanicalStep B2_L0R0_14[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][14].Set(15, B2_L0R0_14, temprot, tempstate);
	//B2_L0R0_15
	MechanicalStep B2_L0R0_15[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_0][15].Set(15, B2_L0R0_15, temprot, tempstate);
}
void B3_L0R0int(void)
{
	//B3_L0R0_0
	MechanicalStep B3_L0R0_0[] = { M_LO, M_R2, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][0].Set(4, B3_L0R0_0, temprot, tempstate);
	//B3_L0R0_1
	MechanicalStep B3_L0R0_1[] = { M_LO, M_R2, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][1].Set(5, B3_L0R0_1, temprot, tempstate);
	//B3_L0R0_2
	MechanicalStep B3_L0R0_2[] = { M_RO, M_L2, M_RC, M_LO, M_R2, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][2].Set(8, B3_L0R0_2, temprot, tempstate);
	//B3_L0R0_3
	MechanicalStep B3_L0R0_3[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][3].Set(8, B3_L0R0_3, temprot, tempstate);
	//B3_L0R0_4
	MechanicalStep B3_L0R0_4[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][4].Set(8, B3_L0R0_4, temprot, tempstate);
	//B3_L0R0_5
	MechanicalStep B3_L0R0_5[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][5].Set(9, B3_L0R0_5, temprot, tempstate);
	//B3_L0R0_6
	MechanicalStep B3_L0R0_6[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][6].Set(9, B3_L0R0_6, temprot, tempstate);
	//B3_L0R0_7
	MechanicalStep B3_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][7].Set(11, B3_L0R0_7, temprot, tempstate);
	//B3_L0R0_8
	MechanicalStep B3_L0R0_8[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][8].Set(11, B3_L0R0_8, temprot, tempstate);
	//B3_L0R0_9
	MechanicalStep B3_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][9].Set(11, B3_L0R0_9, temprot, tempstate);
	//B3_L0R0_10
	MechanicalStep B3_L0R0_10[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][10].Set(12, B3_L0R0_10, temprot, tempstate);
	//B3_L0R0_11
	MechanicalStep B3_L0R0_11[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][11].Set(12, B3_L0R0_11, temprot, tempstate);
	//B3_L0R0_12
	MechanicalStep B3_L0R0_12[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][12].Set(12, B3_L0R0_12, temprot, tempstate);
	//B3_L0R0_13
	MechanicalStep B3_L0R0_13[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][13].Set(12, B3_L0R0_13, temprot, tempstate);
	//B3_L0R0_14
	MechanicalStep B3_L0R0_14[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][14].Set(15, B3_L0R0_14, temprot, tempstate);
	//B3_L0R0_15
	MechanicalStep B3_L0R0_15[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_3][L_0_R_0][15].Set(15, B3_L0R0_15, temprot, tempstate);
}
void L1_L0R0int(void)
{
	//L1_L0R0_0
	MechanicalStep L1_L0R0_0[] = { M_RO, M_L2, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][0].Set(4, L1_L0R0_0, temprot, tempstate);
	//L1_L0R0_1
	MechanicalStep L1_L0R0_1[] = { M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][1].Set(5, L1_L0R0_1, temprot, tempstate);
	//L1_L0R0_2
	MechanicalStep L1_L0R0_2[] = { M_LO, M_R2, M_LC, M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][2].Set(8, L1_L0R0_2, temprot, tempstate);
	//L1_L0R0_3
	MechanicalStep L1_L0R0_3[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][3].Set(8, L1_L0R0_3, temprot, tempstate);
	//L1_L0R0_4
	MechanicalStep L1_L0R0_4[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][4].Set(8, L1_L0R0_4, temprot, tempstate);
	//L1_L0R0_5
	MechanicalStep L1_L0R0_5[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][5].Set(9, L1_L0R0_5, temprot, tempstate);
	//L1_L0R0_6
	MechanicalStep L1_L0R0_6[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][6].Set(9, L1_L0R0_6, temprot, tempstate);
	//L1_L0R0_7
	MechanicalStep L1_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][7].Set(11, L1_L0R0_7, temprot, tempstate);
	//L1_L0R0_8
	MechanicalStep L1_L0R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_R1, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][8].Set(11, L1_L0R0_8, temprot, tempstate);
	//L1_L0R0_9
	MechanicalStep L1_L0R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][9].Set(11, L1_L0R0_9, temprot, tempstate);
	//L1_L0R0_10
	MechanicalStep L1_L0R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][10].Set(12, L1_L0R0_10, temprot, tempstate);
	//L1_L0R0_11
	MechanicalStep L1_L0R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][11].Set(12, L1_L0R0_11, temprot, tempstate);
	//L1_L0R0_12
	MechanicalStep L1_L0R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][12].Set(12, L1_L0R0_12, temprot, tempstate);
	//L1_L0R0_13
	MechanicalStep L1_L0R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][13].Set(12, L1_L0R0_13, temprot, tempstate);
	//L1_L0R0_14
	MechanicalStep L1_L0R0_14[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][14].Set(15, L1_L0R0_14, temprot, tempstate);
	//L1_L0R0_15
	MechanicalStep L1_L0R0_15[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_0][15].Set(15, L1_L0R0_15, temprot, tempstate);
}
void L2_L0R0int(void)
{
	//L2_L0R0_0
	MechanicalStep L2_L0R0_0[] = { M_RO, M_L2, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][0].Set(4, L2_L0R0_0, temprot, tempstate);
	//L2_L0R0_1
	MechanicalStep L2_L0R0_1[] = { M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][1].Set(5, L2_L0R0_1, temprot, tempstate);
	//L2_L0R0_2
	MechanicalStep L2_L0R0_2[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][2].Set(8, L2_L0R0_2, temprot, tempstate);
	//L2_L0R0_3
	MechanicalStep L2_L0R0_3[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][3].Set(8, L2_L0R0_3, temprot, tempstate);
	//L2_L0R0_4
	MechanicalStep L2_L0R0_4[] = { M_LO, M_R2, M_LC, M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][4].Set(8, L2_L0R0_4, temprot, tempstate);
	//L2_L0R0_5
	MechanicalStep L2_L0R0_5[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][5].Set(9, L2_L0R0_5, temprot, tempstate);
	//L2_L0R0_6
	MechanicalStep L2_L0R0_6[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][6].Set(9, L2_L0R0_6, temprot, tempstate);
	//L2_L0R0_7
	MechanicalStep L2_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][7].Set(11, L2_L0R0_7, temprot, tempstate);
	//L2_L0R0_8
	MechanicalStep L2_L0R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][8].Set(11, L2_L0R0_8, temprot, tempstate);
	//L2_L0R0_9
	MechanicalStep L2_L0R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_R2, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][9].Set(11, L2_L0R0_9, temprot, tempstate);
	//L2_L0R0_10
	MechanicalStep L2_L0R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][10].Set(12, L2_L0R0_10, temprot, tempstate);
	//L2_L0R0_11
	MechanicalStep L2_L0R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][11].Set(12, L2_L0R0_11, temprot, tempstate);
	//L2_L0R0_12
	MechanicalStep L2_L0R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][12].Set(12, L2_L0R0_12, temprot, tempstate);
	//L2_L0R0_13
	MechanicalStep L2_L0R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][13].Set(12, L2_L0R0_13, temprot, tempstate);
	//L2_L0R0_14
	MechanicalStep L2_L0R0_14[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][14].Set(15, L2_L0R0_14, temprot, tempstate);
	//L2_L0R0_15
	MechanicalStep L2_L0R0_15[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_0][15].Set(15, L2_L0R0_15, temprot, tempstate);
}
void L3_L0R0int(void)
{
	//L3_L0R0_0
	MechanicalStep L3_L0R0_0[] = { M_RO, M_L2, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][0].Set(4, L3_L0R0_0, temprot, tempstate);
	//L3_L0R0_1
	MechanicalStep L3_L0R0_1[] = { M_RO, M_L2, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][1].Set(5, L3_L0R0_1, temprot, tempstate);
	//L3_L0R0_2
	MechanicalStep L3_L0R0_2[] = { M_LO, M_R2, M_LC, M_RO, M_L2, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][2].Set(8, L3_L0R0_2, temprot, tempstate);
	//L3_L0R0_3
	MechanicalStep L3_L0R0_3[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][3].Set(8, L3_L0R0_3, temprot, tempstate);
	//L3_L0R0_4
	MechanicalStep L3_L0R0_4[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][4].Set(8, L3_L0R0_4, temprot, tempstate);
	//L3_L0R0_5
	MechanicalStep L3_L0R0_5[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R3, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][5].Set(9, L3_L0R0_5, temprot, tempstate);
	//L3_L0R0_6
	MechanicalStep L3_L0R0_6[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R3, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][6].Set(9, L3_L0R0_6, temprot, tempstate);
	//L3_L0R0_7
	MechanicalStep L3_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][7].Set(11, L3_L0R0_7, temprot, tempstate);
	//L3_L0R0_8
	MechanicalStep L3_L0R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][8].Set(11, L3_L0R0_8, temprot, tempstate);
	//L3_L0R0_9
	MechanicalStep L3_L0R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_R3, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][9].Set(11, L3_L0R0_9, temprot, tempstate);
	//L3_L0R0_10
	MechanicalStep L3_L0R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][10].Set(12, L3_L0R0_10, temprot, tempstate);
	//L3_L0R0_11
	MechanicalStep L3_L0R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][11].Set(12, L3_L0R0_11, temprot, tempstate);
	//L3_L0R0_12
	MechanicalStep L3_L0R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][12].Set(12, L3_L0R0_12, temprot, tempstate);
	//L3_L0R0_13
	MechanicalStep L3_L0R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][13].Set(12, L3_L0R0_13, temprot, tempstate);
	//L3_L0R0_14
	MechanicalStep L3_L0R0_14[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][14].Set(15, L3_L0R0_14, temprot, tempstate);
	//L3_L0R0_15
	MechanicalStep L3_L0R0_15[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_0][15].Set(15, L3_L0R0_15, temprot, tempstate);
}
void D1_L0R0int(void)
{
	//D1_L0R0_0
	MechanicalStep D1_L0R0_0[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][0].Set(7, D1_L0R0_0, temprot, tempstate);
	//D1_L0R0_1
	MechanicalStep D1_L0R0_1[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][1].Set(7, D1_L0R0_1, temprot, tempstate);
	//D1_L0R0_2
	MechanicalStep D1_L0R0_2[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][2].Set(8, D1_L0R0_2, temprot, tempstate);
	//D1_L0R0_3
	MechanicalStep D1_L0R0_3[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][3].Set(8, D1_L0R0_3, temprot, tempstate);
	//D1_L0R0_4
	MechanicalStep D1_L0R0_4[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][4].Set(8, D1_L0R0_4, temprot, tempstate);
	//D1_L0R0_5
	MechanicalStep D1_L0R0_5[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][5].Set(8, D1_L0R0_5, temprot, tempstate);
	//D1_L0R0_6
	MechanicalStep D1_L0R0_6[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][6].Set(8, D1_L0R0_6, temprot, tempstate);
	//D1_L0R0_7
	MechanicalStep D1_L0R0_7[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][7].Set(8, D1_L0R0_7, temprot, tempstate);
	//D1_L0R0_8
	MechanicalStep D1_L0R0_8[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][8].Set(8, D1_L0R0_8, temprot, tempstate);
	//D1_L0R0_9
	MechanicalStep D1_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_L1, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][9].Set(8, D1_L0R0_9, temprot, tempstate);
	//D1_L0R0_10
	MechanicalStep D1_L0R0_10[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][10].Set(9, D1_L0R0_10, temprot, tempstate);
	//D1_L0R0_11
	MechanicalStep D1_L0R0_11[] = { M_RO, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][11].Set(9, D1_L0R0_11, temprot, tempstate);
	//D1_L0R0_12
	MechanicalStep D1_L0R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][12].Set(12, D1_L0R0_12, temprot, tempstate);
	//D1_L0R0_13
	MechanicalStep D1_L0R0_13[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][13].Set(12, D1_L0R0_13, temprot, tempstate);
	//D1_L0R0_14
	MechanicalStep D1_L0R0_14[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][14].Set(12, D1_L0R0_14, temprot, tempstate);
	//D1_L0R0_15
	MechanicalStep D1_L0R0_15[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_1][L_0_R_0][15].Set(12, D1_L0R0_15, temprot, tempstate);
}
void D2_L0R0int(void)
{
	//D2_L0R0_0
	MechanicalStep D2_L0R0_0[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][0].Set(7, D2_L0R0_0, temprot, tempstate);
	//D2_L0R0_1
	MechanicalStep D2_L0R0_1[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][1].Set(7, D2_L0R0_1, temprot, tempstate);
	//D2_L0R0_2
	MechanicalStep D2_L0R0_2[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][2].Set(8, D2_L0R0_2, temprot, tempstate);
	//D2_L0R0_3
	MechanicalStep D2_L0R0_3[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_L2, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][3].Set(8, D2_L0R0_3, temprot, tempstate);
	//D2_L0R0_4
	MechanicalStep D2_L0R0_4[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][4].Set(8, D2_L0R0_4, temprot, tempstate);
	//D2_L0R0_5
	MechanicalStep D2_L0R0_5[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][5].Set(8, D2_L0R0_5, temprot, tempstate);
	//D2_L0R0_6
	MechanicalStep D2_L0R0_6[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][6].Set(8, D2_L0R0_6, temprot, tempstate);
	//D2_L0R0_7
	MechanicalStep D2_L0R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][7].Set(8, D2_L0R0_7, temprot, tempstate);
	//D2_L0R0_8
	MechanicalStep D2_L0R0_8[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][8].Set(8, D2_L0R0_8, temprot, tempstate);
	//D2_L0R0_9
	MechanicalStep D2_L0R0_9[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][9].Set(8, D2_L0R0_9, temprot, tempstate);
	//D2_L0R0_10
	MechanicalStep D2_L0R0_10[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][10].Set(9, D2_L0R0_10, temprot, tempstate);
	//D2_L0R0_11
	MechanicalStep D2_L0R0_11[] = { M_RO, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][11].Set(9, D2_L0R0_11, temprot, tempstate);
	//D2_L0R0_12
	MechanicalStep D2_L0R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][12].Set(12, D2_L0R0_12, temprot, tempstate);
	//D2_L0R0_13
	MechanicalStep D2_L0R0_13[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][13].Set(12, D2_L0R0_13, temprot, tempstate);
	//D2_L0R0_14
	MechanicalStep D2_L0R0_14[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][14].Set(12, D2_L0R0_14, temprot, tempstate);
	//D2_L0R0_15
	MechanicalStep D2_L0R0_15[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_0][15].Set(12, D2_L0R0_15, temprot, tempstate);
}
void D3_L0R0int(void)
{
	//D3_L0R0_0
	MechanicalStep D3_L0R0_0[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][0].Set(7, D3_L0R0_0, temprot, tempstate);
	//D3_L0R0_1
	MechanicalStep D3_L0R0_1[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][1].Set(7, D3_L0R0_1, temprot, tempstate);
	//D3_L0R0_2
	MechanicalStep D3_L0R0_2[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][2].Set(8, D3_L0R0_2, temprot, tempstate);
	//D3_L0R0_3
	MechanicalStep D3_L0R0_3[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][3].Set(8, D3_L0R0_3, temprot, tempstate);
	//D3_L0R0_4
	MechanicalStep D3_L0R0_4[] = { M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][4].Set(8, D3_L0R0_4, temprot, tempstate);
	//D3_L0R0_5
	MechanicalStep D3_L0R0_5[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][5].Set(8, D3_L0R0_5, temprot, tempstate);
	//D3_L0R0_6
	MechanicalStep D3_L0R0_6[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][6].Set(8, D3_L0R0_6, temprot, tempstate);
	//D3_L0R0_7
	MechanicalStep D3_L0R0_7[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][7].Set(8, D3_L0R0_7, temprot, tempstate);
	//D3_L0R0_8
	MechanicalStep D3_L0R0_8[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][8].Set(8, D3_L0R0_8, temprot, tempstate);
	//D3_L0R0_9
	MechanicalStep D3_L0R0_9[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_L3, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][9].Set(8, D3_L0R0_9, temprot, tempstate);
	//D3_L0R0_10
	MechanicalStep D3_L0R0_10[] = { M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][10].Set(9, D3_L0R0_10, temprot, tempstate);
	//D3_L0R0_11
	MechanicalStep D3_L0R0_11[] = { M_RO, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][11].Set(9, D3_L0R0_11, temprot, tempstate);
	//D3_L0R0_12
	MechanicalStep D3_L0R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][12].Set(12, D3_L0R0_12, temprot, tempstate);
	//D3_L0R0_13
	MechanicalStep D3_L0R0_13[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][13].Set(12, D3_L0R0_13, temprot, tempstate);
	//D3_L0R0_14
	MechanicalStep D3_L0R0_14[] = { M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][14].Set(12, D3_L0R0_14, temprot, tempstate);
	//D3_L0R0_15
	MechanicalStep D3_L0R0_15[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_3][L_0_R_0][15].Set(12, D3_L0R0_15, temprot, tempstate);
}
void F1_L0R1int(void)
{
	//F1_L0R1_0
	MechanicalStep F1_L0R1_0[] = { M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][0].Set(4, F1_L0R1_0, temprot, tempstate);
	//F1_L0R1_1
	MechanicalStep F1_L0R1_1[] = { M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][1].Set(5, F1_L0R1_1, temprot, tempstate);
	//F1_L0R1_2
	MechanicalStep F1_L0R1_2[] = { M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][2].Set(5, F1_L0R1_2, temprot, tempstate);
	//F1_L0R1_3
	MechanicalStep F1_L0R1_3[] = { M_RO, M_R1, M_L2, M_RC, M_L1, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][3].Set(5, F1_L0R1_3, temprot, tempstate);
	//F1_L0R1_4
	MechanicalStep F1_L0R1_4[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][4].Set(8, F1_L0R1_4, temprot, tempstate);
	//F1_L0R1_5
	MechanicalStep F1_L0R1_5[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][5].Set(8, F1_L0R1_5, temprot, tempstate);
	//F1_L0R1_6
	MechanicalStep F1_L0R1_6[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][6].Set(9, F1_L0R1_6, temprot, tempstate);
	//F1_L0R1_7
	MechanicalStep F1_L0R1_7[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][7].Set(9, F1_L0R1_7, temprot, tempstate);
	//F1_L0R1_8
	MechanicalStep F1_L0R1_8[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][8].Set(11, F1_L0R1_8, temprot, tempstate);
	//F1_L0R1_9
	MechanicalStep F1_L0R1_9[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][9].Set(11, F1_L0R1_9, temprot, tempstate);
	//F1_L0R1_10
	MechanicalStep F1_L0R1_10[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][10].Set(11, F1_L0R1_10, temprot, tempstate);
	//F1_L0R1_11
	MechanicalStep F1_L0R1_11[] = { M_LO, M_R1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][11].Set(12, F1_L0R1_11, temprot, tempstate);
	//F1_L0R1_12
	MechanicalStep F1_L0R1_12[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][12].Set(12, F1_L0R1_12, temprot, tempstate);
	//F1_L0R1_13
	MechanicalStep F1_L0R1_13[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][13].Set(12, F1_L0R1_13, temprot, tempstate);
	//F1_L0R1_14
	MechanicalStep F1_L0R1_14[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][14].Set(19, F1_L0R1_14, temprot, tempstate);
	//F1_L0R1_15
	MechanicalStep F1_L0R1_15[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_1][L_0_R_1][15].Set(19, F1_L0R1_15, temprot, tempstate);
}
void F2_L0R1int(void)
{
	//F2_L0R1_0
	MechanicalStep F2_L0R1_0[] = { M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][0].Set(4, F2_L0R1_0, temprot, tempstate);
	//F2_L0R1_1
	MechanicalStep F2_L0R1_1[] = { M_RO, M_R1, M_L2, M_RC, M_L2, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][1].Set(5, F2_L0R1_1, temprot, tempstate);
	//F2_L0R1_2
	MechanicalStep F2_L0R1_2[] = { M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][2].Set(5, F2_L0R1_2, temprot, tempstate);
	//F2_L0R1_3
	MechanicalStep F2_L0R1_3[] = { M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][3].Set(5, F2_L0R1_3, temprot, tempstate);
	//F2_L0R1_4
	MechanicalStep F2_L0R1_4[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][4].Set(8, F2_L0R1_4, temprot, tempstate);
	//F2_L0R1_5
	MechanicalStep F2_L0R1_5[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][5].Set(8, F2_L0R1_5, temprot, tempstate);
	//F2_L0R1_6
	MechanicalStep F2_L0R1_6[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][6].Set(9, F2_L0R1_6, temprot, tempstate);
	//F2_L0R1_7
	MechanicalStep F2_L0R1_7[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][7].Set(9, F2_L0R1_7, temprot, tempstate);
	//F2_L0R1_8
	MechanicalStep F2_L0R1_8[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][8].Set(11, F2_L0R1_8, temprot, tempstate);
	//F2_L0R1_9
	MechanicalStep F2_L0R1_9[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][9].Set(11, F2_L0R1_9, temprot, tempstate);
	//F2_L0R1_10
	MechanicalStep F2_L0R1_10[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][10].Set(11, F2_L0R1_10, temprot, tempstate);
	//F2_L0R1_11
	MechanicalStep F2_L0R1_11[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][11].Set(12, F2_L0R1_11, temprot, tempstate);
	//F2_L0R1_12
	MechanicalStep F2_L0R1_12[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][12].Set(12, F2_L0R1_12, temprot, tempstate);
	//F2_L0R1_13
	MechanicalStep F2_L0R1_13[] = { M_LO, M_R1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][13].Set(12, F2_L0R1_13, temprot, tempstate);
	//F2_L0R1_14
	MechanicalStep F2_L0R1_14[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][14].Set(19, F2_L0R1_14, temprot, tempstate);
	//F2_L0R1_15
	MechanicalStep F2_L0R1_15[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_0_R_1][15].Set(19, F2_L0R1_15, temprot, tempstate);
}
void F3_L0R1int(void)
{
	//F3_L0R1_0
	MechanicalStep F3_L0R1_0[] = { M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][0].Set(4, F3_L0R1_0, temprot, tempstate);
	//F3_L0R1_1
	MechanicalStep F3_L0R1_1[] = { M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][1].Set(5, F3_L0R1_1, temprot, tempstate);
	//F3_L0R1_2
	MechanicalStep F3_L0R1_2[] = { M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][2].Set(5, F3_L0R1_2, temprot, tempstate);
	//F3_L0R1_3
	MechanicalStep F3_L0R1_3[] = { M_RO, M_R1, M_L2, M_RC, M_L3, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][3].Set(5, F3_L0R1_3, temprot, tempstate);
	//F3_L0R1_4
	MechanicalStep F3_L0R1_4[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][4].Set(8, F3_L0R1_4, temprot, tempstate);
	//F3_L0R1_5
	MechanicalStep F3_L0R1_5[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][5].Set(8, F3_L0R1_5, temprot, tempstate);
	//F3_L0R1_6
	MechanicalStep F3_L0R1_6[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][6].Set(9, F3_L0R1_6, temprot, tempstate);
	//F3_L0R1_7
	MechanicalStep F3_L0R1_7[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][7].Set(9, F3_L0R1_7, temprot, tempstate);
	//F3_L0R1_8
	MechanicalStep F3_L0R1_8[] = { M_LO, M_R1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][8].Set(11, F3_L0R1_8, temprot, tempstate);
	//F3_L0R1_9
	MechanicalStep F3_L0R1_9[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][9].Set(11, F3_L0R1_9, temprot, tempstate);
	//F3_L0R1_10
	MechanicalStep F3_L0R1_10[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][10].Set(11, F3_L0R1_10, temprot, tempstate);
	//F3_L0R1_11
	MechanicalStep F3_L0R1_11[] = { M_LO, M_R1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][11].Set(12, F3_L0R1_11, temprot, tempstate);
	//F3_L0R1_12
	MechanicalStep F3_L0R1_12[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][12].Set(12, F3_L0R1_12, temprot, tempstate);
	//F3_L0R1_13
	MechanicalStep F3_L0R1_13[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][13].Set(12, F3_L0R1_13, temprot, tempstate);
	//F3_L0R1_14
	MechanicalStep F3_L0R1_14[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][14].Set(19, F3_L0R1_14, temprot, tempstate);
	//F3_L0R1_15
	MechanicalStep F3_L0R1_15[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_3][L_0_R_1][15].Set(19, F3_L0R1_15, temprot, tempstate);
}
void R1_L0R1int(void)
{
	//R1_L0R1_0
	MechanicalStep R1_L0R1_0[] = { M_R1, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][0].Set(1, R1_L0R1_0, temprot, tempstate);
	//R1_L0R1_1
	MechanicalStep R1_L0R1_1[] = { M_LO, M_R2, M_LC, M_R1, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][1].Set(4, R1_L0R1_1, temprot, tempstate);
	//R1_L0R1_2
	MechanicalStep R1_L0R1_2[] = { M_RO, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][2].Set(4, R1_L0R1_2, temprot, tempstate);
	//R1_L0R1_3
	MechanicalStep R1_L0R1_3[] = { M_LO, M_R3, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][3].Set(4, R1_L0R1_3, temprot, tempstate);
	//R1_L0R1_4
	MechanicalStep R1_L0R1_4[] = { M_LO, M_R1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][4].Set(4, R1_L0R1_4, temprot, tempstate);
	//R1_L0R1_5
	MechanicalStep R1_L0R1_5[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][5].Set(12, R1_L0R1_5, temprot, tempstate);
	//R1_L0R1_6
	MechanicalStep R1_L0R1_6[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][6].Set(12, R1_L0R1_6, temprot, tempstate);
	//R1_L0R1_7
	MechanicalStep R1_L0R1_7[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][7].Set(12, R1_L0R1_7, temprot, tempstate);
	//R1_L0R1_8
	MechanicalStep R1_L0R1_8[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][8].Set(12, R1_L0R1_8, temprot, tempstate);
	//R1_L0R1_9
	MechanicalStep R1_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][9].Set(13, R1_L0R1_9, temprot, tempstate);
	//R1_L0R1_10
	MechanicalStep R1_L0R1_10[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][10].Set(13, R1_L0R1_10, temprot, tempstate);
	//R1_L0R1_11
	MechanicalStep R1_L0R1_11[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][11].Set(13, R1_L0R1_11, temprot, tempstate);
	//R1_L0R1_12
	MechanicalStep R1_L0R1_12[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][12].Set(13, R1_L0R1_12, temprot, tempstate);
	//R1_L0R1_13
	MechanicalStep R1_L0R1_13[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][13].Set(13, R1_L0R1_13, temprot, tempstate);
	//R1_L0R1_14
	MechanicalStep R1_L0R1_14[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][14].Set(15, R1_L0R1_14, temprot, tempstate);
	//R1_L0R1_15
	MechanicalStep R1_L0R1_15[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_0_R_1][15].Set(15, R1_L0R1_15, temprot, tempstate);
}
void R2_L0R1int(void)
{
	//R2_L0R1_0
	MechanicalStep R2_L0R1_0[] = { M_R2, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][0].Set(1, R2_L0R1_0, temprot, tempstate);
	//R2_L0R1_1
	MechanicalStep R2_L0R1_1[] = { M_LO, M_R3, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][1].Set(4, R2_L0R1_1, temprot, tempstate);
	//R2_L0R1_2
	MechanicalStep R2_L0R1_2[] = { M_RO, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][2].Set(4, R2_L0R1_2, temprot, tempstate);
	//R2_L0R1_3
	MechanicalStep R2_L0R1_3[] = { M_LO, M_R1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][3].Set(4, R2_L0R1_3, temprot, tempstate);
	//R2_L0R1_4
	MechanicalStep R2_L0R1_4[] = { M_LO, M_R2, M_LC, M_R2, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][4].Set(4, R2_L0R1_4, temprot, tempstate);
	//R2_L0R1_5
	MechanicalStep R2_L0R1_5[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][5].Set(12, R2_L0R1_5, temprot, tempstate);
	//R2_L0R1_6
	MechanicalStep R2_L0R1_6[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][6].Set(12, R2_L0R1_6, temprot, tempstate);
	//R2_L0R1_7
	MechanicalStep R2_L0R1_7[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][7].Set(12, R2_L0R1_7, temprot, tempstate);
	//R2_L0R1_8
	MechanicalStep R2_L0R1_8[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][8].Set(12, R2_L0R1_8, temprot, tempstate);
	//R2_L0R1_9
	MechanicalStep R2_L0R1_9[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][9].Set(13, R2_L0R1_9, temprot, tempstate);
	//R2_L0R1_10
	MechanicalStep R2_L0R1_10[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][10].Set(13, R2_L0R1_10, temprot, tempstate);
	//R2_L0R1_11
	MechanicalStep R2_L0R1_11[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][11].Set(13, R2_L0R1_11, temprot, tempstate);
	//R2_L0R1_12
	MechanicalStep R2_L0R1_12[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][12].Set(13, R2_L0R1_12, temprot, tempstate);
	//R2_L0R1_13
	MechanicalStep R2_L0R1_13[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][13].Set(13, R2_L0R1_13, temprot, tempstate);
	//R2_L0R1_14
	MechanicalStep R2_L0R1_14[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][14].Set(15, R2_L0R1_14, temprot, tempstate);
	//R2_L0R1_15
	MechanicalStep R2_L0R1_15[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_2][L_0_R_1][15].Set(15, R2_L0R1_15, temprot, tempstate);
}
void R3_L0R1int(void)
{
	//R3_L0R1_0
	MechanicalStep R3_L0R1_0[] = { M_R3, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][0].Set(1, R3_L0R1_0, temprot, tempstate);
	//R3_L0R1_1
	MechanicalStep R3_L0R1_1[] = { M_LO, M_R2, M_LC, M_R3, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][1].Set(4, R3_L0R1_1, temprot, tempstate);
	//R3_L0R1_2
	MechanicalStep R3_L0R1_2[] = { M_RO, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][2].Set(4, R3_L0R1_2, temprot, tempstate);
	//R3_L0R1_3
	MechanicalStep R3_L0R1_3[] = { M_LO, M_R3, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][3].Set(4, R3_L0R1_3, temprot, tempstate);
	//R3_L0R1_4
	MechanicalStep R3_L0R1_4[] = { M_LO, M_R1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][4].Set(4, R3_L0R1_4, temprot, tempstate);
	//R3_L0R1_5
	MechanicalStep R3_L0R1_5[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][5].Set(12, R3_L0R1_5, temprot, tempstate);
	//R3_L0R1_6
	MechanicalStep R3_L0R1_6[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][6].Set(12, R3_L0R1_6, temprot, tempstate);
	//R3_L0R1_7
	MechanicalStep R3_L0R1_7[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][7].Set(12, R3_L0R1_7, temprot, tempstate);
	//R3_L0R1_8
	MechanicalStep R3_L0R1_8[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][8].Set(12, R3_L0R1_8, temprot, tempstate);
	//R3_L0R1_9
	MechanicalStep R3_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][9].Set(13, R3_L0R1_9, temprot, tempstate);
	//R3_L0R1_10
	MechanicalStep R3_L0R1_10[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][10].Set(13, R3_L0R1_10, temprot, tempstate);
	//R3_L0R1_11
	MechanicalStep R3_L0R1_11[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R3, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][11].Set(13, R3_L0R1_11, temprot, tempstate);
	//R3_L0R1_12
	MechanicalStep R3_L0R1_12[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R3, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][12].Set(13, R3_L0R1_12, temprot, tempstate);
	//R3_L0R1_13
	MechanicalStep R3_L0R1_13[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][13].Set(13, R3_L0R1_13, temprot, tempstate);
	//R3_L0R1_14
	MechanicalStep R3_L0R1_14[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][14].Set(15, R3_L0R1_14, temprot, tempstate);
	//R3_L0R1_15
	MechanicalStep R3_L0R1_15[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_0_R_1][15].Set(15, R3_L0R1_15, temprot, tempstate);
}
void U1_L0R1int(void)
{
	//U1_L0R1_0
	MechanicalStep U1_L0R1_0[] = { M_LO, M_R3, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][0].Set(4, U1_L0R1_0, temprot, tempstate);
	//U1_L0R1_1
	MechanicalStep U1_L0R1_1[] = { M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][1].Set(5, U1_L0R1_1, temprot, tempstate);
	//U1_L0R1_2
	MechanicalStep U1_L0R1_2[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][2].Set(8, U1_L0R1_2, temprot, tempstate);
	//U1_L0R1_3
	MechanicalStep U1_L0R1_3[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][3].Set(9, U1_L0R1_3, temprot, tempstate);
	//U1_L0R1_4
	MechanicalStep U1_L0R1_4[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][4].Set(9, U1_L0R1_4, temprot, tempstate);
	//U1_L0R1_5
	MechanicalStep U1_L0R1_5[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_R1, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][5].Set(9, U1_L0R1_5, temprot, tempstate);
	//U1_L0R1_6
	MechanicalStep U1_L0R1_6[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][6].Set(9, U1_L0R1_6, temprot, tempstate);
	//U1_L0R1_7
	MechanicalStep U1_L0R1_7[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][7].Set(10, U1_L0R1_7, temprot, tempstate);
	//U1_L0R1_8
	MechanicalStep U1_L0R1_8[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][8].Set(11, U1_L0R1_8, temprot, tempstate);
	//U1_L0R1_9
	MechanicalStep U1_L0R1_9[] = { M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][9].Set(11, U1_L0R1_9, temprot, tempstate);
	//U1_L0R1_10
	MechanicalStep U1_L0R1_10[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][10].Set(11, U1_L0R1_10, temprot, tempstate);
	//U1_L0R1_11
	MechanicalStep U1_L0R1_11[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][11].Set(12, U1_L0R1_11, temprot, tempstate);
	//U1_L0R1_12
	MechanicalStep U1_L0R1_12[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][12].Set(13, U1_L0R1_12, temprot, tempstate);
	//U1_L0R1_13
	MechanicalStep U1_L0R1_13[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][13].Set(13, U1_L0R1_13, temprot, tempstate);
	//U1_L0R1_14
	MechanicalStep U1_L0R1_14[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][14].Set(15, U1_L0R1_14, temprot, tempstate);
	//U1_L0R1_15
	MechanicalStep U1_L0R1_15[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_1][L_0_R_1][15].Set(15, U1_L0R1_15, temprot, tempstate);
}
void U2_L0R1int(void)
{
	//U2_L0R1_0
	MechanicalStep U2_L0R1_0[] = { M_LO, M_R3, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][0].Set(4, U2_L0R1_0, temprot, tempstate);
	//U2_L0R1_1
	MechanicalStep U2_L0R1_1[] = { M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][1].Set(5, U2_L0R1_1, temprot, tempstate);
	//U2_L0R1_2
	MechanicalStep U2_L0R1_2[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][2].Set(8, U2_L0R1_2, temprot, tempstate);
	//U2_L0R1_3
	MechanicalStep U2_L0R1_3[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][3].Set(9, U2_L0R1_3, temprot, tempstate);
	//U2_L0R1_4
	MechanicalStep U2_L0R1_4[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_R2, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][4].Set(9, U2_L0R1_4, temprot, tempstate);
	//U2_L0R1_5
	MechanicalStep U2_L0R1_5[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][5].Set(9, U2_L0R1_5, temprot, tempstate);
	//U2_L0R1_6
	MechanicalStep U2_L0R1_6[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][6].Set(9, U2_L0R1_6, temprot, tempstate);
	//U2_L0R1_7
	MechanicalStep U2_L0R1_7[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][7].Set(10, U2_L0R1_7, temprot, tempstate);
	//U2_L0R1_8
	MechanicalStep U2_L0R1_8[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][8].Set(11, U2_L0R1_8, temprot, tempstate);
	//U2_L0R1_9
	MechanicalStep U2_L0R1_9[] = { M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][9].Set(11, U2_L0R1_9, temprot, tempstate);
	//U2_L0R1_10
	MechanicalStep U2_L0R1_10[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][10].Set(11, U2_L0R1_10, temprot, tempstate);
	//U2_L0R1_11
	MechanicalStep U2_L0R1_11[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][11].Set(12, U2_L0R1_11, temprot, tempstate);
	//U2_L0R1_12
	MechanicalStep U2_L0R1_12[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][12].Set(13, U2_L0R1_12, temprot, tempstate);
	//U2_L0R1_13
	MechanicalStep U2_L0R1_13[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][13].Set(13, U2_L0R1_13, temprot, tempstate);
	//U2_L0R1_14
	MechanicalStep U2_L0R1_14[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][14].Set(15, U2_L0R1_14, temprot, tempstate);
	//U2_L0R1_15
	MechanicalStep U2_L0R1_15[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_0_R_1][15].Set(15, U2_L0R1_15, temprot, tempstate);
}
void U3_L0R1int(void)
{
	//U3_L0R1_0
	MechanicalStep U3_L0R1_0[] = { M_LO, M_R3, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][0].Set(4, U3_L0R1_0, temprot, tempstate);
	//U3_L0R1_1
	MechanicalStep U3_L0R1_1[] = { M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][1].Set(5, U3_L0R1_1, temprot, tempstate);
	//U3_L0R1_2
	MechanicalStep U3_L0R1_2[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][2].Set(8, U3_L0R1_2, temprot, tempstate);
	//U3_L0R1_3
	MechanicalStep U3_L0R1_3[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][3].Set(9, U3_L0R1_3, temprot, tempstate);
	//U3_L0R1_4
	MechanicalStep U3_L0R1_4[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][4].Set(9, U3_L0R1_4, temprot, tempstate);
	//U3_L0R1_5
	MechanicalStep U3_L0R1_5[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_R3, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][5].Set(9, U3_L0R1_5, temprot, tempstate);
	//U3_L0R1_6
	MechanicalStep U3_L0R1_6[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][6].Set(9, U3_L0R1_6, temprot, tempstate);
	//U3_L0R1_7
	MechanicalStep U3_L0R1_7[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][7].Set(10, U3_L0R1_7, temprot, tempstate);
	//U3_L0R1_8
	MechanicalStep U3_L0R1_8[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][8].Set(11, U3_L0R1_8, temprot, tempstate);
	//U3_L0R1_9
	MechanicalStep U3_L0R1_9[] = { M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][9].Set(11, U3_L0R1_9, temprot, tempstate);
	//U3_L0R1_10
	MechanicalStep U3_L0R1_10[] = { M_RO, M_R1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][10].Set(11, U3_L0R1_10, temprot, tempstate);
	//U3_L0R1_11
	MechanicalStep U3_L0R1_11[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][11].Set(12, U3_L0R1_11, temprot, tempstate);
	//U3_L0R1_12
	MechanicalStep U3_L0R1_12[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][12].Set(13, U3_L0R1_12, temprot, tempstate);
	//U3_L0R1_13
	MechanicalStep U3_L0R1_13[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][13].Set(13, U3_L0R1_13, temprot, tempstate);
	//U3_L0R1_14
	MechanicalStep U3_L0R1_14[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][14].Set(15, U3_L0R1_14, temprot, tempstate);
	//U3_L0R1_15
	MechanicalStep U3_L0R1_15[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_3][L_0_R_1][15].Set(15, U3_L0R1_15, temprot, tempstate);
}
void B1_L0R1int(void)
{
	//B1_L0R1_0
	MechanicalStep B1_L0R1_0[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][0].Set(7, B1_L0R1_0, temprot, tempstate);
	//B1_L0R1_1
	MechanicalStep B1_L0R1_1[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][1].Set(8, B1_L0R1_1, temprot, tempstate);
	//B1_L0R1_2
	MechanicalStep B1_L0R1_2[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][2].Set(8, B1_L0R1_2, temprot, tempstate);
	//B1_L0R1_3
	MechanicalStep B1_L0R1_3[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][3].Set(8, B1_L0R1_3, temprot, tempstate);
	//B1_L0R1_4
	MechanicalStep B1_L0R1_4[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][4].Set(8, B1_L0R1_4, temprot, tempstate);
	//B1_L0R1_5
	MechanicalStep B1_L0R1_5[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][5].Set(8, B1_L0R1_5, temprot, tempstate);
	//B1_L0R1_6
	MechanicalStep B1_L0R1_6[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_RC, M_L1, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][6].Set(8, B1_L0R1_6, temprot, tempstate);
	//B1_L0R1_7
	MechanicalStep B1_L0R1_7[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][7].Set(9, B1_L0R1_7, temprot, tempstate);
	//B1_L0R1_8
	MechanicalStep B1_L0R1_8[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R2, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][8].Set(9, B1_L0R1_8, temprot, tempstate);
	//B1_L0R1_9
	MechanicalStep B1_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][9].Set(9, B1_L0R1_9, temprot, tempstate);
	//B1_L0R1_10
	MechanicalStep B1_L0R1_10[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][10].Set(9, B1_L0R1_10, temprot, tempstate);
	//B1_L0R1_11
	MechanicalStep B1_L0R1_11[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][11].Set(9, B1_L0R1_11, temprot, tempstate);
	//B1_L0R1_12
	MechanicalStep B1_L0R1_12[] = { M_LO, M_R1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][12].Set(11, B1_L0R1_12, temprot, tempstate);
	//B1_L0R1_13
	MechanicalStep B1_L0R1_13[] = { M_LO, M_R1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][13].Set(11, B1_L0R1_13, temprot, tempstate);
	//B1_L0R1_14
	MechanicalStep B1_L0R1_14[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][14].Set(16, B1_L0R1_14, temprot, tempstate);
	//B1_L0R1_15
	MechanicalStep B1_L0R1_15[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_1][L_0_R_1][15].Set(16, B1_L0R1_15, temprot, tempstate);
}
void B2_L0R1int(void)
{
	//B2_L0R1_0
	MechanicalStep B2_L0R1_0[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][0].Set(7, B2_L0R1_0, temprot, tempstate);
	//B2_L0R1_1
	MechanicalStep B2_L0R1_1[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][1].Set(8, B2_L0R1_1, temprot, tempstate);
	//B2_L0R1_2
	MechanicalStep B2_L0R1_2[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][2].Set(8, B2_L0R1_2, temprot, tempstate);
	//B2_L0R1_3
	MechanicalStep B2_L0R1_3[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_RC, M_L2, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][3].Set(8, B2_L0R1_3, temprot, tempstate);
	//B2_L0R1_4
	MechanicalStep B2_L0R1_4[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][4].Set(8, B2_L0R1_4, temprot, tempstate);
	//B2_L0R1_5
	MechanicalStep B2_L0R1_5[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][5].Set(8, B2_L0R1_5, temprot, tempstate);
	//B2_L0R1_6
	MechanicalStep B2_L0R1_6[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][6].Set(8, B2_L0R1_6, temprot, tempstate);
	//B2_L0R1_7
	MechanicalStep B2_L0R1_7[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][7].Set(9, B2_L0R1_7, temprot, tempstate);
	//B2_L0R1_8
	MechanicalStep B2_L0R1_8[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][8].Set(9, B2_L0R1_8, temprot, tempstate);
	//B2_L0R1_9
	MechanicalStep B2_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][9].Set(9, B2_L0R1_9, temprot, tempstate);
	//B2_L0R1_10
	MechanicalStep B2_L0R1_10[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][10].Set(9, B2_L0R1_10, temprot, tempstate);
	//B2_L0R1_11
	MechanicalStep B2_L0R1_11[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R2, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][11].Set(9, B2_L0R1_11, temprot, tempstate);
	//B2_L0R1_12
	MechanicalStep B2_L0R1_12[] = { M_LO, M_R1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][12].Set(11, B2_L0R1_12, temprot, tempstate);
	//B2_L0R1_13
	MechanicalStep B2_L0R1_13[] = { M_LO, M_R1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][13].Set(11, B2_L0R1_13, temprot, tempstate);
	//B2_L0R1_14
	MechanicalStep B2_L0R1_14[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][14].Set(16, B2_L0R1_14, temprot, tempstate);
	//B2_L0R1_15
	MechanicalStep B2_L0R1_15[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_0_R_1][15].Set(16, B2_L0R1_15, temprot, tempstate);
}
void B3_L0R1int(void)
{
	//B3_L0R1_0
	MechanicalStep B3_L0R1_0[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][0].Set(7, B3_L0R1_0, temprot, tempstate);
	//B3_L0R1_1
	MechanicalStep B3_L0R1_1[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][1].Set(8, B3_L0R1_1, temprot, tempstate);
	//B3_L0R1_2
	MechanicalStep B3_L0R1_2[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][2].Set(8, B3_L0R1_2, temprot, tempstate);
	//B3_L0R1_3
	MechanicalStep B3_L0R1_3[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][3].Set(8, B3_L0R1_3, temprot, tempstate);
	//B3_L0R1_4
	MechanicalStep B3_L0R1_4[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][4].Set(8, B3_L0R1_4, temprot, tempstate);
	//B3_L0R1_5
	MechanicalStep B3_L0R1_5[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][5].Set(8, B3_L0R1_5, temprot, tempstate);
	//B3_L0R1_6
	MechanicalStep B3_L0R1_6[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_RC, M_L3, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][6].Set(8, B3_L0R1_6, temprot, tempstate);
	//B3_L0R1_7
	MechanicalStep B3_L0R1_7[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][7].Set(9, B3_L0R1_7, temprot, tempstate);
	//B3_L0R1_8
	MechanicalStep B3_L0R1_8[] = { M_RO, M_R1, M_L2, M_RC, M_LO, M_R2, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][8].Set(9, B3_L0R1_8, temprot, tempstate);
	//B3_L0R1_9
	MechanicalStep B3_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][9].Set(9, B3_L0R1_9, temprot, tempstate);
	//B3_L0R1_10
	MechanicalStep B3_L0R1_10[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][10].Set(9, B3_L0R1_10, temprot, tempstate);
	//B3_L0R1_11
	MechanicalStep B3_L0R1_11[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][11].Set(9, B3_L0R1_11, temprot, tempstate);
	//B3_L0R1_12
	MechanicalStep B3_L0R1_12[] = { M_LO, M_R1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][12].Set(11, B3_L0R1_12, temprot, tempstate);
	//B3_L0R1_13
	MechanicalStep B3_L0R1_13[] = { M_LO, M_R1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][13].Set(11, B3_L0R1_13, temprot, tempstate);
	//B3_L0R1_14
	MechanicalStep B3_L0R1_14[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][14].Set(16, B3_L0R1_14, temprot, tempstate);
	//B3_L0R1_15
	MechanicalStep B3_L0R1_15[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_3][L_0_R_1][15].Set(16, B3_L0R1_15, temprot, tempstate);
}
void L1_L0R1int(void)
{
	//L1_L0R1_0
	MechanicalStep L1_L0R1_0[] = { M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][0].Set(5, L1_L0R1_0, temprot, tempstate);
	//L1_L0R1_1
	MechanicalStep L1_L0R1_1[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][1].Set(6, L1_L0R1_1, temprot, tempstate);
	//L1_L0R1_2
	MechanicalStep L1_L0R1_2[] = { M_LO, M_R1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][2].Set(8, L1_L0R1_2, temprot, tempstate);
	//L1_L0R1_3
	MechanicalStep L1_L0R1_3[] = { M_LO, M_R3, M_LC, M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][3].Set(8, L1_L0R1_3, temprot, tempstate);
	//L1_L0R1_4
	MechanicalStep L1_L0R1_4[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][4].Set(8, L1_L0R1_4, temprot, tempstate);
	//L1_L0R1_5
	MechanicalStep L1_L0R1_5[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][5].Set(9, L1_L0R1_5, temprot, tempstate);
	//L1_L0R1_6
	MechanicalStep L1_L0R1_6[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][6].Set(11, L1_L0R1_6, temprot, tempstate);
	//L1_L0R1_7
	MechanicalStep L1_L0R1_7[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][7].Set(11, L1_L0R1_7, temprot, tempstate);
	//L1_L0R1_8
	MechanicalStep L1_L0R1_8[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][8].Set(12, L1_L0R1_8, temprot, tempstate);
	//L1_L0R1_9
	MechanicalStep L1_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][9].Set(12, L1_L0R1_9, temprot, tempstate);
	//L1_L0R1_10
	MechanicalStep L1_L0R1_10[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][10].Set(12, L1_L0R1_10, temprot, tempstate);
	//L1_L0R1_11
	MechanicalStep L1_L0R1_11[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][11].Set(12, L1_L0R1_11, temprot, tempstate);
	//L1_L0R1_12
	MechanicalStep L1_L0R1_12[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][12].Set(13, L1_L0R1_12, temprot, tempstate);
	//L1_L0R1_13
	MechanicalStep L1_L0R1_13[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][13].Set(13, L1_L0R1_13, temprot, tempstate);
	//L1_L0R1_14
	MechanicalStep L1_L0R1_14[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][14].Set(15, L1_L0R1_14, temprot, tempstate);
	//L1_L0R1_15
	MechanicalStep L1_L0R1_15[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_0_R_1][15].Set(15, L1_L0R1_15, temprot, tempstate);
}
void L2_L0R1int(void)
{
	//L2_L0R1_0
	MechanicalStep L2_L0R1_0[] = { M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][0].Set(5, L2_L0R1_0, temprot, tempstate);
	//L2_L0R1_1
	MechanicalStep L2_L0R1_1[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][1].Set(6, L2_L0R1_1, temprot, tempstate);
	//L2_L0R1_2
	MechanicalStep L2_L0R1_2[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][2].Set(8, L2_L0R1_2, temprot, tempstate);
	//L2_L0R1_3
	MechanicalStep L2_L0R1_3[] = { M_LO, M_R1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][3].Set(8, L2_L0R1_3, temprot, tempstate);
	//L2_L0R1_4
	MechanicalStep L2_L0R1_4[] = { M_LO, M_R3, M_LC, M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][4].Set(8, L2_L0R1_4, temprot, tempstate);
	//L2_L0R1_5
	MechanicalStep L2_L0R1_5[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][5].Set(9, L2_L0R1_5, temprot, tempstate);
	//L2_L0R1_6
	MechanicalStep L2_L0R1_6[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][6].Set(11, L2_L0R1_6, temprot, tempstate);
	//L2_L0R1_7
	MechanicalStep L2_L0R1_7[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][7].Set(11, L2_L0R1_7, temprot, tempstate);
	//L2_L0R1_8
	MechanicalStep L2_L0R1_8[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][8].Set(12, L2_L0R1_8, temprot, tempstate);
	//L2_L0R1_9
	MechanicalStep L2_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][9].Set(12, L2_L0R1_9, temprot, tempstate);
	//L2_L0R1_10
	MechanicalStep L2_L0R1_10[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][10].Set(12, L2_L0R1_10, temprot, tempstate);
	//L2_L0R1_11
	MechanicalStep L2_L0R1_11[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][11].Set(12, L2_L0R1_11, temprot, tempstate);
	//L2_L0R1_12
	MechanicalStep L2_L0R1_12[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][12].Set(13, L2_L0R1_12, temprot, tempstate);
	//L2_L0R1_13
	MechanicalStep L2_L0R1_13[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][13].Set(13, L2_L0R1_13, temprot, tempstate);
	//L2_L0R1_14
	MechanicalStep L2_L0R1_14[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][14].Set(15, L2_L0R1_14, temprot, tempstate);
	//L2_L0R1_15
	MechanicalStep L2_L0R1_15[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_2][L_0_R_1][15].Set(15, L2_L0R1_15, temprot, tempstate);
}
void L3_L0R1int(void)
{
	//L3_L0R1_0
	MechanicalStep L3_L0R1_0[] = { M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][0].Set(5, L3_L0R1_0, temprot, tempstate);
	//L3_L0R1_1
	MechanicalStep L3_L0R1_1[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][1].Set(6, L3_L0R1_1, temprot, tempstate);
	//L3_L0R1_2
	MechanicalStep L3_L0R1_2[] = { M_LO, M_R1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R3, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][2].Set(8, L3_L0R1_2, temprot, tempstate);
	//L3_L0R1_3
	MechanicalStep L3_L0R1_3[] = { M_LO, M_R3, M_LC, M_RO, M_L2, M_R1, M_RC, M_R3, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][3].Set(8, L3_L0R1_3, temprot, tempstate);
	//L3_L0R1_4
	MechanicalStep L3_L0R1_4[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][4].Set(8, L3_L0R1_4, temprot, tempstate);
	//L3_L0R1_5
	MechanicalStep L3_L0R1_5[] = { M_LO, M_R2, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][5].Set(9, L3_L0R1_5, temprot, tempstate);
	//L3_L0R1_6
	MechanicalStep L3_L0R1_6[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][6].Set(11, L3_L0R1_6, temprot, tempstate);
	//L3_L0R1_7
	MechanicalStep L3_L0R1_7[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][7].Set(11, L3_L0R1_7, temprot, tempstate);
	//L3_L0R1_8
	MechanicalStep L3_L0R1_8[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][8].Set(12, L3_L0R1_8, temprot, tempstate);
	//L3_L0R1_9
	MechanicalStep L3_L0R1_9[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][9].Set(12, L3_L0R1_9, temprot, tempstate);
	//L3_L0R1_10
	MechanicalStep L3_L0R1_10[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][10].Set(12, L3_L0R1_10, temprot, tempstate);
	//L3_L0R1_11
	MechanicalStep L3_L0R1_11[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][11].Set(12, L3_L0R1_11, temprot, tempstate);
	//L3_L0R1_12
	MechanicalStep L3_L0R1_12[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][12].Set(13, L3_L0R1_12, temprot, tempstate);
	//L3_L0R1_13
	MechanicalStep L3_L0R1_13[] = { M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][13].Set(13, L3_L0R1_13, temprot, tempstate);
	//L3_L0R1_14
	MechanicalStep L3_L0R1_14[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][14].Set(15, L3_L0R1_14, temprot, tempstate);
	//L3_L0R1_15
	MechanicalStep L3_L0R1_15[] = { M_LO, M_R1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_0_R_1][15].Set(15, L3_L0R1_15, temprot, tempstate);
}
void D1_L0R1int(void)
{
	//D1_L0R1_0
	MechanicalStep D1_L0R1_0[] = { M_LO, M_R1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][0].Set(4, D1_L0R1_0, temprot, tempstate);
	//D1_L0R1_1
	MechanicalStep D1_L0R1_1[] = { M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][1].Set(5, D1_L0R1_1, temprot, tempstate);
	//D1_L0R1_2
	MechanicalStep D1_L0R1_2[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][2].Set(8, D1_L0R1_2, temprot, tempstate);
	//D1_L0R1_3
	MechanicalStep D1_L0R1_3[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][3].Set(9, D1_L0R1_3, temprot, tempstate);
	//D1_L0R1_4
	MechanicalStep D1_L0R1_4[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][4].Set(9, D1_L0R1_4, temprot, tempstate);
	//D1_L0R1_5
	MechanicalStep D1_L0R1_5[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_R1, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][5].Set(9, D1_L0R1_5, temprot, tempstate);
	//D1_L0R1_6
	MechanicalStep D1_L0R1_6[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][6].Set(9, D1_L0R1_6, temprot, tempstate);
	//D1_L0R1_7
	MechanicalStep D1_L0R1_7[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][7].Set(10, D1_L0R1_7, temprot, tempstate);
	//D1_L0R1_8
	MechanicalStep D1_L0R1_8[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][8].Set(11, D1_L0R1_8, temprot, tempstate);
	//D1_L0R1_9
	MechanicalStep D1_L0R1_9[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][9].Set(11, D1_L0R1_9, temprot, tempstate);
	//D1_L0R1_10
	MechanicalStep D1_L0R1_10[] = { M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][10].Set(11, D1_L0R1_10, temprot, tempstate);
	//D1_L0R1_11
	MechanicalStep D1_L0R1_11[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][11].Set(12, D1_L0R1_11, temprot, tempstate);
	//D1_L0R1_12
	MechanicalStep D1_L0R1_12[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][12].Set(13, D1_L0R1_12, temprot, tempstate);
	//D1_L0R1_13
	MechanicalStep D1_L0R1_13[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][13].Set(13, D1_L0R1_13, temprot, tempstate);
	//D1_L0R1_14
	MechanicalStep D1_L0R1_14[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][14].Set(15, D1_L0R1_14, temprot, tempstate);
	//D1_L0R1_15
	MechanicalStep D1_L0R1_15[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_1][L_0_R_1][15].Set(15, D1_L0R1_15, temprot, tempstate);
}
void D2_L0R1int(void)
{
	//D2_L0R1_0
	MechanicalStep D2_L0R1_0[] = { M_LO, M_R1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][0].Set(4, D2_L0R1_0, temprot, tempstate);
	//D2_L0R1_1
	MechanicalStep D2_L0R1_1[] = { M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][1].Set(5, D2_L0R1_1, temprot, tempstate);
	//D2_L0R1_2
	MechanicalStep D2_L0R1_2[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][2].Set(8, D2_L0R1_2, temprot, tempstate);
	//D2_L0R1_3
	MechanicalStep D2_L0R1_3[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_R2, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][3].Set(9, D2_L0R1_3, temprot, tempstate);
	//D2_L0R1_4
	MechanicalStep D2_L0R1_4[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][4].Set(9, D2_L0R1_4, temprot, tempstate);
	//D2_L0R1_5
	MechanicalStep D2_L0R1_5[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][5].Set(9, D2_L0R1_5, temprot, tempstate);
	//D2_L0R1_6
	MechanicalStep D2_L0R1_6[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][6].Set(9, D2_L0R1_6, temprot, tempstate);
	//D2_L0R1_7
	MechanicalStep D2_L0R1_7[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][7].Set(10, D2_L0R1_7, temprot, tempstate);
	//D2_L0R1_8
	MechanicalStep D2_L0R1_8[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][8].Set(11, D2_L0R1_8, temprot, tempstate);
	//D2_L0R1_9
	MechanicalStep D2_L0R1_9[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][9].Set(11, D2_L0R1_9, temprot, tempstate);
	//D2_L0R1_10
	MechanicalStep D2_L0R1_10[] = { M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][10].Set(11, D2_L0R1_10, temprot, tempstate);
	//D2_L0R1_11
	MechanicalStep D2_L0R1_11[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][11].Set(12, D2_L0R1_11, temprot, tempstate);
	//D2_L0R1_12
	MechanicalStep D2_L0R1_12[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][12].Set(13, D2_L0R1_12, temprot, tempstate);
	//D2_L0R1_13
	MechanicalStep D2_L0R1_13[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][13].Set(13, D2_L0R1_13, temprot, tempstate);
	//D2_L0R1_14
	MechanicalStep D2_L0R1_14[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][14].Set(15, D2_L0R1_14, temprot, tempstate);
	//D2_L0R1_15
	MechanicalStep D2_L0R1_15[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_0_R_1][15].Set(15, D2_L0R1_15, temprot, tempstate);
}
void D3_L0R1int(void)
{
	//D3_L0R1_0
	MechanicalStep D3_L0R1_0[] = { M_LO, M_R1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][0].Set(4, D3_L0R1_0, temprot, tempstate);
	//D3_L0R1_1
	MechanicalStep D3_L0R1_1[] = { M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][1].Set(5, D3_L0R1_1, temprot, tempstate);
	//D3_L0R1_2
	MechanicalStep D3_L0R1_2[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][2].Set(8, D3_L0R1_2, temprot, tempstate);
	//D3_L0R1_3
	MechanicalStep D3_L0R1_3[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][3].Set(9, D3_L0R1_3, temprot, tempstate);
	//D3_L0R1_4
	MechanicalStep D3_L0R1_4[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][4].Set(9, D3_L0R1_4, temprot, tempstate);
	//D3_L0R1_5
	MechanicalStep D3_L0R1_5[] = { M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_R3, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][5].Set(9, D3_L0R1_5, temprot, tempstate);
	//D3_L0R1_6
	MechanicalStep D3_L0R1_6[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][6].Set(9, D3_L0R1_6, temprot, tempstate);
	//D3_L0R1_7
	MechanicalStep D3_L0R1_7[] = { M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][7].Set(10, D3_L0R1_7, temprot, tempstate);
	//D3_L0R1_8
	MechanicalStep D3_L0R1_8[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][8].Set(11, D3_L0R1_8, temprot, tempstate);
	//D3_L0R1_9
	MechanicalStep D3_L0R1_9[] = { M_RO, M_R1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][9].Set(11, D3_L0R1_9, temprot, tempstate);
	//D3_L0R1_10
	MechanicalStep D3_L0R1_10[] = { M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][10].Set(11, D3_L0R1_10, temprot, tempstate);
	//D3_L0R1_11
	MechanicalStep D3_L0R1_11[] = { M_RO, M_R1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][11].Set(12, D3_L0R1_11, temprot, tempstate);
	//D3_L0R1_12
	MechanicalStep D3_L0R1_12[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][12].Set(13, D3_L0R1_12, temprot, tempstate);
	//D3_L0R1_13
	MechanicalStep D3_L0R1_13[] = { M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][13].Set(13, D3_L0R1_13, temprot, tempstate);
	//D3_L0R1_14
	MechanicalStep D3_L0R1_14[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][14].Set(15, D3_L0R1_14, temprot, tempstate);
	//D3_L0R1_15
	MechanicalStep D3_L0R1_15[] = { M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_3][L_0_R_1][15].Set(15, D3_L0R1_15, temprot, tempstate);
}
void F1_L1R0int(void)
{
	//F1_L1R0_0
	MechanicalStep F1_L1R0_0[] = { M_L1, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][0].Set(1, F1_L1R0_0, temprot, tempstate);
	//F1_L1R0_1
	MechanicalStep F1_L1R0_1[] = { M_RO, M_L2, M_RC, M_L1, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][1].Set(4, F1_L1R0_1, temprot, tempstate);
	//F1_L1R0_2
	MechanicalStep F1_L1R0_2[] = { M_LO, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][2].Set(4, F1_L1R0_2, temprot, tempstate);
	//F1_L1R0_3
	MechanicalStep F1_L1R0_3[] = { M_RO, M_L1, M_RC, M_L1, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][3].Set(4, F1_L1R0_3, temprot, tempstate);
	//F1_L1R0_4
	MechanicalStep F1_L1R0_4[] = { M_RO, M_L3, M_RC, M_L1, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][4].Set(4, F1_L1R0_4, temprot, tempstate);
	//F1_L1R0_5
	MechanicalStep F1_L1R0_5[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][5].Set(12, F1_L1R0_5, temprot, tempstate);
	//F1_L1R0_6
	MechanicalStep F1_L1R0_6[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][6].Set(12, F1_L1R0_6, temprot, tempstate);
	//F1_L1R0_7
	MechanicalStep F1_L1R0_7[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][7].Set(12, F1_L1R0_7, temprot, tempstate);
	//F1_L1R0_8
	MechanicalStep F1_L1R0_8[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][8].Set(12, F1_L1R0_8, temprot, tempstate);
	//F1_L1R0_9
	MechanicalStep F1_L1R0_9[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][9].Set(13, F1_L1R0_9, temprot, tempstate);
	//F1_L1R0_10
	MechanicalStep F1_L1R0_10[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][10].Set(13, F1_L1R0_10, temprot, tempstate);
	//F1_L1R0_11
	MechanicalStep F1_L1R0_11[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][11].Set(13, F1_L1R0_11, temprot, tempstate);
	//F1_L1R0_12
	MechanicalStep F1_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][12].Set(13, F1_L1R0_12, temprot, tempstate);
	//F1_L1R0_13
	MechanicalStep F1_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][13].Set(13, F1_L1R0_13, temprot, tempstate);
	//F1_L1R0_14
	MechanicalStep F1_L1R0_14[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][14].Set(15, F1_L1R0_14, temprot, tempstate);
	//F1_L1R0_15
	MechanicalStep F1_L1R0_15[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_1][L_1_R_0][15].Set(15, F1_L1R0_15, temprot, tempstate);
}
void F2_L1R0int(void)
{
	//F2_L1R0_0
	MechanicalStep F2_L1R0_0[] = { M_L2, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][0].Set(1, F2_L1R0_0, temprot, tempstate);
	//F2_L1R0_1
	MechanicalStep F2_L1R0_1[] = { M_LO, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][1].Set(4, F2_L1R0_1, temprot, tempstate);
	//F2_L1R0_2
	MechanicalStep F2_L1R0_2[] = { M_RO, M_L1, M_RC, M_L2, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][2].Set(4, F2_L1R0_2, temprot, tempstate);
	//F2_L1R0_3
	MechanicalStep F2_L1R0_3[] = { M_RO, M_L3, M_RC, M_L2, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][3].Set(4, F2_L1R0_3, temprot, tempstate);
	//F2_L1R0_4
	MechanicalStep F2_L1R0_4[] = { M_RO, M_L2, M_RC, M_L2, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][4].Set(4, F2_L1R0_4, temprot, tempstate);
	//F2_L1R0_5
	MechanicalStep F2_L1R0_5[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][5].Set(12, F2_L1R0_5, temprot, tempstate);
	//F2_L1R0_6
	MechanicalStep F2_L1R0_6[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][6].Set(12, F2_L1R0_6, temprot, tempstate);
	//F2_L1R0_7
	MechanicalStep F2_L1R0_7[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][7].Set(12, F2_L1R0_7, temprot, tempstate);
	//F2_L1R0_8
	MechanicalStep F2_L1R0_8[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][8].Set(12, F2_L1R0_8, temprot, tempstate);
	//F2_L1R0_9
	MechanicalStep F2_L1R0_9[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][9].Set(13, F2_L1R0_9, temprot, tempstate);
	//F2_L1R0_10
	MechanicalStep F2_L1R0_10[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][10].Set(13, F2_L1R0_10, temprot, tempstate);
	//F2_L1R0_11
	MechanicalStep F2_L1R0_11[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][11].Set(13, F2_L1R0_11, temprot, tempstate);
	//F2_L1R0_12
	MechanicalStep F2_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][12].Set(13, F2_L1R0_12, temprot, tempstate);
	//F2_L1R0_13
	MechanicalStep F2_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][13].Set(13, F2_L1R0_13, temprot, tempstate);
	//F2_L1R0_14
	MechanicalStep F2_L1R0_14[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][14].Set(15, F2_L1R0_14, temprot, tempstate);
	//F2_L1R0_15
	MechanicalStep F2_L1R0_15[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_2][L_1_R_0][15].Set(15, F2_L1R0_15, temprot, tempstate);
}
void F3_L1R0int(void)
{
	//F3_L1R0_0
	MechanicalStep F3_L1R0_0[] = { M_L3, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][0].Set(1, F3_L1R0_0, temprot, tempstate);
	//F3_L1R0_1
	MechanicalStep F3_L1R0_1[] = { M_RO, M_L2, M_RC, M_L3, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][1].Set(4, F3_L1R0_1, temprot, tempstate);
	//F3_L1R0_2
	MechanicalStep F3_L1R0_2[] = { M_LO, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][2].Set(4, F3_L1R0_2, temprot, tempstate);
	//F3_L1R0_3
	MechanicalStep F3_L1R0_3[] = { M_RO, M_L1, M_RC, M_L3, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][3].Set(4, F3_L1R0_3, temprot, tempstate);
	//F3_L1R0_4
	MechanicalStep F3_L1R0_4[] = { M_RO, M_L3, M_RC, M_L3, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][4].Set(4, F3_L1R0_4, temprot, tempstate);
	//F3_L1R0_5
	MechanicalStep F3_L1R0_5[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][5].Set(12, F3_L1R0_5, temprot, tempstate);
	//F3_L1R0_6
	MechanicalStep F3_L1R0_6[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][6].Set(12, F3_L1R0_6, temprot, tempstate);
	//F3_L1R0_7
	MechanicalStep F3_L1R0_7[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][7].Set(12, F3_L1R0_7, temprot, tempstate);
	//F3_L1R0_8
	MechanicalStep F3_L1R0_8[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][8].Set(12, F3_L1R0_8, temprot, tempstate);
	//F3_L1R0_9
	MechanicalStep F3_L1R0_9[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][9].Set(13, F3_L1R0_9, temprot, tempstate);
	//F3_L1R0_10
	MechanicalStep F3_L1R0_10[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][10].Set(13, F3_L1R0_10, temprot, tempstate);
	//F3_L1R0_11
	MechanicalStep F3_L1R0_11[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][11].Set(13, F3_L1R0_11, temprot, tempstate);
	//F3_L1R0_12
	MechanicalStep F3_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][12].Set(13, F3_L1R0_12, temprot, tempstate);
	//F3_L1R0_13
	MechanicalStep F3_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][13].Set(13, F3_L1R0_13, temprot, tempstate);
	//F3_L1R0_14
	MechanicalStep F3_L1R0_14[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][14].Set(15, F3_L1R0_14, temprot, tempstate);
	//F3_L1R0_15
	MechanicalStep F3_L1R0_15[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[F][_3][L_1_R_0][15].Set(15, F3_L1R0_15, temprot, tempstate);
}
void R1_L1R0int(void)
{
	//R1_L1R0_0
	MechanicalStep R1_L1R0_0[] = { M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][0].Set(4, R1_L1R0_0, temprot, tempstate);
	//R1_L1R0_1
	MechanicalStep R1_L1R0_1[] = { M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][1].Set(5, R1_L1R0_1, temprot, tempstate);
	//R1_L1R0_2
	MechanicalStep R1_L1R0_2[] = { M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][2].Set(5, R1_L1R0_2, temprot, tempstate);
	//R1_L1R0_3
	MechanicalStep R1_L1R0_3[] = { M_LO, M_L1, M_R2, M_LC, M_R1, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][3].Set(5, R1_L1R0_3, temprot, tempstate);
	//R1_L1R0_4
	MechanicalStep R1_L1R0_4[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][4].Set(8, R1_L1R0_4, temprot, tempstate);
	//R1_L1R0_5
	MechanicalStep R1_L1R0_5[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][5].Set(8, R1_L1R0_5, temprot, tempstate);
	//R1_L1R0_6
	MechanicalStep R1_L1R0_6[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][6].Set(9, R1_L1R0_6, temprot, tempstate);
	//R1_L1R0_7
	MechanicalStep R1_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][7].Set(9, R1_L1R0_7, temprot, tempstate);
	//R1_L1R0_8
	MechanicalStep R1_L1R0_8[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][8].Set(11, R1_L1R0_8, temprot, tempstate);
	//R1_L1R0_9
	MechanicalStep R1_L1R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][9].Set(11, R1_L1R0_9, temprot, tempstate);
	//R1_L1R0_10
	MechanicalStep R1_L1R0_10[] = { M_RO, M_L1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][10].Set(11, R1_L1R0_10, temprot, tempstate);
	//R1_L1R0_11
	MechanicalStep R1_L1R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][11].Set(12, R1_L1R0_11, temprot, tempstate);
	//R1_L1R0_12
	MechanicalStep R1_L1R0_12[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][12].Set(12, R1_L1R0_12, temprot, tempstate);
	//R1_L1R0_13
	MechanicalStep R1_L1R0_13[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][13].Set(12, R1_L1R0_13, temprot, tempstate);
	//R1_L1R0_14
	MechanicalStep R1_L1R0_14[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][14].Set(19, R1_L1R0_14, temprot, tempstate);
	//R1_L1R0_15
	MechanicalStep R1_L1R0_15[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_1][L_1_R_0][15].Set(19, R1_L1R0_15, temprot, tempstate);
}
void R2_L1R0int(void)
{
	//R2_L1R0_0
	MechanicalStep R2_L1R0_0[] = { M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][0].Set(4, R2_L1R0_0, temprot, tempstate);
	//R2_L1R0_1
	MechanicalStep R2_L1R0_1[] = { M_LO, M_L1, M_R2, M_LC, M_R2, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][1].Set(5, R2_L1R0_1, temprot, tempstate);
	//R2_L1R0_2
	MechanicalStep R2_L1R0_2[] = { M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][2].Set(5, R2_L1R0_2, temprot, tempstate);
	//R2_L1R0_3
	MechanicalStep R2_L1R0_3[] = { M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][3].Set(5, R2_L1R0_3, temprot, tempstate);
	//R2_L1R0_4
	MechanicalStep R2_L1R0_4[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][4].Set(8, R2_L1R0_4, temprot, tempstate);
	//R2_L1R0_5
	MechanicalStep R2_L1R0_5[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][5].Set(8, R2_L1R0_5, temprot, tempstate);
	//R2_L1R0_6
	MechanicalStep R2_L1R0_6[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][6].Set(9, R2_L1R0_6, temprot, tempstate);
	//R2_L1R0_7
	MechanicalStep R2_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][7].Set(9, R2_L1R0_7, temprot, tempstate);
	//R2_L1R0_8
	MechanicalStep R2_L1R0_8[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][8].Set(11, R2_L1R0_8, temprot, tempstate);
	//R2_L1R0_9
	MechanicalStep R2_L1R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][9].Set(11, R2_L1R0_9, temprot, tempstate);
	//R2_L1R0_10
	MechanicalStep R2_L1R0_10[] = { M_RO, M_L1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][10].Set(11, R2_L1R0_10, temprot, tempstate);
	//R2_L1R0_11
	MechanicalStep R2_L1R0_11[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][11].Set(12, R2_L1R0_11, temprot, tempstate);
	//R2_L1R0_12
	MechanicalStep R2_L1R0_12[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][12].Set(12, R2_L1R0_12, temprot, tempstate);
	//R2_L1R0_13
	MechanicalStep R2_L1R0_13[] = { M_RO, M_L1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][13].Set(12, R2_L1R0_13, temprot, tempstate);
	//R2_L1R0_14
	MechanicalStep R2_L1R0_14[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][14].Set(19, R2_L1R0_14, temprot, tempstate);
	//R2_L1R0_15
	MechanicalStep R2_L1R0_15[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_2][L_1_R_0][15].Set(19, R2_L1R0_15, temprot, tempstate);
}
void R3_L1R0int(void)
{
	//R3_L1R0_0
	MechanicalStep R3_L1R0_0[] = { M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][0].Set(4, R3_L1R0_0, temprot, tempstate);
	//R3_L1R0_1
	MechanicalStep R3_L1R0_1[] = { M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][1].Set(5, R3_L1R0_1, temprot, tempstate);
	//R3_L1R0_2
	MechanicalStep R3_L1R0_2[] = { M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][2].Set(5, R3_L1R0_2, temprot, tempstate);
	//R3_L1R0_3
	MechanicalStep R3_L1R0_3[] = { M_LO, M_L1, M_R2, M_LC, M_R3, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][3].Set(5, R3_L1R0_3, temprot, tempstate);
	//R3_L1R0_4
	MechanicalStep R3_L1R0_4[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][4].Set(8, R3_L1R0_4, temprot, tempstate);
	//R3_L1R0_5
	MechanicalStep R3_L1R0_5[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][5].Set(8, R3_L1R0_5, temprot, tempstate);
	//R3_L1R0_6
	MechanicalStep R3_L1R0_6[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][6].Set(9, R3_L1R0_6, temprot, tempstate);
	//R3_L1R0_7
	MechanicalStep R3_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][7].Set(9, R3_L1R0_7, temprot, tempstate);
	//R3_L1R0_8
	MechanicalStep R3_L1R0_8[] = { M_RO, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 1, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][8].Set(11, R3_L1R0_8, temprot, tempstate);
	//R3_L1R0_9
	MechanicalStep R3_L1R0_9[] = { M_RO, M_L1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][9].Set(11, R3_L1R0_9, temprot, tempstate);
	//R3_L1R0_10
	MechanicalStep R3_L1R0_10[] = { M_RO, M_L1, M_RC, M_LO, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][10].Set(11, R3_L1R0_10, temprot, tempstate);
	//R3_L1R0_11
	MechanicalStep R3_L1R0_11[] = { M_RO, M_L1, M_RC, M_LO, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][11].Set(12, R3_L1R0_11, temprot, tempstate);
	//R3_L1R0_12
	MechanicalStep R3_L1R0_12[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][12].Set(12, R3_L1R0_12, temprot, tempstate);
	//R3_L1R0_13
	MechanicalStep R3_L1R0_13[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][13].Set(12, R3_L1R0_13, temprot, tempstate);
	//R3_L1R0_14
	MechanicalStep R3_L1R0_14[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][14].Set(19, R3_L1R0_14, temprot, tempstate);
	//R3_L1R0_15
	MechanicalStep R3_L1R0_15[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 0, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[R][_3][L_1_R_0][15].Set(19, R3_L1R0_15, temprot, tempstate);
}
void U1_L1R0int(void)
{
	//U1_L1R0_0
	MechanicalStep U1_L1R0_0[] = { M_RO, M_L1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][0].Set(4, U1_L1R0_0, temprot, tempstate);
	//U1_L1R0_1
	MechanicalStep U1_L1R0_1[] = { M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][1].Set(5, U1_L1R0_1, temprot, tempstate);
	//U1_L1R0_2
	MechanicalStep U1_L1R0_2[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][2].Set(8, U1_L1R0_2, temprot, tempstate);
	//U1_L1R0_3
	MechanicalStep U1_L1R0_3[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][3].Set(9, U1_L1R0_3, temprot, tempstate);
	//U1_L1R0_4
	MechanicalStep U1_L1R0_4[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][4].Set(9, U1_L1R0_4, temprot, tempstate);
	//U1_L1R0_5
	MechanicalStep U1_L1R0_5[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][5].Set(9, U1_L1R0_5, temprot, tempstate);
	//U1_L1R0_6
	MechanicalStep U1_L1R0_6[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_L1, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][6].Set(9, U1_L1R0_6, temprot, tempstate);
	//U1_L1R0_7
	MechanicalStep U1_L1R0_7[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][7].Set(10, U1_L1R0_7, temprot, tempstate);
	//U1_L1R0_8
	MechanicalStep U1_L1R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][8].Set(11, U1_L1R0_8, temprot, tempstate);
	//U1_L1R0_9
	MechanicalStep U1_L1R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][9].Set(11, U1_L1R0_9, temprot, tempstate);
	//U1_L1R0_10
	MechanicalStep U1_L1R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][10].Set(11, U1_L1R0_10, temprot, tempstate);
	//U1_L1R0_11
	MechanicalStep U1_L1R0_11[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][11].Set(12, U1_L1R0_11, temprot, tempstate);
	//U1_L1R0_12
	MechanicalStep U1_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][12].Set(13, U1_L1R0_12, temprot, tempstate);
	//U1_L1R0_13
	MechanicalStep U1_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][13].Set(13, U1_L1R0_13, temprot, tempstate);
	//U1_L1R0_14
	MechanicalStep U1_L1R0_14[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][14].Set(15, U1_L1R0_14, temprot, tempstate);
	//U1_L1R0_15
	MechanicalStep U1_L1R0_15[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_1][L_1_R_0][15].Set(15, U1_L1R0_15, temprot, tempstate);
}
void U2_L1R0int(void)
{
	//U2_L1R0_0
	MechanicalStep U2_L1R0_0[] = { M_RO, M_L1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][0].Set(4, U2_L1R0_0, temprot, tempstate);
	//U2_L1R0_1
	MechanicalStep U2_L1R0_1[] = { M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][1].Set(5, U2_L1R0_1, temprot, tempstate);
	//U2_L1R0_2
	MechanicalStep U2_L1R0_2[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][2].Set(8, U2_L1R0_2, temprot, tempstate);
	//U2_L1R0_3
	MechanicalStep U2_L1R0_3[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][3].Set(9, U2_L1R0_3, temprot, tempstate);
	//U2_L1R0_4
	MechanicalStep U2_L1R0_4[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_L2, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][4].Set(9, U2_L1R0_4, temprot, tempstate);
	//U2_L1R0_5
	MechanicalStep U2_L1R0_5[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][5].Set(9, U2_L1R0_5, temprot, tempstate);
	//U2_L1R0_6
	MechanicalStep U2_L1R0_6[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][6].Set(9, U2_L1R0_6, temprot, tempstate);
	//U2_L1R0_7
	MechanicalStep U2_L1R0_7[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][7].Set(10, U2_L1R0_7, temprot, tempstate);
	//U2_L1R0_8
	MechanicalStep U2_L1R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][8].Set(11, U2_L1R0_8, temprot, tempstate);
	//U2_L1R0_9
	MechanicalStep U2_L1R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][9].Set(11, U2_L1R0_9, temprot, tempstate);
	//U2_L1R0_10
	MechanicalStep U2_L1R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][10].Set(11, U2_L1R0_10, temprot, tempstate);
	//U2_L1R0_11
	MechanicalStep U2_L1R0_11[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][11].Set(12, U2_L1R0_11, temprot, tempstate);
	//U2_L1R0_12
	MechanicalStep U2_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][12].Set(13, U2_L1R0_12, temprot, tempstate);
	//U2_L1R0_13
	MechanicalStep U2_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][13].Set(13, U2_L1R0_13, temprot, tempstate);
	//U2_L1R0_14
	MechanicalStep U2_L1R0_14[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][14].Set(15, U2_L1R0_14, temprot, tempstate);
	//U2_L1R0_15
	MechanicalStep U2_L1R0_15[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_2][L_1_R_0][15].Set(15, U2_L1R0_15, temprot, tempstate);
}
void U3_L1R0int(void)
{
	//U3_L1R0_0
	MechanicalStep U3_L1R0_0[] = { M_RO, M_L1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][0].Set(4, U3_L1R0_0, temprot, tempstate);
	//U3_L1R0_1
	MechanicalStep U3_L1R0_1[] = { M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 2, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][1].Set(5, U3_L1R0_1, temprot, tempstate);
	//U3_L1R0_2
	MechanicalStep U3_L1R0_2[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][2].Set(8, U3_L1R0_2, temprot, tempstate);
	//U3_L1R0_3
	MechanicalStep U3_L1R0_3[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][3].Set(9, U3_L1R0_3, temprot, tempstate);
	//U3_L1R0_4
	MechanicalStep U3_L1R0_4[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][4].Set(9, U3_L1R0_4, temprot, tempstate);
	//U3_L1R0_5
	MechanicalStep U3_L1R0_5[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][5].Set(9, U3_L1R0_5, temprot, tempstate);
	//U3_L1R0_6
	MechanicalStep U3_L1R0_6[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_L3, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][6].Set(9, U3_L1R0_6, temprot, tempstate);
	//U3_L1R0_7
	MechanicalStep U3_L1R0_7[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][7].Set(10, U3_L1R0_7, temprot, tempstate);
	//U3_L1R0_8
	MechanicalStep U3_L1R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][8].Set(11, U3_L1R0_8, temprot, tempstate);
	//U3_L1R0_9
	MechanicalStep U3_L1R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 1, 1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][9].Set(11, U3_L1R0_9, temprot, tempstate);
	//U3_L1R0_10
	MechanicalStep U3_L1R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][10].Set(11, U3_L1R0_10, temprot, tempstate);
	//U3_L1R0_11
	MechanicalStep U3_L1R0_11[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][11].Set(12, U3_L1R0_11, temprot, tempstate);
	//U3_L1R0_12
	MechanicalStep U3_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(1, 1, 2, 1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][12].Set(13, U3_L1R0_12, temprot, tempstate);
	//U3_L1R0_13
	MechanicalStep U3_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][13].Set(13, U3_L1R0_13, temprot, tempstate);
	//U3_L1R0_14
	MechanicalStep U3_L1R0_14[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][14].Set(15, U3_L1R0_14, temprot, tempstate);
	//U3_L1R0_15
	MechanicalStep U3_L1R0_15[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 0, 1, 1, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[U][_3][L_1_R_0][15].Set(15, U3_L1R0_15, temprot, tempstate);
}
void B1_L1R0int(void)
{
	//B1_L1R0_0
	MechanicalStep B1_L1R0_0[] = { M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][0].Set(5, B1_L1R0_0, temprot, tempstate);
	//B1_L1R0_1
	MechanicalStep B1_L1R0_1[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][1].Set(6, B1_L1R0_1, temprot, tempstate);
	//B1_L1R0_2
	MechanicalStep B1_L1R0_2[] = { M_RO, M_L3, M_RC, M_LO, M_R2, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][2].Set(8, B1_L1R0_2, temprot, tempstate);
	//B1_L1R0_3
	MechanicalStep B1_L1R0_3[] = { M_RO, M_L1, M_RC, M_LO, M_R2, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][3].Set(8, B1_L1R0_3, temprot, tempstate);
	//B1_L1R0_4
	MechanicalStep B1_L1R0_4[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][4].Set(8, B1_L1R0_4, temprot, tempstate);
	//B1_L1R0_5
	MechanicalStep B1_L1R0_5[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L1, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][5].Set(9, B1_L1R0_5, temprot, tempstate);
	//B1_L1R0_6
	MechanicalStep B1_L1R0_6[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][6].Set(11, B1_L1R0_6, temprot, tempstate);
	//B1_L1R0_7
	MechanicalStep B1_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][7].Set(11, B1_L1R0_7, temprot, tempstate);
	//B1_L1R0_8
	MechanicalStep B1_L1R0_8[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R1, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][8].Set(12, B1_L1R0_8, temprot, tempstate);
	//B1_L1R0_9
	MechanicalStep B1_L1R0_9[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][9].Set(12, B1_L1R0_9, temprot, tempstate);
	//B1_L1R0_10
	MechanicalStep B1_L1R0_10[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][10].Set(12, B1_L1R0_10, temprot, tempstate);
	//B1_L1R0_11
	MechanicalStep B1_L1R0_11[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][11].Set(12, B1_L1R0_11, temprot, tempstate);
	//B1_L1R0_12
	MechanicalStep B1_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][12].Set(13, B1_L1R0_12, temprot, tempstate);
	//B1_L1R0_13
	MechanicalStep B1_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][13].Set(13, B1_L1R0_13, temprot, tempstate);
	//B1_L1R0_14
	MechanicalStep B1_L1R0_14[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R1, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][14].Set(15, B1_L1R0_14, temprot, tempstate);
	//B1_L1R0_15
	MechanicalStep B1_L1R0_15[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_1][L_1_R_0][15].Set(15, B1_L1R0_15, temprot, tempstate);
}
void B2_L1R0int(void)
{
	//B2_L1R0_0
	MechanicalStep B2_L1R0_0[] = { M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][0].Set(5, B2_L1R0_0, temprot, tempstate);
	//B2_L1R0_1
	MechanicalStep B2_L1R0_1[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][1].Set(6, B2_L1R0_1, temprot, tempstate);
	//B2_L1R0_2
	MechanicalStep B2_L1R0_2[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][2].Set(8, B2_L1R0_2, temprot, tempstate);
	//B2_L1R0_3
	MechanicalStep B2_L1R0_3[] = { M_RO, M_L3, M_RC, M_LO, M_R2, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][3].Set(8, B2_L1R0_3, temprot, tempstate);
	//B2_L1R0_4
	MechanicalStep B2_L1R0_4[] = { M_RO, M_L1, M_RC, M_LO, M_R2, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][4].Set(8, B2_L1R0_4, temprot, tempstate);
	//B2_L1R0_5
	MechanicalStep B2_L1R0_5[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L2, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][5].Set(9, B2_L1R0_5, temprot, tempstate);
	//B2_L1R0_6
	MechanicalStep B2_L1R0_6[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][6].Set(11, B2_L1R0_6, temprot, tempstate);
	//B2_L1R0_7
	MechanicalStep B2_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][7].Set(11, B2_L1R0_7, temprot, tempstate);
	//B2_L1R0_8
	MechanicalStep B2_L1R0_8[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R2, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][8].Set(12, B2_L1R0_8, temprot, tempstate);
	//B2_L1R0_9
	MechanicalStep B2_L1R0_9[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][9].Set(12, B2_L1R0_9, temprot, tempstate);
	//B2_L1R0_10
	MechanicalStep B2_L1R0_10[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][10].Set(12, B2_L1R0_10, temprot, tempstate);
	//B2_L1R0_11
	MechanicalStep B2_L1R0_11[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][11].Set(12, B2_L1R0_11, temprot, tempstate);
	//B2_L1R0_12
	MechanicalStep B2_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][12].Set(13, B2_L1R0_12, temprot, tempstate);
	//B2_L1R0_13
	MechanicalStep B2_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][13].Set(13, B2_L1R0_13, temprot, tempstate);
	//B2_L1R0_14
	MechanicalStep B2_L1R0_14[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R2, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][14].Set(15, B2_L1R0_14, temprot, tempstate);
	//B2_L1R0_15
	MechanicalStep B2_L1R0_15[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_2][L_1_R_0][15].Set(15, B2_L1R0_15, temprot, tempstate);
}
void B3_L1R0int(void)
{
	//B3_L1R0_0
	MechanicalStep B3_L1R0_0[] = { M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][0].Set(5, B3_L1R0_0, temprot, tempstate);
	//B3_L1R0_1
	MechanicalStep B3_L1R0_1[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 1, 1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][1].Set(6, B3_L1R0_1, temprot, tempstate);
	//B3_L1R0_2
	MechanicalStep B3_L1R0_2[] = { M_RO, M_L3, M_RC, M_LO, M_R2, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][2].Set(8, B3_L1R0_2, temprot, tempstate);
	//B3_L1R0_3
	MechanicalStep B3_L1R0_3[] = { M_RO, M_L1, M_RC, M_LO, M_R2, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][3].Set(8, B3_L1R0_3, temprot, tempstate);
	//B3_L1R0_4
	MechanicalStep B3_L1R0_4[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][4].Set(8, B3_L1R0_4, temprot, tempstate);
	//B3_L1R0_5
	MechanicalStep B3_L1R0_5[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_L1, M_LC, M_L3, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][5].Set(9, B3_L1R0_5, temprot, tempstate);
	//B3_L1R0_6
	MechanicalStep B3_L1R0_6[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][6].Set(11, B3_L1R0_6, temprot, tempstate);
	//B3_L1R0_7
	MechanicalStep B3_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R2, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(0, -1, 2, 1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][7].Set(11, B3_L1R0_7, temprot, tempstate);
	//B3_L1R0_8
	MechanicalStep B3_L1R0_8[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_RC, M_R3, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][8].Set(12, B3_L1R0_8, temprot, tempstate);
	//B3_L1R0_9
	MechanicalStep B3_L1R0_9[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][9].Set(12, B3_L1R0_9, temprot, tempstate);
	//B3_L1R0_10
	MechanicalStep B3_L1R0_10[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][10].Set(12, B3_L1R0_10, temprot, tempstate);
	//B3_L1R0_11
	MechanicalStep B3_L1R0_11[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][11].Set(12, B3_L1R0_11, temprot, tempstate);
	//B3_L1R0_12
	MechanicalStep B3_L1R0_12[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(1, -1, 0, 1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][12].Set(13, B3_L1R0_12, temprot, tempstate);
	//B3_L1R0_13
	MechanicalStep B3_L1R0_13[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][13].Set(13, B3_L1R0_13, temprot, tempstate);
	//B3_L1R0_14
	MechanicalStep B3_L1R0_14[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][14].Set(15, B3_L1R0_14, temprot, tempstate);
	//B3_L1R0_15
	MechanicalStep B3_L1R0_15[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(1, -1, 2, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[B][_3][L_1_R_0][15].Set(15, B3_L1R0_15, temprot, tempstate);
}
void L1_L1R0int(void)
{
	//L1_L1R0_0
	MechanicalStep L1_L1R0_0[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][0].Set(7, L1_L1R0_0, temprot, tempstate);
	//L1_L1R0_1
	MechanicalStep L1_L1R0_1[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][1].Set(8, L1_L1R0_1, temprot, tempstate);
	//L1_L1R0_2
	MechanicalStep L1_L1R0_2[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][2].Set(8, L1_L1R0_2, temprot, tempstate);
	//L1_L1R0_3
	MechanicalStep L1_L1R0_3[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][3].Set(8, L1_L1R0_3, temprot, tempstate);
	//L1_L1R0_4
	MechanicalStep L1_L1R0_4[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][4].Set(8, L1_L1R0_4, temprot, tempstate);
	//L1_L1R0_5
	MechanicalStep L1_L1R0_5[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][5].Set(8, L1_L1R0_5, temprot, tempstate);
	//L1_L1R0_6
	MechanicalStep L1_L1R0_6[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_R1, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][6].Set(8, L1_L1R0_6, temprot, tempstate);
	//L1_L1R0_7
	MechanicalStep L1_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][7].Set(9, L1_L1R0_7, temprot, tempstate);
	//L1_L1R0_8
	MechanicalStep L1_L1R0_8[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][8].Set(9, L1_L1R0_8, temprot, tempstate);
	//L1_L1R0_9
	MechanicalStep L1_L1R0_9[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][9].Set(9, L1_L1R0_9, temprot, tempstate);
	//L1_L1R0_10
	MechanicalStep L1_L1R0_10[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][10].Set(9, L1_L1R0_10, temprot, tempstate);
	//L1_L1R0_11
	MechanicalStep L1_L1R0_11[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R1, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][11].Set(9, L1_L1R0_11, temprot, tempstate);
	//L1_L1R0_12
	MechanicalStep L1_L1R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][12].Set(11, L1_L1R0_12, temprot, tempstate);
	//L1_L1R0_13
	MechanicalStep L1_L1R0_13[] = { M_RO, M_L1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][13].Set(11, L1_L1R0_13, temprot, tempstate);
	//L1_L1R0_14
	MechanicalStep L1_L1R0_14[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][14].Set(16, L1_L1R0_14, temprot, tempstate);
	//L1_L1R0_15
	MechanicalStep L1_L1R0_15[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_1][L_1_R_0][15].Set(16, L1_L1R0_15, temprot, tempstate);
}
void L2_L1R0int(void)
{
	//L2_L1R0_0
	MechanicalStep L2_L1R0_0[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][0].Set(7, L2_L1R0_0, temprot, tempstate);
	//L2_L1R0_1
	MechanicalStep L2_L1R0_1[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][1].Set(8, L2_L1R0_1, temprot, tempstate);
	//L2_L1R0_2
	MechanicalStep L2_L1R0_2[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][2].Set(8, L2_L1R0_2, temprot, tempstate);
	//L2_L1R0_3
	MechanicalStep L2_L1R0_3[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_R2, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][3].Set(8, L2_L1R0_3, temprot, tempstate);
	//L2_L1R0_4
	MechanicalStep L2_L1R0_4[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][4].Set(8, L2_L1R0_4, temprot, tempstate);
	//L2_L1R0_5
	MechanicalStep L2_L1R0_5[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][5].Set(8, L2_L1R0_5, temprot, tempstate);
	//L2_L1R0_6
	MechanicalStep L2_L1R0_6[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][6].Set(8, L2_L1R0_6, temprot, tempstate);
	//L2_L1R0_7
	MechanicalStep L2_L1R0_7[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][7].Set(9, L2_L1R0_7, temprot, tempstate);
	//L2_L1R0_8
	MechanicalStep L2_L1R0_8[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R2, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][8].Set(9, L2_L1R0_8, temprot, tempstate);
	//L2_L1R0_9
	MechanicalStep L2_L1R0_9[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][9].Set(9, L2_L1R0_9, temprot, tempstate);
	//L2_L1R0_10
	MechanicalStep L2_L1R0_10[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][10].Set(9, L2_L1R0_10, temprot, tempstate);
	//L2_L1R0_11
	MechanicalStep L2_L1R0_11[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][11].Set(9, L2_L1R0_11, temprot, tempstate);
	//L2_L1R0_12
	MechanicalStep L2_L1R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][12].Set(11, L2_L1R0_12, temprot, tempstate);
	//L2_L1R0_13
	MechanicalStep L2_L1R0_13[] = { M_RO, M_L1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][13].Set(11, L2_L1R0_13, temprot, tempstate);
	//L2_L1R0_14
	MechanicalStep L2_L1R0_14[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][14].Set(16, L2_L1R0_14, temprot, tempstate);
	//L2_L1R0_15
	MechanicalStep L2_L1R0_15[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_2][L_1_R_0][15].Set(16, L2_L1R0_15, temprot, tempstate);
}
void L3_L1R0int(void)
{
	//L3_L1R0_0
	MechanicalStep L3_L1R0_0[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][0].Set(7, L3_L1R0_0, temprot, tempstate);
	//L3_L1R0_1
	MechanicalStep L3_L1R0_1[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][1].Set(8, L3_L1R0_1, temprot, tempstate);
	//L3_L1R0_2
	MechanicalStep L3_L1R0_2[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][2].Set(8, L3_L1R0_2, temprot, tempstate);
	//L3_L1R0_3
	MechanicalStep L3_L1R0_3[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 1, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][3].Set(8, L3_L1R0_3, temprot, tempstate);
	//L3_L1R0_4
	MechanicalStep L3_L1R0_4[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][4].Set(8, L3_L1R0_4, temprot, tempstate);
	//L3_L1R0_5
	MechanicalStep L3_L1R0_5[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][5].Set(8, L3_L1R0_5, temprot, tempstate);
	//L3_L1R0_6
	MechanicalStep L3_L1R0_6[] = { M_RO, M_L2, M_RC, M_LO, M_L1, M_R2, M_LC, M_R3, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][6].Set(8, L3_L1R0_6, temprot, tempstate);
	//L3_L1R0_7
	MechanicalStep L3_L1R0_7[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 0, -1, 1, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][7].Set(9, L3_L1R0_7, temprot, tempstate);
	//L3_L1R0_8
	MechanicalStep L3_L1R0_8[] = { M_LO, M_L1, M_R2, M_LC, M_RO, M_L2, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 1, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][8].Set(9, L3_L1R0_8, temprot, tempstate);
	//L3_L1R0_9
	MechanicalStep L3_L1R0_9[] = { M_RO, M_L3, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][9].Set(9, L3_L1R0_9, temprot, tempstate);
	//L3_L1R0_10
	MechanicalStep L3_L1R0_10[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, 1, 1, -1, 0, 1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][10].Set(9, L3_L1R0_10, temprot, tempstate);
	//L3_L1R0_11
	MechanicalStep L3_L1R0_11[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_R3, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][11].Set(9, L3_L1R0_11, temprot, tempstate);
	//L3_L1R0_12
	MechanicalStep L3_L1R0_12[] = { M_RO, M_L1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][12].Set(11, L3_L1R0_12, temprot, tempstate);
	//L3_L1R0_13
	MechanicalStep L3_L1R0_13[] = { M_RO, M_L1, M_RC, M_LO, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][13].Set(11, L3_L1R0_13, temprot, tempstate);
	//L3_L1R0_14
	MechanicalStep L3_L1R0_14[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 0, -1, 2, 1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][14].Set(16, L3_L1R0_14, temprot, tempstate);
	//L3_L1R0_15
	MechanicalStep L3_L1R0_15[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 0, -1, 2, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[L][_3][L_1_R_0][15].Set(16, L3_L1R0_15, temprot, tempstate);
}
void D1_L1R0int(void)
{
	//D1_L1R0_0
	MechanicalStep D1_L1R0_0[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L1, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][0].Set(8, D1_L1R0_0, temprot, tempstate);
	//D1_L1R0_1
	MechanicalStep D1_L1R0_1[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L1, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][1].Set(9, D1_L1R0_1, temprot, tempstate);
	//D1_L1R0_2
	MechanicalStep D1_L1R0_2[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L1, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][2].Set(9, D1_L1R0_2, temprot, tempstate);
	//D1_L1R0_3
	MechanicalStep D1_L1R0_3[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_L1, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][3].Set(9, D1_L1R0_3, temprot, tempstate);
	//D1_L1R0_4
	MechanicalStep D1_L1R0_4[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][4].Set(9, D1_L1R0_4, temprot, tempstate);
	//D1_L1R0_5
	MechanicalStep D1_L1R0_5[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][5].Set(10, D1_L1R0_5, temprot, tempstate);
	//D1_L1R0_6
	MechanicalStep D1_L1R0_6[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][6].Set(10, D1_L1R0_6, temprot, tempstate);
	//D1_L1R0_7
	MechanicalStep D1_L1R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][7].Set(11, D1_L1R0_7, temprot, tempstate);
	//D1_L1R0_8
	MechanicalStep D1_L1R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][8].Set(11, D1_L1R0_8, temprot, tempstate);
	//D1_L1R0_9
	MechanicalStep D1_L1R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][9].Set(11, D1_L1R0_9, temprot, tempstate);
	//D1_L1R0_10
	MechanicalStep D1_L1R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L1, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][10].Set(12, D1_L1R0_10, temprot, tempstate);
	//D1_L1R0_11
	MechanicalStep D1_L1R0_11[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][11].Set(13, D1_L1R0_11, temprot, tempstate);
	//D1_L1R0_12
	MechanicalStep D1_L1R0_12[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L1, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][12].Set(13, D1_L1R0_12, temprot, tempstate);
	//D1_L1R0_13
	MechanicalStep D1_L1R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R1, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][13].Set(14, D1_L1R0_13, temprot, tempstate);
	//D1_L1R0_14
	MechanicalStep D1_L1R0_14[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][14].Set(15, D1_L1R0_14, temprot, tempstate);
	//D1_L1R0_15
	MechanicalStep D1_L1R0_15[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R1, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_1][L_1_R_0][15].Set(15, D1_L1R0_15, temprot, tempstate);
}
void D2_L1R0int(void)
{
	//D2_L1R0_0
	MechanicalStep D2_L1R0_0[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L2, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][0].Set(8, D2_L1R0_0, temprot, tempstate);
	//D2_L1R0_1
	MechanicalStep D2_L1R0_1[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][1].Set(9, D2_L1R0_1, temprot, tempstate);
	//D2_L1R0_2
	MechanicalStep D2_L1R0_2[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_L2, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][2].Set(9, D2_L1R0_2, temprot, tempstate);
	//D2_L1R0_3
	MechanicalStep D2_L1R0_3[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L2, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][3].Set(9, D2_L1R0_3, temprot, tempstate);
	//D2_L1R0_4
	MechanicalStep D2_L1R0_4[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L2, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][4].Set(9, D2_L1R0_4, temprot, tempstate);
	//D2_L1R0_5
	MechanicalStep D2_L1R0_5[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][5].Set(10, D2_L1R0_5, temprot, tempstate);
	//D2_L1R0_6
	MechanicalStep D2_L1R0_6[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][6].Set(10, D2_L1R0_6, temprot, tempstate);
	//D2_L1R0_7
	MechanicalStep D2_L1R0_7[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][7].Set(11, D2_L1R0_7, temprot, tempstate);
	//D2_L1R0_8
	MechanicalStep D2_L1R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][8].Set(11, D2_L1R0_8, temprot, tempstate);
	//D2_L1R0_9
	MechanicalStep D2_L1R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][9].Set(11, D2_L1R0_9, temprot, tempstate);
	//D2_L1R0_10
	MechanicalStep D2_L1R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L2, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][10].Set(12, D2_L1R0_10, temprot, tempstate);
	//D2_L1R0_11
	MechanicalStep D2_L1R0_11[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][11].Set(13, D2_L1R0_11, temprot, tempstate);
	//D2_L1R0_12
	MechanicalStep D2_L1R0_12[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L2, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][12].Set(13, D2_L1R0_12, temprot, tempstate);
	//D2_L1R0_13
	MechanicalStep D2_L1R0_13[] = { M_LO, M_L1, M_LC, M_RO, M_L1, M_RC, M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_R2, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][13].Set(14, D2_L1R0_13, temprot, tempstate);
	//D2_L1R0_14
	MechanicalStep D2_L1R0_14[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][14].Set(15, D2_L1R0_14, temprot, tempstate);
	//D2_L1R0_15
	MechanicalStep D2_L1R0_15[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R2, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_2][L_1_R_0][15].Set(15, D2_L1R0_15, temprot, tempstate);
}
void D3_L1R0int(void)
{
	//D3_L1R0_0
	MechanicalStep D3_L1R0_0[] = { M_RO, M_L3, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][0].Set(4, D3_L1R0_0, temprot, tempstate);
	//D3_L1R0_1
	MechanicalStep D3_L1R0_1[] = { M_RO, M_L3, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, 1, 2, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][1].Set(5, D3_L1R0_1, temprot, tempstate);
	//D3_L1R0_2
	MechanicalStep D3_L1R0_2[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_RC, M_L3, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][2].Set(8, D3_L1R0_2, temprot, tempstate);
	//D3_L1R0_3
	MechanicalStep D3_L1R0_3[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L1, M_RC, M_L3, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][3].Set(9, D3_L1R0_3, temprot, tempstate);
	//D3_L1R0_4
	MechanicalStep D3_L1R0_4[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L3, M_RC, M_L3, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][4].Set(9, D3_L1R0_4, temprot, tempstate);
	//D3_L1R0_5
	MechanicalStep D3_L1R0_5[] = { M_LO, M_L1, M_R1, M_LC, M_RO, M_R1, M_L2, M_RC, M_L3, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][5].Set(9, D3_L1R0_5, temprot, tempstate);
	//D3_L1R0_6
	MechanicalStep D3_L1R0_6[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][6].Set(9, D3_L1R0_6, temprot, tempstate);
	//D3_L1R0_7
	MechanicalStep D3_L1R0_7[] = { M_LO, M_L1, M_R2, M_L1, M_LC, M_RO, M_L1, M_R1, M_RC, M_R3, M_END };
	temprot.Set(0, -1, 2, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][7].Set(10, D3_L1R0_7, temprot, tempstate);
	//D3_L1R0_8
	MechanicalStep D3_L1R0_8[] = { M_LO, M_L1, M_LC, M_RO, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, 1, 1, 1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][8].Set(11, D3_L1R0_8, temprot, tempstate);
	//D3_L1R0_9
	MechanicalStep D3_L1R0_9[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][9].Set(11, D3_L1R0_9, temprot, tempstate);
	//D3_L1R0_10
	MechanicalStep D3_L1R0_10[] = { M_LO, M_L1, M_LC, M_RO, M_L3, M_RC, M_LO, M_L1, M_R3, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][10].Set(11, D3_L1R0_10, temprot, tempstate);
	//D3_L1R0_11
	MechanicalStep D3_L1R0_11[] = { M_LO, M_L1, M_LC, M_RO, M_L2, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_L3, M_END };
	temprot.Set(2, -1, 1, -1, 0, -1);
	tempstate.Set(0, 0, 0, 0);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][11].Set(12, D3_L1R0_11, temprot, tempstate);
	//D3_L1R0_12
	MechanicalStep D3_L1R0_12[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L1, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(1, -1, 2, 1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][12].Set(13, D3_L1R0_12, temprot, tempstate);
	//D3_L1R0_13
	MechanicalStep D3_L1R0_13[] = { M_LO, M_L1, M_R3, M_LC, M_RO, M_R1, M_L3, M_RC, M_LO, M_L1, M_R2, M_LC, M_L3, M_END };
	temprot.Set(1, 1, 2, -1, 0, -1);
	tempstate.Set(0, 0, 1, 0);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][13].Set(13, D3_L1R0_13, temprot, tempstate);
	//D3_L1R0_14
	MechanicalStep D3_L1R0_14[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R1, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(2, -1, 0, 1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][14].Set(15, D3_L1R0_14, temprot, tempstate);
	//D3_L1R0_15
	MechanicalStep D3_L1R0_15[] = { M_RO, M_L1, M_R1, M_RC, M_LO, M_R3, M_L1, M_LC, M_RO, M_L2, M_RC, M_LO, M_L1, M_LC, M_R3, M_END };
	temprot.Set(2, 1, 0, -1, 1, -1);
	tempstate.Set(0, 0, 0, 1);
	MechanicalHandGroupLibrary[D][_3][L_1_R_0][15].Set(15, D3_L1R0_15, temprot, tempstate);
}


void Libraryint(void)
{

	F1_L0R0int();
	F1_L0R1int();
	F1_L1R0int();
	F2_L0R0int();
	F2_L0R1int();
	F2_L1R0int();
	F3_L0R0int();
	F3_L0R1int();
	F3_L1R0int();
	R1_L0R0int();
	R1_L0R1int();
	R1_L1R0int();
	R2_L0R0int();
	R2_L0R1int();
	R2_L1R0int();
	R3_L0R0int();
	R3_L0R1int();
	R3_L1R0int();
	U1_L0R0int();
	U1_L0R1int();
	U1_L1R0int();
	U2_L0R0int();
	U2_L0R1int();
	U2_L1R0int();
	U3_L0R0int();
	U3_L0R1int();
	U3_L1R0int();
	B1_L0R0int();
	B1_L0R1int();
	B1_L1R0int();
	B2_L0R0int();
	B2_L0R1int();
	B2_L1R0int();
	B3_L0R0int();
	B3_L0R1int();
	B3_L1R0int();
	L1_L0R0int();
	L1_L0R1int();
	L1_L1R0int();
	L2_L0R0int();
	L2_L0R1int();
	L2_L1R0int();
	L3_L0R0int();
	L3_L0R1int();
	L3_L1R0int();
	D1_L0R0int();
	D1_L0R1int();
	D1_L1R0int();
	D2_L0R0int();
	D2_L0R1int();
	D2_L1R0int();
	D3_L0R0int();
	D3_L0R1int();
	D3_L1R0int();

}


void Librarytimeint(void)
{
	for (int i = F; i <= D; i++)
	{
		for (int j = _1; j <= _3; j++)
		{
			for (int k = L_0_R_0; k <= L_1_R_0; k++)
			{
				for (int m = 0; m < 16; m++)
				{
					MechanicalHandGroupLibrary[i][j][k][m].time = 0;
					int LeftHand = CLOSE;
					int RightHand = CLOSE;

					for (int n = 0; MechanicalHandGroupLibrary[i][j][k][m].steps[n].num != -1; n++)
					{
						if (MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == LO)
						{
							LeftHand = OPEN;
							MechanicalHandGroupLibrary[i][j][k][m].time +=  timeair;
						}
						else if (MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == LC)
						{
							LeftHand = CLOSE;
							MechanicalHandGroupLibrary[i][j][k][m].time += timeair;
						}
						else if (MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == RO)
						{
							RightHand = OPEN;
							MechanicalHandGroupLibrary[i][j][k][m].time +=  timeair;
						}
						else if (MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == RC)
						{
							RightHand = CLOSE;
							MechanicalHandGroupLibrary[i][j][k][m].time += timeair;
						}
						else
						{
							if ((LeftHand == CLOSE) && (RightHand == CLOSE))
							{
								if ((MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == L2) || (MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == R2))
								{
									MechanicalHandGroupLibrary[i][j][k][m].time += timezb180;
								}

								else
								{
									MechanicalHandGroupLibrary[i][j][k][m].time +=  timezb90;
								}

							}
							else if ((MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == L1) || (MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == L3) && ((RightHand == CLOSE) && (LeftHand == OPEN)))
							{
								MechanicalHandGroupLibrary[i][j][k][m].time += timezk90;
							}
							else if ((MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == R1) || (MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == R3) && ((RightHand == OPEN) && (LeftHand == CLOSE)))
							{
								MechanicalHandGroupLibrary[i][j][k][m].time +=  timezk90;
							}
							else if ((MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == L1) || (MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == L3) && ((RightHand == OPEN) && (LeftHand == CLOSE)))
							{
								MechanicalHandGroupLibrary[i][j][k][m].time +=  timefz90;
							}
							else if ((MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == R1) || (MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == R3) && ((RightHand == CLOSE) && (LeftHand == OPEN)))
							{
								MechanicalHandGroupLibrary[i][j][k][m].time +=  timefz90;
							}
							else if ((MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == L2) && ((RightHand == OPEN) && (LeftHand == CLOSE)))
							{
								MechanicalHandGroupLibrary[i][j][k][m].time +=  timefz180;
							}
							else if ((MechanicalHandGroupLibrary[i][j][k][m].steps[n].num == R2) && ((RightHand == CLOSE) && (LeftHand == OPEN)))
							{
								MechanicalHandGroupLibrary[i][j][k][m].time += timefz180;
							}
						}
					}
				}
			}
		}
	}
}