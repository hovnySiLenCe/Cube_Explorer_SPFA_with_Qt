#include "CubePoseTransformer.h"


void CubePoseTransformer::Init() {
	poseTransformerLib[0].set(F, R, U, B, L, D, BASE_D, BASE_D, BASE_D, BASE_D, BASE_D, BASE_D);
	poseTransformerLib[1].set(F, U, L, B, D, R, BASE_L, BASE_L, BASE_L, BASE_R, BASE_L, BASE_L);
	poseTransformerLib[2].set(F, L, D, B, R, U, BASE_U, BASE_U, BASE_U, BASE_U, BASE_U, BASE_U);
	poseTransformerLib[3].set(F, D, R, B, U, L, BASE_R, BASE_R, BASE_R, BASE_L, BASE_R, BASE_R);
	poseTransformerLib[4].set(R, F, D, L, B, U, BASE_U, BASE_U, BASE_L, BASE_U, BASE_U, BASE_R);
	poseTransformerLib[5].set(R, U, F, L, D, B, BASE_L, BASE_U, BASE_L, BASE_R, BASE_D, BASE_L);
	poseTransformerLib[6].set(R, B, U, L, F, D, BASE_D, BASE_D, BASE_L, BASE_D, BASE_D, BASE_R);
	poseTransformerLib[7].set(R, D, B, L, U, F, BASE_R, BASE_U, BASE_R, BASE_L, BASE_D, BASE_R);
	poseTransformerLib[8].set(U, F, R, D, B, L, BASE_R, BASE_R, BASE_U, BASE_R, BASE_L, BASE_D);
	poseTransformerLib[9].set(U, R, B, D, L, F, BASE_D, BASE_R, BASE_U, BASE_U, BASE_L, BASE_D);
	poseTransformerLib[10].set(U, B, L, D, F, R, BASE_L, BASE_R, BASE_U, BASE_L, BASE_L, BASE_D);
	poseTransformerLib[11].set(U, L, F, D, R, B, BASE_U, BASE_R, BASE_U, BASE_D, BASE_L, BASE_D);
	poseTransformerLib[12].set(B, R, D, F, L, U, BASE_U, BASE_U, BASE_D, BASE_U, BASE_U, BASE_D);
	poseTransformerLib[13].set(B, U, R, F, D, L, BASE_L, BASE_R, BASE_L, BASE_R, BASE_R, BASE_L);
	poseTransformerLib[14].set(B, L, U, F, R, D, BASE_D, BASE_D, BASE_U, BASE_D, BASE_D, BASE_U);
	poseTransformerLib[15].set(B, D, L, F, U, R, BASE_R, BASE_L, BASE_R, BASE_L, BASE_L, BASE_R);
	poseTransformerLib[16].set(L, F, U, R, B, D, BASE_D, BASE_D, BASE_R, BASE_D, BASE_D, BASE_L);
	poseTransformerLib[17].set(L, U, B, R, D, F, BASE_L, BASE_D, BASE_L, BASE_R, BASE_U, BASE_L);
	poseTransformerLib[18].set(L, B, D, R, F, U, BASE_U, BASE_U, BASE_R, BASE_U, BASE_U, BASE_L);
	poseTransformerLib[19].set(L, D, F, R, U, B, BASE_R, BASE_D, BASE_R, BASE_L, BASE_U, BASE_R);
	poseTransformerLib[20].set(D, F, L, U, B, R, BASE_L, BASE_L, BASE_D, BASE_L, BASE_R, BASE_U);
	poseTransformerLib[21].set(D, R, F, U, L, B, BASE_D, BASE_L, BASE_D, BASE_U, BASE_R, BASE_U);
	poseTransformerLib[22].set(D, B, R, U, F, L, BASE_R, BASE_L, BASE_D, BASE_R, BASE_R, BASE_U);
	poseTransformerLib[23].set(D, L, B, U, R, F, BASE_U, BASE_L, BASE_D, BASE_D, BASE_R, BASE_U);

	int8_t tmp0[9] = { 0,1,2,3,4,5,6,7,8 };
	int8_t tmp1[9] = { 8,7,6,5,4,3,2,1,0 };
	int8_t tmp2[9] = { 2,5,8,1,4,7,0,3,6 };
	int8_t tmp3[9] = { 6,3,0,7,4,1,8,5,2 };
	for (int i = 0; i < 9; i++)faceBaseLib[BASE_D][i] = tmp0[i];
	for (int i = 0; i < 9; i++)faceBaseLib[BASE_U][i] = tmp1[i];
	for (int i = 0; i < 9; i++)faceBaseLib[BASE_L][i] = tmp2[i];
	for (int i = 0; i < 9; i++)faceBaseLib[BASE_R][i] = tmp3[i];

	isInit = true;

	return;
}

cubeModel CubePoseTransformer::generateFromStr(string str) {
	cubeModel ansModel;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 9; j++) {
			int index = 9 * faceKociembaRank[i] + j;
			ansModel.cubeToSolve[i][j] = Face2FaceID[str[index]];
		}
	}
	for (int i = 0; i < 6; i++) {
		ansModel.faceInfo[i] = i;
	}
	return ansModel;
}

cubeModel CubePoseTransformer::faceMapping(cubeModel oldModel, int8_t targetPosecode) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 9; j++) {
			oldModel.cubeToSolve[i][j] = poseTransformerLib[targetPosecode].FaceMapList[oldModel.cubeToSolve[i][j]];
		}
	}
	for (int i = 0; i < 6; i++) {
		oldModel.faceInfo[i] = poseTransformerLib[targetPosecode].FaceMapList[oldModel.faceInfo[i]];
	}
	return oldModel;
}

cubeModel CubePoseTransformer::faceRotate(cubeModel oldModel, int8_t targetPosecode) {
	for (int i = 0; i < 6; i++) {
		int8_t faceAfterRotate[9];

		for (int j = 0; j < 9; j++) {
			int8_t tmp0 = poseTransformerLib[targetPosecode].FaceBaseList[i];
			int8_t tmp1 = faceBaseLib[tmp0][j];
			faceAfterRotate[j] = oldModel.cubeToSolve[i][tmp1];
		}
		for (int j = 0; j < 9; j++) {
			oldModel.cubeToSolve[i][j] = faceAfterRotate[j];
		}
	}
	return oldModel;
}

string CubePoseTransformer::generateFromCubeModel(cubeModel model) {
	string ans_str = "";
	for (int i = 0; i < 6; i++) {
		int8_t targetModelFaceIndex;
		for (int j = 0; j < 6; j++) {
			if (faceKociembaRank[i] == model.faceInfo[j]) {
				targetModelFaceIndex = j;
				break;
			}
		}
		for (int j = 0; j < 9; j++) {
			ans_str = ans_str + faceLib[model.cubeToSolve[targetModelFaceIndex][j]];
		}
	}
	return ans_str;
}

string CubePoseTransformer::Transform(string str, int8_t targetPosecode) {
	if (!isInit)Init();
	cubeModel model;
	string ans_str;
	model = generateFromStr(str);
	model = faceMapping(model, targetPosecode);
	model = faceRotate(model, targetPosecode);
	ans_str = generateFromCubeModel(model);
	return ans_str;
}