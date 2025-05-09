#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <QDebug>
#include "threadSafeKociemba.h"
#include "CubePoseTransformer.h"
#include "MultiSolver.h"
#include "CubeExplorerSPFA.h"

class MultiSolver {
private:
	threadSafeKociemba* kociembaSolver[24];
	CubeExplorerSPFA* ansCubeExplorerSPFA;
	CubeExplorerSPFA* cubeExplorerSPFA[24];
	CubePoseTransformer* cubePoseTransformer[24];
	string cubeStatus;
	int ansTime;
	void GetSinglePosePath(int poseId);
	

public:
	CubeExplorerSPFA* GetMultiThreadPath(const string& str, int threshold=800, int timeoutTime=200);
	void InitSolver();
	//ThreadSolver() {
	//	InitSolver();
	//}
};