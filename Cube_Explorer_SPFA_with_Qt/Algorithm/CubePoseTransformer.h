#ifndef CUBE_POSE_TRANSFORMER
#define CUBE_POSE_TRANSFORMER
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <stack>
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
	int8_t reTransform[24][6] = {
		{0, 1, 2, 3, 4, 5},
		{0, 5, 1, 3, 2, 4},
		{0, 4, 5, 3, 1, 2},
		{0, 2, 4, 3, 5, 1},
		{1, 0, 5, 4, 3, 2},
		{2, 0, 1, 5, 3, 4},
		{4, 0, 2, 1, 3, 5},
		{5, 0, 4, 2, 3, 1},
		{1, 2, 0, 4, 5, 3},
		{5, 1, 0, 2, 4, 3},
		{4, 5, 0, 1, 2, 3},
		{2, 4, 0, 5, 1, 3},
		{3, 1, 5, 0, 4, 2},
		{3, 2, 1, 0, 5, 4},
		{3, 4, 2, 0, 1, 5},
		{3, 5, 4, 0, 2, 1},
		{1, 3, 2, 4, 0, 5},
		{5, 3, 1, 2, 0, 4},
		{4, 3, 5, 1, 0, 2},
		{2, 3, 4, 5, 0, 1},
		{1, 5, 3, 4, 2, 0},
		{2, 1, 3, 5, 4, 0},
		{4, 2, 3, 1, 5, 0},
		{5, 4, 3, 2, 1, 0} };
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
	string ReTransform(string res, int8_t poseId);
};
#endif // !CUBE_POSE_TRANSFORMER
