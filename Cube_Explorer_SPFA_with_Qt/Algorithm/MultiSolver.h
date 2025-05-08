#pragma once
#include <thread>
#include <mutex>
#include "threadSafeKociemba.h"
#include "CubePoseTransformer.h"
#include "MultiSolver.h"
#include "CubeExplorerSPFA.h"

class MultiSolver {
private:
	threadSafeKociemba* kociembaSolver;
	CubeExplorerSPFA* ansCubeExplorerSPFA;
	CubeExplorerSPFA* cubeExplorerSPFA[24];
	CubePoseTransformer* cubePoseTransformer;
	string cubeStatus;
	int ansTime;
	void GetSinglePosePath(int poseId);
	

public:
	CubeExplorerSPFA* GetMultiThreadPath(string str);
	void InitSolver();
	//ThreadSolver() {
	//	InitSolver();
	//}
};