#include "MultiSolver.h"

MultiSolver::MultiSolver() {
    ansCubeExplorerSPFA = new CubeExplorerSPFA();
    ansCubeExplorerSPFA->ansTime = 0x7f7f7f7f;
    ansId = finishedCnt = 0;

    connect(this, &MultiSolver::allThreadsFinished, &m_loop, &QEventLoop::quit);

    for (int i = 0; i < MAX_THREAD_NUM; i++) {
        m_thread[i] = new MyThread(i, &taskQueue);
        bool connected = QObject::connect(
            m_thread[i], &MyThread::sendAns,
            this, &MultiSolver::slot_threadFinished,
            Qt::DirectConnection
        );
        if (!connected) {
            qCritical() << "Failed to connect thread" << i;
        }
        m_thread[i]->start();
    }
}
//void MultiSolver::InitSolver() {
//    ansTime = 0x7f7f7f7f;
//    ansId = 0;
//    //kociembaSolver = new threadSafeKociemba();
//    //cubePoseTransformer = new CubePoseTransformer();
//    //cubePoseTransformer->Init();
//    for (int i = 0; i < 24; i++) {
//        m_thread[i] = new MyThread(i);
//    }
//}

CubeExplorerSPFA* MultiSolver::GetMultiThreadPath(const std::string& str, int threshold, int timeoutTime)
{
    finishedCnt = ansId = 0, ansCubeExplorerSPFA->ansTime = 0x7f7f7f7f;

    if (str.size() != 54) {
        ansCubeExplorerSPFA->ansTime = -1;
        return ansCubeExplorerSPFA;
    }

    //QTimer::singleShot(timeoutTime, &loop, &QEventLoop::quit);

    for (int i = 0; i < MAX_TASK_NUM; ++i) taskQueue.push(i);

    for (int i = 0; i < MAX_THREAD_NUM; i++) {
        m_thread[i]->setCubeStatus(str);
        m_thread[i]->resume();
    }

    //qDebug() << "---------------- Run Success ----------------";
    
    m_loop.exec(); // 启动事件循环，等待所有线程完成

    //qDebug() << "Final: " << ansId << " " << ansCubeExplorerSPFA->ansTime << "Time: " << clock();

    return ansCubeExplorerSPFA;
    /*
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
    */
}

void MultiSolver::slot_threadFinished(int id, int costTime, string ansOpseq)
{
    //qDebug() << "Current Time: " << clock() << "MultiSolver::slot_threadFinished: " << id << " " << costTime << " " << finishedCnt;
    //if (finishedCnt) return;
    QMutexLocker locker(&m_mutex);
    if (costTime < ansCubeExplorerSPFA->ansTime) {
        ansCubeExplorerSPFA->ansTime = costTime;
        ansCubeExplorerSPFA->ansOpSequence = ansOpseq;
        ansId = id;
    }
    //if (!finishedCnt++) emit allThreadsFinished();
    if(++finishedCnt >= MAX_TASK_NUM || ansCubeExplorerSPFA->ansTime < 0) emit allThreadsFinished();
    //qDebug() << "MultiSolver::slot_threadFinished: " << ansId << " " << ansCubeExplorerSPFA->ansTime;
}

MultiSolver::~MultiSolver()
{
    for (int i = 0; i < MAX_THREAD_NUM; i++) {
        m_thread[i]->stop();
        m_thread[i]->wait();
        //qDebug() << "MultiSolver::~MultiSolver: " << i;
        delete m_thread[i];
    }
}