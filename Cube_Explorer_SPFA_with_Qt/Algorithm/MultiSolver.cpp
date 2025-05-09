#include "MultiSolver.h"

void MultiSolver::InitSolver() {
    ansTime = 0x7f7f7f7f;
    //kociembaSolver = new threadSafeKociemba();
    //cubePoseTransformer = new CubePoseTransformer();
    //cubePoseTransformer->Init();
    for (int i = 0; i < 24; i++) {
        cubeExplorerSPFA[i] = new CubeExplorerSPFA();
        cubeExplorerSPFA[i]->InitOrientation();
        kociembaSolver[i] = new threadSafeKociemba();
        cubePoseTransformer[i] = new CubePoseTransformer();
        cubePoseTransformer[i]->Init();
    }
    ansCubeExplorerSPFA = nullptr;
}

CubeExplorerSPFA* MultiSolver::GetMultiThreadPath(const std::string& str, int threshold, int timeoutTime)
{
    cubeStatus = str; ansTime = 0x7f7f7f7f;
    ansCubeExplorerSPFA = nullptr;
    int st = clock();
    for (int i = 0; i < 24; i++) {
        GetSinglePosePath(i);
        if (cubeExplorerSPFA[i]->ansTime < ansTime) {
            ansTime = cubeExplorerSPFA[i]->ansTime;
            ansCubeExplorerSPFA = cubeExplorerSPFA[i];
        }
        if (cubeExplorerSPFA[i]->ansTime < threshold || clock() - st > timeoutTime) return ansCubeExplorerSPFA;
    }
    return ansCubeExplorerSPFA;

    //constexpr int totalTasks = 24;
    //std::vector<std::thread> threads;

    //// 无锁数据结构
    //std::atomic<int> bestTime(INT_MAX);
    //std::atomic<CubeExplorerSPFA*> bestSolution(nullptr);
    //std::atomic<int> taskIndex(0);

    //auto worker = [&]() {
    //    while (true) {
    //        int current = taskIndex.fetch_add(1, std::memory_order_relaxed);
    //        if (current >= totalTasks) break;

    //        GetSinglePosePath(current);

    //        // 局部变量暂存结果
    //        int localTime = cubeExplorerSPFA[current]->ansTime;
    //        CubeExplorerSPFA* localSolution = cubeExplorerSPFA[current];

    //        // 无锁更新最优解
    //        int currentBest = bestTime.load(std::memory_order_acquire);
    //        while (localTime < currentBest) {
    //            if (bestTime.compare_exchange_weak(
    //                currentBest, localTime,
    //                std::memory_order_release,
    //                std::memory_order_relaxed))
    //            {
    //                bestSolution.store(localSolution, std::memory_order_release);
    //                break;
    //            }
    //        }
    //    }
    //};

    //const int threadsNum = 1;
    //threads.reserve(threadsNum);
    //for (int t = 0; t < threadsNum; ++t) {
    //    threads.emplace_back(worker);
    //}

    //for (auto& t : threads) t.join();

    //return bestSolution.load(std::memory_order_acquire);
}

void MultiSolver::GetSinglePosePath(int poseId)
{
    ofstream out; out.open("./Data/multiSolver"+to_string(poseId) + ".txt", ios::trunc);
    int st = clock();

    char* tmpCubeStatus = new char[cubeStatus.length() + 1];

    out << "tmpCubeStatus Time = " << clock() - st << endl;
    //FILE* out = fopen("MultiSolver.txt", "w");
    //fprintf(out, "cubeStatus = ");
    //for (int i = 0; i < cubeStatus.length(); i++) {
    //    fprintf(out, "%c", cubeStatus[i]);
    //}fprintf(out, "\n");
    st = clock();

    strcpy(tmpCubeStatus, cubePoseTransformer[poseId]->Transform(cubeStatus, poseId).c_str());

    out << "cpy&Transform Time = " << clock() - st << endl;
    
    //fprintf(out, "tmp = %s\n", tmpCubeStatus);

    st = clock();
    string solKociemba = kociembaSolver[poseId]->cube_solve(tmpCubeStatus, NULL);
    out << "kociemba Time = " << clock() - st << endl;
    
    //fprintf(out, "sol = %s\n", tmpCubeStatus);
    //fclose(out);
    //string solKociemba(tmpCubeStatus);

    out << "Kociemba Result : " << solKociemba << endl;
    
    st = clock();
    solKociemba = cubePoseTransformer[poseId]->ReTransform(solKociemba, poseId);
    out << "ReTransform Time = " << clock() - st << endl;

    st = clock();
    
    /*int tmpTime = */

    cubeExplorerSPFA[poseId]->GetShortestPath(solKociemba);
    out << "SPFA Time = " << clock() - st << endl;
    
    int targetStep = cubeExplorerSPFA[poseId]->GetTargetStepNumber();
    int ansOpStep = cubeExplorerSPFA[poseId]->GetAnsOpStepNumber();

    out << "Retransform Result : " << solKociemba << " " << targetStep << endl;
    out << cubeExplorerSPFA[poseId]->GetAnsOpSequence() << " " << ansOpStep << endl;

    out.close();
}
