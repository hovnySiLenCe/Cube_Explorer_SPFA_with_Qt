#include "MultiSolver.h"

void MultiSolver::InitSolver() {
    ansTime = 0x7f7f7f7f;
    kociembaSolver = new threadSafeKociemba();
    cubePoseTransformer = new CubePoseTransformer();
    cubePoseTransformer->Init();
    for (int i = 0; i < 24; i++) {
        cubeExplorerSPFA[i] = new CubeExplorerSPFA();
        cubeExplorerSPFA[i]->InitOrientation();
    }
    ansCubeExplorerSPFA = nullptr;
}

CubeExplorerSPFA* MultiSolver::GetMultiThreadPath(string str)
{
    cubeStatus = str;
    vector<thread> threads;

    for (int i = 1; i < 24; i++) {
        GetSinglePosePath(i);
        //threads.push_back(std::thread(&MultiSolver::GetSinglePosePath, this, i));
    }

    //for (auto& thread : threads) {
    //    if (thread.joinable()) {
    //        thread.join();
    //    }
    //}

    ansTime = INF;
    ansCubeExplorerSPFA = nullptr;

    for (int i = 1; i < 24; i++) {
        //printf("%d %d\n", i, cubeExplorerSPFA[i]->GetAnsCostTime());
        /*if (cubeExplorerSPFA[i]->GetAnsCostTime() < ansTime) {
            ansTime = cubeExplorerSPFA[i]->GetAnsCostTime();
            ansCubeExplorerSPFA = cubeExplorerSPFA[i];
        }*/
        if (cubeExplorerSPFA[i]->ansTime < ansTime) {
            ansTime = cubeExplorerSPFA[i]->ansTime;
            ansCubeExplorerSPFA = cubeExplorerSPFA[i];
        }
    }

    //printf( "Ans Of All.\n");
    if (!ansCubeExplorerSPFA) return nullptr;

    ansCubeExplorerSPFA->SaveMechanicalStep();
    //cout << ansCubeExplorerSPFA->GetAnsOpSequence();
    //printf(" %d\n", ansCubeExplorerSPFA->GetAnsOpStepNumber());
    return ansCubeExplorerSPFA;
}

void MultiSolver::GetSinglePosePath(int poseId)
{
    //ofstream out; out.open("multiSolver.txt", ios::trunc);
    //int st = clock();

    char* tmpCubeStatus = new char[cubeStatus.length() + 1];

    //out << "tmpCubeStatus Time = " << clock() - st << endl;
    //FILE* out = fopen("MultiSolver.txt", "w");
    //fprintf(out, "cubeStatus = ");
    //for (int i = 0; i < cubeStatus.length(); i++) {
    //    fprintf(out, "%c", cubeStatus[i]);
    //}fprintf(out, "\n");
    //st = clock();

    strcpy(tmpCubeStatus, cubePoseTransformer->Transform(cubeStatus, poseId).c_str());

    //out << "cpy&Transform Time = " << clock() - st << endl;
    //fprintf(out, "tmp = %s\n", tmpCubeStatus);

    //st = clock();
    string solKociemba = kociembaSolver->cube_solve(tmpCubeStatus, NULL);
    //out << "kociemba Time = " << clock() - st << endl;
    //fprintf(out, "sol = %s\n", tmpCubeStatus);
    //fclose(out);
    //string solKociemba(tmpCubeStatus);

    //cout << "Kociemba Result : " << solKociemba << endl;
    //st = clock();
    solKociemba = cubePoseTransformer->ReTransform(solKociemba, poseId);
    //out << "ReTransform Time = " << clock() - st << endl;

    //st = clock();
    /*int tmpTime = */cubeExplorerSPFA[poseId]->GetShortestPath(solKociemba);
    //out << "SPFA Time = " << clock() - st << endl;
    //out.close();
    //int targetStep = cubeExplorerSPFA[poseId]->GetTargetStepNumber();
    //int ansOpStep = cubeExplorerSPFA[poseId]->GetAnsOpStepNumber();

    //cout << "Retransform Result : " << solKociemba << " " << targetStep << endl;
    //cout << cubeExplorerSPFA[poseId]->GetAnsOpSequence() << " " << ansOpStep << endl;
}
