#include "MultiSolver.h"

#define	MAX_THREAD_NUM 24

MultiSolver::MultiSolver() {
    ansTime = 0x7f7f7f7f;
    ansId = finishedCnt = 0;
    for (int i = 0; i < MAX_THREAD_NUM; i++) {
        m_thread[i] = new MyThread(i);
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
    finishedCnt = ansId = 0, ansTime = 0x7f7f7f7f;

    QEventLoop loop;  // 局部事件循环
    connect(this, &MultiSolver::allThreadsFinished, &loop, &QEventLoop::quit);

    //QTimer::singleShot(timeoutTime, &loop, &QEventLoop::quit);

    for (int i = 0; i < MAX_THREAD_NUM; i++) {
        m_thread[i]->setCubeStatus(str);
        m_thread[i]->resume();
    }

    //qDebug() << "---------------- Run Success ----------------";
    
    loop.exec(); // 启动事件循环，等待所有线程完成

    //qDebug() << "Final: " << ansId << " " << ansTime << "Time: " << clock();

    return m_thread[ansId]->getCubeExplorerSPFA();
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

void MultiSolver::slot_threadFinished(int id, int costTime)
{
    QMutexLocker locker(&m_mutex);
    if (costTime < ansTime) {
        ansTime = costTime;
        ansId = id;
    }
    if(++finishedCnt >= MAX_THREAD_NUM)
        emit allThreadsFinished();
    qDebug() << "Current Time: " << clock() << "MultiSolver::slot_threadFinished: " << id << " " << costTime << " " << finishedCnt;
    qDebug() << "MultiSolver::slot_threadFinished: " << ansId << " " << ansTime;
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