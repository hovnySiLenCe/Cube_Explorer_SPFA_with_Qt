#ifndef CUBE_POSE_TRANSFORMER
#define CUBE_POSE_TRANSFORMER
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <stack>
#include "AtomSolver.h"
using namespace std;

#define BASE_D 0
#define BASE_U 1
#define BASE_L 2
#define BASE_R 3

//定义各面代数
#define F 0
#define R 1
#define U 2
#define B 3
#define L 4
#define D 5

struct Transformer {
	int8_t FaceMapList[6];
	int8_t FaceBaseList[6];
	void set(int8_t m0, int8_t m1, int8_t m2, int8_t m3, int8_t m4, int8_t m5, int8_t b0, int8_t b1, int8_t b2, int8_t b3, int8_t b4, int8_t b5) {
		FaceMapList[0] = m0;
		FaceMapList[1] = m1;
		FaceMapList[2] = m2;
		FaceMapList[3] = m3;
		FaceMapList[4] = m4;
		FaceMapList[5] = m5;
		FaceBaseList[0] = b0;
		FaceBaseList[1] = b1;
		FaceBaseList[2] = b2;
		FaceBaseList[3] = b3;
		FaceBaseList[4] = b4;
		FaceBaseList[5] = b5;
	}
};
struct cubeModel {
	int8_t cubeToSolve[6][9];
	int8_t faceInfo[6];
};

class CubePoseTransformer {
private:
	Transformer poseTransformerLib[24];
	int8_t faceBaseLib[4][9];
	bool isInit = false;
	const string faceLib = "FRUBLD";
	const int8_t faceKociembaRank[6] = { U,R,F,D,L,B };
	map<char, int>Face2FaceID = { {'F',0}, {'R',1}, {'U',2}, {'B',3}, {'L',4}, {'D',5} };

	cubeModel generateFromStr(string str);
	cubeModel faceMapping(cubeModel oldModel, int8_t targetPosecode);
	cubeModel faceRotate(cubeModel oldModel, int8_t targetPosecode);
	string generateFromCubeModel(cubeModel model);
public:
	CubePoseTransformer() {
		Init();
	}
	void Init();
	string Transform(string str, int8_t targetPosecode);
};
#endif // !CUBE_POSE_TRANSFORMER
