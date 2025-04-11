#include"CubeExplorer1.h"

vector<string>& CubeExplorer1::GetVecStrSerial() { return vecStrSerial; }

void CubeExplorer1::init() {
	MechanicalStepint();
	Rotint();
	Pointint();
	Libraryint();
	Librarytimeint();
}

void CubeExplorer1::Reset() {
	vecStrSerial.clear();
	vecStrSerial.push_back("#0P6T200\r\n");
	//vecStrSerial.push_back("#4P1T200\r\n");
	//vecStrSerial.push_back("#5P0T200\r\n");	// clamp_open
}

void CubeExplorer1::SetTarget(string str) {
	target = "";
	int len = str.length();
	for (int i = 0; i < len; i++) {
		if (str[i] == ' ') {
			switch (str[i - 1]) {
			case '\'': target += str[i - 2] + "3 "; break;
			case '2': target += str[i - 2] + "2 "; break;
			default: target += str[i - 1] + "1 "; break;
			}
		}
	}
	switch (str[len - 1]) {
	case '\'': target += str[len - 2] + "3"; break;
	case '2': target += str[len - 2] + "2"; break;
	default: target += str[len - 1] + "1"; break;
	}
}

//void CubeExplorer1::SetTarget(string str) {
//	char old;
////	target = str;
//	old = '2';
//	int j = 0;
//	target = "";
//	for (int i = 0; i < str.length(); i++) {
//		if ((str[i] == ' ') && (old != '2') && (old != '\'')) {
//			target += "1 ";
//		}
//		else if (str[i] == '\'') {
//			target += '3';
//		}
//		else {
//			target += str[i];
//		}
//		old = str[i];
//	}
//	//target = str;
//}

void CubeExplorer1::GetShortestWay() {
	int start, end;

	//start = GetTickCount64();
	int TheoryStrLength = (target.length() - 1) / 3 + 1;
	a_TheoryStrStep[0] = TheoryStrLength;
	a_TheoryStrStep[1] = 0;

	for (int i = 0; i < TheoryStrLength; i++)
	{
		for (int j = 0; j < 3; j++) {
			a_TheoryStep[i].face.a[j][0] = P_FRUBLD[Charint(target[i * 3])].a[j][0];
		}
		a_TheoryStep[i].distance = target[i * 3 + 1] - 0x30 - 1;
	}

	for (int i = 0; i < a_TheoryStrStep[0]; i++) {
		a_TheoryStep2[0][i] = a_TheoryStep[i];
	}

	for (int i = 0; i < a_TheoryStrStep[1]; i++)
	{
		a_TheoryStep2[1][i] = a_TheoryStep[i + a_TheoryStrStep[0]];
	}

	Bookint();
	a_HandState.Set(0, 0, 0, 0);
	a_CubeRot.Set(0, 1, 1, 1, 2, 1);
	a_time[0] = 0;
	a_time[1] = 0;
	b_time[0] = 1000000;
	b_time[1] = 1000000;
	a_StepNum[0] = 0;
	a_StepNum[1] = 0;
	b_StepNum[0] = 1000;
	b_StepNum[1] = 1000;

	for (int i = 0; i < 120; i++)
	{
		a_Move[0][i] = -1;
		a_Move[1][i] = -1;
		b_Move[0][i] = -1;
		b_Move[1][i] = -1;
	}

	//cout << "开始搜索" << endl;

	DFS(0, 0);
	a_CubeRot = b_Rot[0];
	a_HandState = b_HandState[0];
	DFS(0, 1);

	//cout << "搜索结束" << endl;
	//cout << "步骤数：" << b_StepNum[0] + b_StepNum[1] << endl;
	//cout << "执行时间：" << b_time[0] + b_time[1] << endl;
	SaveMechanicalStep();
	//cout << "保存成功" << endl;

	//cout << endl << target << endl;
	//cout << b_StepNum[0] + b_StepNum[1] << " " << b_time[0] + b_time[1] << endl;
	//cout << "Answer" << endl;
	for (int i = 0; i < b_StepNum[0]; i++)
	{
		SloveString += MechanicalMoveStr[b_Move[0][i]]+" ";
	}

	slove_count = b_StepNum[0];
	//for (int i = 0; i < b_StepNum[1]; i++)
	//{
	//	SloveString += MechanicalMoveStr[b_Move[1][i]]+" ";
	//}
	//cout << endl;
	//for (int i = 0; i < b_StepNum[0]; i++)
	//{
	//	cout << Output[output[i]];
	//}
	//cout << endl;
	//cout << b_HandState << endl;
	//end = GetTickCount64();
	//std::cout << "搜索用时" << end - start << "ms" << endl;

}

void CubeExplorer1::SaveMechanicalStep(void)
{
	ofstream in;
	in.open(".\\mechanical\\MechanicalStep.txt", ios::trunc);

	for (int i = 0; i < b_StepNum[0]; i++)
	{
		in << MechanicalMoveStr[b_Move[0][i]] << "";
	}
	for (int i = 0; i < b_StepNum[0]; i++)
	{
		int k = 0;
		int j = 0;
		if (MechanicalMoveStr[b_Move[0][i]] == "M_LO")
		{
			k = i;
			output[i] = 1;
			k++;
			for (int m = 0; m < 20; m++)
			{
				if (MechanicalMoveStr[b_Move[0][k]] == "M_L3")
				{
					output[k] = 16;
					k++;
				}
				else if (MechanicalMoveStr[b_Move[0][k]] == "M_L1")
				{
					output[k] = 17;
					k++;
				}
				else if (MechanicalMoveStr[b_Move[0][k]] == "M_R3")
				{
					output[k] = 18;
					k++;
				}
				else if (MechanicalMoveStr[b_Move[0][k]] == "M_R2")
				{
					output[k] = 19;
					k++;
				}
				else if (MechanicalMoveStr[b_Move[0][k]] == "M_R1")
				{
					output[k] = 20;
					k++;
				}
				else if (MechanicalMoveStr[b_Move[0][k]] == "M_LC")
				{
					output[k] = 2;
					k++;
					break;
				}
			}
			i = k - 1;
		}
		else if (MechanicalMoveStr[b_Move[0][i]] == "M_RO")
		{
			j = i;
			output[i] = 3;
			j++;
			for (int m = 0; m < 20; m++)
			{
				if (MechanicalMoveStr[b_Move[0][j]] == "M_L3")
				{
					output[j] = 11;
					j++;
				}
				else if (MechanicalMoveStr[b_Move[0][j]] == "M_L2")
				{
					output[j] = 12;
					j++;
				}
				else if (MechanicalMoveStr[b_Move[0][j]] == "M_L1")
				{
					output[j] = 13;
					j++;
				}
				else if (MechanicalMoveStr[b_Move[0][j]] == "M_R3")
				{
					output[j] = 14;
					j++;
				}
				else if (MechanicalMoveStr[b_Move[0][j]] == "M_R1")
				{
					output[j] = 15;
					j++;
				}
				else if (MechanicalMoveStr[b_Move[0][j]] == "M_RC")
				{
					output[j] = 4;
					j++;
					break;
				}
			}
			i = j - 1;
		}
		else if (MechanicalMoveStr[b_Move[0][i]] == "M_L2")
		{
			output[i] = 5;
			//i++;
		}
		else if (MechanicalMoveStr[b_Move[0][i]] == "M_R2")
		{
			output[i] = 6;
			//i++;
		}
		else if (MechanicalMoveStr[b_Move[0][i]] == "M_L3")
		{
			output[i] = 7;
			//i++;
		}
		else if (MechanicalMoveStr[b_Move[0][i]] == "M_L1")
		{
			output[i] = 8;
			//i++;
		}
		else if (MechanicalMoveStr[b_Move[0][i]] == "M_R3")
		{
			output[i] = 9;
			//i++;
		}
		else if (MechanicalMoveStr[b_Move[0][i]] == "M_R1")
		{
			output[i] = 10;
			//i++;
		}
	}

	in.close();

	in.open(".\\mechanical\\MechanicalFlag.txt", ios::trunc);
	in.close();

	in.open(".\\mechanical\\output.txt", ios::trunc);
	for (int i = 0; i < b_StepNum[0]; i++)
	{
		in << Output[output[i]] << " ";
		vecStrSerial.push_back(Output[output[i]]);
	}
	in.close();
}


void CubeExplorer1::Bookint(void)//book[25][2][3][3][2][3][2][3][2];
{
	for (int i = 0; i < 25; i++)
	{
		for (int state = 0; state < 2; state++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					for (int l = 0; l < 2; l++)
					{
						for (int m = 0; m < 3; m++)
						{
							for (int n = 0; n < 2; n++)
							{
								for (int u = 0; u < 3; u++)
								{
									for (int v = 0; v < 2; v++)
									{
										book[i][state][j][k][l][m][n][u][v] = 1000000;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}


int CubeExplorer1::Charint(char inChar)
{
	switch (inChar) {
	case'F':return F; break;
	case'B':return B; break;
	case'R':return R; break;
	case'L':return L; break;
	case'U':return U; break;
	case'D':return D; break;
	default:return -1;
	}
	return -1;
}


void CubeExplorer1::DFS(int step, int state)
{
	if (step == a_TheoryStrStep[state])
	{
		if (a_time[state] < b_time[state])
		{
			b_time[state] = a_time[state];
			b_StepNum[state] = a_StepNum[state];


			for (int i = 0; i < 120; i++)
			{
				b_Move[state][i] = a_Move[state][i];
				if (a_Move[state][i] == -1)
				{
					break;
				}
			}
			b_Rot[state] = a_CubeRot;
			b_HandState[state] = a_HandState;

			//cout << "state:" << state << "step:" << a_StepNum[state] << "time:" << b_time[state];
			//cout << "handstate:" << a_HandState.NoLeftNice << a_HandState.NoRightNice << endl;

		}
		return;
	}


	int face;
	int m;
	for (m = 0; m < 3; ++m)
	{
		if (a_TheoryStep2[state][step].face.a[m][0] != 0)
		{
			break;
		}
	}

	int n;
	for (n = 0; n < 3; ++n)
	{
		if (a_CubeRot.a[n][m] != 0)
		{
			break;
		}
	}

	if (n == 0)
	{
		if (a_CubeRot.a[n][m] == a_TheoryStep2[state][step].face.a[m][0])
		{
			face = F;
		}
		else
		{
			face = B;
		}
	}
	if (n == 1)
	{
		if (a_CubeRot.a[n][m] == a_TheoryStep2[state][step].face.a[m][0])
		{
			face = R;
		}
		else
		{
			face = L;
		}
	}
	if (n == 2)
	{
		if (a_CubeRot.a[n][m] == a_TheoryStep2[state][step].face.a[m][0])
		{
			face = U;
		}
		else
		{
			face = D;
		}
	}

	int i = a_TheoryStep2[state][step].distance;
	int j = a_HandState.NoLeftNice * 2 + a_HandState.NoRightNice;

	for (int k = 0; k < 16; k++)
	{
		Rot temprot = a_CubeRot;
		HandState1 tempstate = a_HandState;

		int tempStepNum = a_StepNum[state];
		int temptime = a_time[state];
		int tempmove[120];

		for (int p = 0; p < 120; p++)
		{
			tempmove[p] = a_Move[state][p];
			if (a_Move[state][p] == -1)
			{
				break;
			}
		}

		a_time[state] += MechanicalHandGroupLibrary[face][i][j][k].time;
		a_CubeRot = Rotmt(MechanicalHandGroupLibrary[face][i][j][k].rot, a_CubeRot);
		a_HandState = MechanicalHandGroupLibrary[face][i][j][k].FinalHandState;

		for (int q = 0; q < MechanicalHandGroupLibrary[face][i][j][k].StepNum; q++)
		{
			a_Move[state][a_StepNum[state] + q] = MechanicalHandGroupLibrary[face][i][j][k].steps[q].num;
		}

		a_StepNum[state] += MechanicalHandGroupLibrary[face][i][j][k].StepNum;

		int row[3], num[3];
		for (int u = 0; u < 3; u++)
		{
			for (row[u] = 0; row[u] < 3; row[u]++)
			{
				if (a_CubeRot.a[row[u]][u] != 0)
				{
					if (a_CubeRot.a[row[u]][u] == -1)
					{
						num[u] = 0;
					}
					else if (a_CubeRot.a[row[u]][u] == 1)
					{
						num[u] = 1;
					}
					else
					{
						num[u] = -1;
					}
					break;
				}
			}
		}

		int hand = a_HandState.NoLeftNice * 2 + a_HandState.NoRightNice;

		if (a_time[state] < book[step][state][hand][row[0]][num[0]][row[1]][num[1]][row[2]][num[2]])
		{
			book[step][state][hand][row[0]][num[0]][row[1]][num[1]][row[2]][num[2]] = a_time[state];

			DFS(step + 1, state);
			a_CubeRot = temprot;
			a_HandState = tempstate;
			a_time[state] = temptime;
			a_StepNum[state] = tempStepNum;

			for (int w = 0; w < 120; w++)
			{
				a_Move[state][w] = tempmove[w];
				if (tempmove[w] == -1)
				{
					//cout << "     " << endl;
					break;
				}
				//cout << MechanicalMoveStr[a_Move[state][w]] << "";
			}
		}

		else
		{
			a_CubeRot = temprot;
			a_HandState = tempstate;
			a_time[state] = temptime;
			a_StepNum[state] = tempStepNum;

			for (int x = 0; x < 120; x++)
			{
				a_Move[state][x] = tempmove[x];
				if (tempmove[x] == -1)
				{
					//cout << "     " << endl;
					break;
				}
				//cout << MechanicalMoveStr[a_Move[state][x]] << "";
			}

		}

	}

}


// to do
void CubeExplorerSPFA::InitOrientation() {
	eOp.cubeState.orien = Ori_Vector(0, 0, 1);
	eOp.cubeState.angle = Ori_Vector(1, 0, 0);
	eOp.cubeState.GetId();

	// opOrien[MAX_HAND][MAX_ROTATION]
	opOrien[1][0] = Ori_Vector(0, 0, -1); // R'
	opOrien[1][1] = Ori_Vector(0, 0, 1); // R
	opOrien[2][0] = Ori_Vector(-1, 0, 0); // L'
	opOrien[2][1] = Ori_Vector(1, 0, 0); // L

	// record the face toward the hand of each state
	opCheckHand[0 * 6 + 2][0] = FACE_B_ID, opCheckHand[0 * 6 + 2][1] = FACE_L_ID;
	opCheckHand[0 * 6 + 3][0] = FACE_B_ID, opCheckHand[0 * 6 + 3][1] = FACE_R_ID;
	opCheckHand[0 * 6 + 4][0] = FACE_B_ID, opCheckHand[0 * 6 + 4][1] = FACE_D_ID;
	opCheckHand[0 * 6 + 5][0] = FACE_B_ID, opCheckHand[0 * 6 + 5][1] = FACE_U_ID;

	opCheckHand[1 * 6 + 2][0] = FACE_F_ID, opCheckHand[1 * 6 + 2][1] = FACE_R_ID;
	opCheckHand[1 * 6 + 3][0] = FACE_F_ID, opCheckHand[1 * 6 + 3][1] = FACE_L_ID;
	opCheckHand[1 * 6 + 4][0] = FACE_F_ID, opCheckHand[1 * 6 + 4][1] = FACE_D_ID;
	opCheckHand[1 * 6 + 5][0] = FACE_F_ID, opCheckHand[1 * 6 + 5][1] = FACE_U_ID;


	opCheckHand[2 * 6 + 0][0] = FACE_D_ID, opCheckHand[2 * 6 + 0][1] = FACE_R_ID;
	opCheckHand[2 * 6 + 1][0] = FACE_U_ID, opCheckHand[2 * 6 + 1][1] = FACE_L_ID;
	opCheckHand[2 * 6 + 4][0] = FACE_L_ID, opCheckHand[2 * 6 + 4][1] = FACE_D_ID;
	opCheckHand[2 * 6 + 5][0] = FACE_R_ID, opCheckHand[2 * 6 + 5][1] = FACE_U_ID;

	opCheckHand[3 * 6 + 0][0] = FACE_D_ID, opCheckHand[3 * 6 + 0][1] = FACE_L_ID;
	opCheckHand[3 * 6 + 1][0] = FACE_U_ID, opCheckHand[3 * 6 + 1][1] = FACE_R_ID;
	opCheckHand[3 * 6 + 4][0] = FACE_R_ID, opCheckHand[3 * 6 + 4][1] = FACE_D_ID;
	opCheckHand[3 * 6 + 5][0] = FACE_L_ID, opCheckHand[3 * 6 + 5][1] = FACE_U_ID;


	opCheckHand[4 * 6 + 0][0] = FACE_D_ID, opCheckHand[4 * 6 + 0][1] = FACE_B_ID;
	opCheckHand[4 * 6 + 1][0] = FACE_U_ID, opCheckHand[4 * 6 + 1][1] = FACE_B_ID;
	opCheckHand[4 * 6 + 2][0] = FACE_R_ID, opCheckHand[4 * 6 + 2][1] = FACE_B_ID;
	opCheckHand[4 * 6 + 3][0] = FACE_L_ID, opCheckHand[4 * 6 + 3][1] = FACE_B_ID;

	opCheckHand[5 * 6 + 0][0] = FACE_D_ID, opCheckHand[5 * 6 + 0][1] = FACE_F_ID;
	opCheckHand[5 * 6 + 1][0] = FACE_U_ID, opCheckHand[5 * 6 + 1][1] = FACE_F_ID;
	opCheckHand[5 * 6 + 2][0] = FACE_L_ID, opCheckHand[5 * 6 + 2][1] = FACE_F_ID;
	opCheckHand[5 * 6 + 3][0] = FACE_R_ID, opCheckHand[5 * 6 + 3][1] = FACE_F_ID;

	freopen("..//opChechHand.out", "w", stdout);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			printf("cubeId = (%d, %d) Lhand = %d Rhand = %d\n", i, j, opCheckHand[i * 6 + j][0], opCheckHand[i * 6 + j][1]);
		}
		printf("\n");
	}
	fclose(stdout);
	return;
}

string CubeExplorerSPFA::GetAnsOpSequence()
{
	return ansOpSequence;
}

int CubeExplorerSPFA::GetFaceId(char c) {
	switch (c) {
	case 'F':return FACE_F_ID;
	case 'U':return FACE_U_ID;
	case 'R':return FACE_R_ID;
	case 'L':return FACE_L_ID;
	case 'B':return FACE_B_ID;
	case 'D':return FACE_D_ID;
	default:return -1;
	}
}

void CubeExplorerSPFA::SetTarget(string str) {
	targetStep = 0;
	int len = str.length(), faceId;
	for (int i = 0; i < len; i++) {
		if (str[i] == ' ') {
			char c;
			switch (str[i - 1]) {
			case '\'': faceId = GetFaceId(c=str[i - 2]), target[targetStep] = Cube_Operation(faceId, '3'); break;
			case '2': faceId = GetFaceId(c=str[i - 2]), target[targetStep] = Cube_Operation(faceId, '2'); break;
			default: faceId = GetFaceId(c=str[i - 1]), target[targetStep] = Cube_Operation(faceId, '1'); break;
			};++targetStep;
		}
	}
}

inline int CubeExplorerSPFA::CheckFace(int cubeStateId, int face) {
	//printf("cubeStateId = %d LHand = %d RHand = %d face = %d\n", cubeStateId, opCheckHand[cubeStateId][0], opCheckHand[cubeStateId][1], face);
	if (opCheckHand[cubeStateId][0] == face) return 2; // left hand is the face required
	if (opCheckHand[cubeStateId][1] == face) return 1; // right hand is the face required
	return 0;
}

inline bool CubeExplorerSPFA::StateUpdate(int costTime, int step, int cubeStateId, int handState, string curOpSequence) {
	//printf("costTime = %d step = %d cubeStateId = %d handState = %d\n", costTime, step, cubeStateId, handState);
	if (costTime < stepTime[step][cubeStateId][handState]) {
		stepTime[step][cubeStateId][handState] = costTime;
		opSequence[step][cubeStateId][handState] = curOpSequence;
		if (!vis[step][cubeStateId][handState]) {
			vis[step][cubeStateId][handState] = 1;
			return 1;
		}
	}return 0;
}

void CubeExplorerSPFA::SPFA() {
	//printf("targetStep = %d\n", targetStep);
	Step_State currentOp, nextOp;
	Cube_State cubeState;
	int step, costTime, handId;
	int newHandState, handState;
	string curOpSequence;
	queue<Step_State> p; ansTime = INF;
	memset(stepTime, INF, sizeof(stepTime));
	opSequence[0][eOp.cubeState.id][0] = "";
	stepTime[0][eOp.cubeState.id][0] = 0;
	vis[0][eOp.cubeState.id][0] = 1;
	p.push(eOp);
	while (!p.empty()) {
		currentOp = p.front(), p.pop();

		step = currentOp.step;
		cubeState = currentOp.cubeState;
		handState = currentOp.handState;
		vis[step][cubeState.id][handState] = 0;

		curOpSequence = opSequence[step][cubeState.id][handState];

		/*currentOp.Out(), printf("%s\n", curOpSequence.c_str());
		printf("son:\n");*/

		if (stepTime[step][cubeState.id][handState] > ansTime) continue;
		if (step == targetStep) {
			if (ansTime > stepTime[step][cubeState.id][handState]) {
				ansTime = stepTime[step][cubeState.id][handState];
				ansOpSequence = curOpSequence;
			}
			continue;
		}
		if (handState >> 2) { 
			// check the hands are open or not
			// if there is one hand open
			// 1. we can close the hands
			int newHandState;
			handId = handState >> 2;
			Cube_State newCubeState;
			costTime = stepTime[step][cubeState.id][handState] + HAND_CLOSE;
			newHandState = handState & 3;
			if (StateUpdate(costTime, step, cubeState.id, newHandState, curOpSequence + opHandId[handId] + "C "))
				p.push(Step_State(step, cubeState, newHandState));

			//2. we can change the orientation of the cube,
			
			// 2.1 the change angle can be 90
			handId = (handState >> 2) ^ 3; // get the id of the hand closed
			costTime = stepTime[step][cubeState.id][handState] + HAND_TURN_CUBE_90;
			//printf("why costTime? step=%d cubeState.id = %d handState = %d stepTime = %d\n", step, cubeState.id, handState, stepTime[step][cubeState.id][handState]);
			newHandState = handState ^ (handId ^ 3);
			for (int i = 0; i < 2; i++) { //the turning orientation can be clockwise as well as anticlockwise
				newCubeState = cubeState * opOrien[handId][i]; // determined in InitOrientation()
				if(StateUpdate(costTime, step, newCubeState.id, newHandState, curOpSequence + opHandId[handId] + opOrienId[i] + char(HAND_TURN_CUBE_90)))
					p.push(Step_State(step, newCubeState, newHandState));
			}

			//2.2 also can be 180, the hand with cube is only meaningful.
			costTime = stepTime[step][cubeState.id][handState] + HAND_TURN_CUBE_180;
			newCubeState = cubeState * opOrien[handId][0] * opOrien[handId][0];
			if(StateUpdate(costTime, step, newCubeState.id, handState, curOpSequence + opHandId[handId] + '2' + char(HAND_TURN_CUBE_180)))
				p.push(Step_State(step, newCubeState, handState));
		}
		else {// if the hands are all close
			if (handId = CheckFace(cubeState.id, target[step].face)) { // 判断面是否符合
				//printf("handId = %c targetFace = %d\n", (handId >> 1) ? 'L' : 'R', target[step].face);
				// 爪向变化 
				int epsTime;
				if (target[step].dire != '2') {
					epsTime = HAND_TWIST_CUBE_90;
					newHandState = handState ^ handId;
				}
				else {
					epsTime = HAND_TWIST_CUBE_180;
					newHandState = handState;
				}
				costTime = stepTime[step][cubeState.id][handState] + epsTime;
				if (StateUpdate(costTime, step + 1, cubeState.id, newHandState, curOpSequence + opHandId[handId] + target[step].dire + char(epsTime)))
					p.push(Step_State(step + 1, cubeState, newHandState));
			}
			// the face isn't required and all hands are close.
			costTime = stepTime[step][cubeState.id][handState] + HAND_OPEN;
			for (int i = 0; i < 2; i++) {
				newHandState = handState + (4 << i);
				if (StateUpdate(costTime, step, cubeState.id, newHandState, curOpSequence + ((i) ? "LO " : "RO "))) {
					p.push(Step_State(step, cubeState, newHandState));
				}
			}
		}
		//printf("end of son.\n\n");
	}
}

void CubeExplorerSPFA::SaveMechanicalStep()
{
	ofstream in;
	int len = ansOpSequence.length();

	in.open(".\\Mechanical\\MechanicalStep.txt", ios::trunc);

	in << ansOpSequence << endl;
	in << "total step: " << len / 3 << endl;

	in.close();

	in.open(".\\Mechanical\\ControlCommand.txt", ios::trunc);

	int opId;
	for (int i = 0; i < len; i+=3) {
		in << ansOpSequence[i] << ansOpSequence[i + 1] << " ";
		char op = ansOpSequence[i + 1];
		opId = 0;
		if (isdigit(ansOpSequence[i + 1])) {
			opId = 1;
		}
		else {
			switch (ansOpSequence[i + 1]) {
			case 'O':opId = 0; break;
			case 'C':opId = 1; break;
			}
			switch (ansOpSequence[i]) {
			case 'L':opId += 1; break;
			case 'R':opId += 3; break;
			}
		}
		vecStrSerial.push_back(commandOp[opId]);
	}
	
	in.close();

	//in.open(".\\Mechanical\\MechanicalFlag.txt", ios::trunc);
	//in.close();
}

void CubeExplorerSPFA::GetShortestPath() {
	//freopen("..//runtime.out", "w", stdout);
	//for (int i = 0; i < 6; i++) {
	//	for (int j = 0; j < 6; j++) {
	//		printf("cubeId = (%d, %d) Lhand = %d Rhand = %d\n", i, j, opCheckHand[i * 6 + j][0], opCheckHand[i * 6 + j][1]);
	//	}
	//	printf("\n");
	//}
		
	SPFA();
	//printf("SPFA done.\n");
	//std::cout << ansOpSequence <<endl;
	SaveMechanicalStep();
	//fclose(stdout);
}