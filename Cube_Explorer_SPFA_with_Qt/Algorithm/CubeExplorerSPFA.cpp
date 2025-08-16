#include"CubeExplorerSPFA.h"

void CubeExplorerSPFA::InitOrientation() {
	// Ori_Vector cubeVector;
	/*eOp.cubeState.orien = Ori_Vector(0, 0, 1);
	eOp.cubeState.angle = Ori_Vector(1, 0, 0);*/
	eOp.cubeState.orien = Ori_Vector(1, 0, 0);
	eOp.cubeState.angle = Ori_Vector(0, -1, 0);
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

#ifdef DEBUG
	freopen("..//opChechHand.out", "w", stdout);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			printf("cubeId = (%d, %d) Lhand = %d Rhand = %d\n", i, j, opCheckHand[i * 6 + j][0], opCheckHand[i * 6 + j][1]);
		}
		printf("\n");
	}
	fclose(stdout);
#endif // DEBUG

	return;
}

void CubeExplorerSPFA::SetAnsOpSequence(string str)
{
	ansOpSequence = str;
}

string CubeExplorerSPFA::GetAnsOpSequence()
{
	return ansOpSequence;
}


string CubeExplorerSPFA::GetAnsOpSequenceFormat()
{
	std::string tmp = "";
	for (int i = 0; i < ansOpSequence.length(); i+=3) {
		tmp += ansOpSequence.substr(i, 2) + " ";
		if (i / 3 % 15 == 14) {
			tmp += "\n";
		}
	}
	return tmp;
}

int CubeExplorerSPFA::GetAnsOpStepNumber()
{
	return ansOpSequence.length()/3;
}

int CubeExplorerSPFA::GetTargetStepNumber()
{
	return targetStep;
}

int CubeExplorerSPFA::GetAnsCostTime()
{
	return ansTime;
}

vector<string>& CubeExplorerSPFA::GetVecStrSerial()
{
	return vecStrSerial;
}

inline int CubeExplorerSPFA::GetFaceId(char c) {
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
	/*for (int i = 0; i < len; i += 2)
		target[targetStep++] = Cube_Operation(GetFaceId(str[i]), str[i + 1]);*/
	for (int i = 0; i < len; i++) {
		if (str[i] == ' ') {
			char c;
			switch (str[i - 1]) {
			case '\'': faceId = GetFaceId(c = str[i - 2]), target[targetStep] = Cube_Operation(faceId, '3'); break;
			case '2': faceId = GetFaceId(c = str[i - 2]), target[targetStep] = Cube_Operation(faceId, '2'); break;
			default: faceId = GetFaceId(c = str[i - 1]), target[targetStep] = Cube_Operation(faceId, '1'); break;
			}; ++targetStep;
		}
	}
}

inline int CubeExplorerSPFA::CheckFace(int cubeStateId, int face) {
	//printf("cubeStateId = %d LHand = %d RHand = %d face = %d\n", cubeStateId, opCheckHand[cubeStateId][0], opCheckHand[cubeStateId][1], face);
	if (opCheckHand[cubeStateId][0] == face) return 2; // left hand is the face required
	if (opCheckHand[cubeStateId][1] == face) return 1; // right hand is the face required
	return 0;
}

inline string ExeState(int cubeStateId, int handState) {
	string s = "";
	s = s + char('0' + cubeStateId / 6)+ "/" + char('0' + cubeStateId % 6) + " ";
	for (int i = 3; i >= 0; i--)
		s = s + char('0' + ((handState >> i) & 1));
	return s;
}

inline bool CubeExplorerSPFA::StateUpdate(int costTime, int step, int cubeStateId, int handState, Node_List* preNode, int opId) {
	//printf("costTime = %d step = %d cubeStateId = %d handState = %d\n", costTime, step, cubeStateId, handState);
	//printf("costTime = %d step = %d state = ", costTime, step); std::cout << ExeState(cubeStateId, handState) <<endl;
	if (costTime < stepTime[step][cubeStateId][handState]) {
		stepTime[step][cubeStateId][handState] = costTime;
		pathList[step][cubeStateId][handState] = Node_List(opId+1, preNode);
		//qDebug("step = %d cubeStateId = %d handState = %d opId = %d", step, cubeStateId, handState, opId);
		//opSequence[step][cubeStateId][handState] = curOpSequence;
		if (!vis[step][cubeStateId][handState]) {
			vis[step][cubeStateId][handState] = 1;
			return 1;
		}
	}return 0;
}

void CubeExplorerSPFA::SPFA() {
	//printf("targetStep = %d\n", targetStep);
	Step_State currentOp, nextOp;
	Cube_State cubeState, newCubeState;
	int step, costTime, handId, i;
	int newHandState, handState;
	char preOpHand, preOpOrien;
	Node_List* curNodePoint;
	//string curOpSequence;
	queue<Step_State> p;

	ansTime = INF;
	memset(stepTime, INF, sizeof(stepTime));

	memset(pathList, 0, sizeof(pathList));

	/*0815
	opSequence[0][eOp.cubeState.id][0] = "";
	*/
	pathList[0][eOp.cubeState.id][0].opId = 0;
	stepTime[0][eOp.cubeState.id][0] = 0;
	vis[0][eOp.cubeState.id][0] = 1;
	p.push(eOp);
	while (!p.empty()) {
		currentOp = p.front(), p.pop();

		step = currentOp.step;
		cubeState = currentOp.cubeState;
		handState = currentOp.handState;
		vis[step][cubeState.id][handState] = 0;

		curNodePoint = &pathList[step][cubeState.id][handState];
		/* 0815
		curOpSequence = opSequence[step][cubeState.id][handState];
		*/

		//curOpSequence = opSequence[step][cubeState.id][handState]+ExeState(cubeState.id,handState)+'\n';

		//currentOp.Out()/*, printf("%s\n", curOpSequence.c_str())*/;
		//printf("costTime = %d step = %d state = ", stepTime[step][cubeState.id][handState], step); std::cout << ExeState(cubeState.id, handState) << endl;
		//printf("son:\n");

		if (stepTime[step][cubeState.id][handState] > ansTime) continue;
		if (step == targetStep) {
			if (ansTime > stepTime[step][cubeState.id][handState]) {
				ansTime = stepTime[step][cubeState.id][handState];
				ansPathEnd = &pathList[step][cubeState.id][handState];
				//qDebug("ansTime = %d step = %d cubeState = %d handState = %d", ansTime, step, cubeState.id, handState);
			}
			continue;
		}
		if (handState >> 2) {
			// check the hands are open or not
			// 1. if there is one hand open
			// 1.1 we can close the hands

			/*int newHandState;
			Cube_State newCubeState;*/

			handId = handState >> 2;
			costTime = stepTime[step][cubeState.id][handState] + HAND_CLOSE;
			newHandState = handState & 3;
			if (StateUpdate(costTime, step, cubeState.id, newHandState, curNodePoint, (2 - handId) * MAX_OPS_LEN + OP_C_ID))
				p.push(Step_State(step, cubeState, newHandState));
			// 1.2 the hand is open, the change angle can be 90
			if (handId + (handState & 3) != 3) {

				costTime = stepTime[step][cubeState.id][handState] + HAND_TURN_CUBE_90;

				/*
				if (reuseFlag) { // check whether current step can be reused ( Two different hands in different orientations, with one hand open and the other closed. )
					preOpHand = curOpSequence[curOpSequence.length() - 3];
					preOpOrien = curOpSequence[curOpSequence.length() - 2];
					if (opHandId[handId] != preOpHand && (preOpOrien == '1' || preOpOrien == '3')) {
						costTime = stepTime[step][cubeState.id][handState];
						preOpOrien = (preOpOrien == '1') ? '3' : '1';
					}
					else preOpOrien = '1';
				}
				*/
				preOpOrien = '1';

				//printf("why costTime? step=%d cubeState.id = %d handState = %d stepTime = %d\n", step, cubeState.id, handState, stepTime[step][cubeState.id][handState]);
				newHandState = handState ^ handId;
				if (StateUpdate(costTime, step, cubeState.id, newHandState, curNodePoint, (2 - handId) * MAX_OPS_LEN + OP_1_ID))
					p.push(Step_State(step, cubeState, newHandState));

				//2.2.2 also can be 180, the hand with cube is only meaningful.
				/*costTime = stepTime[step][cubeState.id][handState] + HAND_TURN_ONLY_180;
				if (StateUpdate(costTime, step, cubeState.id, handState, curOpSequence + opHandId[handId] + '2' + char(HAND_TURN_CUBE_180)))
					p.push(Step_State(step, cubeState, handState));*/
			}


			//2. we can change the orientation of the cube,
			// 2.1 the hand is close, the change angle can be 90
			handId = (handState >> 2) ^ 3; // get the id of the hand closed
			if (handId + (handState & 3) != 3) {

				newHandState = handState ^ handId;

				for (i = 0; i < 2; i++) { //the turning orientation can be clockwise as well as anticlockwise
					newCubeState = cubeState * opOrien[handId][i]; // determined in InitOrientation()
					costTime = stepTime[step][cubeState.id][handState] + HAND_TURN_CUBE_90;
					
					/*
					if (reuseFlag) {
						preOpHand = curOpSequence[curOpSequence.length() - 3];
						preOpOrien = curOpSequence[curOpSequence.length() - 2];
						if (opHandId[handId] != preOpHand && preOpOrien != opOrienId[i] && (preOpOrien == '1' || preOpOrien == '3')) {
							costTime = stepTime[step][cubeState.id][handState];
						}
					}
					*/

					if (StateUpdate(costTime, step, newCubeState.id, newHandState, curNodePoint, (2 - handId) * MAX_OPS_LEN + (1 - i) * OP_3_ID))
						p.push(Step_State(step, newCubeState, newHandState));
				}

				//2.2 also can be 180, the hand with cube is only meaningful.
				costTime = stepTime[step][cubeState.id][handState] + HAND_TURN_CUBE_180;
				newCubeState = cubeState * opOrien[handId][0] * opOrien[handId][0];
				if (StateUpdate(costTime, step, newCubeState.id, handState, curNodePoint, (2 - handId) * MAX_OPS_LEN + OP_2_ID))
					p.push(Step_State(step, newCubeState, handState));
			}

		}
		else {// if the hands are all close
			if (handId = CheckFace(cubeState.id, target[step].face)) { // 判断面是否符合
				// 爪向变化
				
				if (handId + (handState&3) != 3) {
					//printf("handId = %c handState = %d targetFace = %d\n", (handId >> 1) ? 'L' : 'R', handState % 4, target[step].face);
					//printf("handId = %d handState = %d\n", handId, handState % 4);
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
					if (StateUpdate(costTime, step + 1, cubeState.id, newHandState, curNodePoint, (2 - handId) * MAX_OPS_LEN + (target[step].dire - '1')))
						p.push(Step_State(step + 1, cubeState, newHandState));
				}
			}
			// the face isn't required and all hands are close.
			costTime = stepTime[step][cubeState.id][handState] + HAND_OPEN;
			for (int i = 0; i < 2; i++) {
				newHandState = handState + (4 << i);
				if (StateUpdate(costTime, step, cubeState.id, newHandState, curNodePoint, (1 - i) * MAX_OPS_LEN + OP_O_ID)) {
					p.push(Step_State(step, cubeState, newHandState));
				}
			}
		}
		//printf("end of son.\n\n");
	}

	ansOpSequence = "";
	Node_List* curNode = ansPathEnd;
	int stepNum = 0;
	while (curNode->opId > 0) {
        ansOpSequence = opString[curNode->opId-1] + ansOpSequence;
        curNode = curNode->preNode;
	}
}

void CubeExplorerSPFA::SaveMechanicalStep()
{
	int len = ansOpSequence.length();

#ifdef DEBUG
	ofstream out;
	out.open(".\\Mechanical\\MechanicalStep.txt", ios::trunc);

	out << ansOpSequence << endl;
	out << "total step: " << len / 3 << endl;
	out << "total time: " << ansTime << endl;

	out.close();

	out.open(".\\Mechanical\\ControlCommand.txt", ios::trunc);
#endif // DEBUG

	int opId;
	//int cntTurn[2];
	//memset(cntTurn, 0, sizeof(cntTurn));

	//ifstream in;
	//in.open(".\\Mechanical\\HandTurnState.txt", ios::in);
	//in >> cntTurn[0] >> cntTurn[1];
	//in.close();

	vecStrSerial.clear();
	for (int i = 0; i < len; i += 3) {

		switch (ansOpSequence[i]) {
		case 'L':opId = 0; break;
		case 'R':opId = MAX_OPS_LEN; break;
		}
		switch (ansOpSequence[i + 1]) {
		case '1': opId += OP_1_ID; break;
		case '2': opId += OP_2_ID; break;
		case '3': opId += OP_3_ID; break;
		case 'O': opId += OP_O_ID; break;
		case 'C': opId += OP_C_ID; break;
		}
		//out << cntTurn[0] << " " << cntTurn[1] << " " << ansOpSequence.substr(i, 3) << " " << commandOp[opId];
		vecStrSerial.push_back(commandOp[opId]);
	}

	//ofstream out; out.open(".\\Mechanical\\HandTurnState.txt", ios::trunc);
	//out << cntTurn[0] << " " << cntTurn[1];
	//out.close();

#ifdef DEBUG
	out.close();
#endif // DEBUG

	//in.open(".\\Mechanical\\MechanicalFlag.txt", ios::trunc);
	//in.close();
}

void CubeExplorerSPFA::GetShortestPath(string str) {
	//freopen("..//runtime.out", "w", stdout);
	//for (int i = 0; i < 6; i++) {
	//	for (int j = 0; j < 6; j++) {
	//		printf("cubeId = (%d, %d) Lhand = %d Rhand = %d\n", i, j, opCheckHand[i * 6 + j][0], opCheckHand[i * 6 + j][1]);
	//	}
	//	printf("\n");
	//}
	SetTarget(str);	SPFA();
	//printf("SPFA done.\n");
	//std::cout << ansOpSequence <<endl;
	//fclose(stdout);
}