#include "MyThread.h"

MyThread::MyThread(int poseId) {
    // 设置线程编号
    m_poseId = poseId;

    // 设置初始状态
    m_paused = true;
    m_stop = false;
    
    // SPFA求解器
    cubeExplorerSPFA = new CubeExplorerSPFA();
    cubeExplorerSPFA->InitOrientation();
    
    // Kociemba求解器
    kociembaSolver = new threadSafeKociemba();
    
    // 姿态转换器
    cubePoseTransformer = new CubePoseTransformer();
    cubePoseTransformer->Init();
    
}

void MyThread::run() {
    while (!m_stop) {
        //qDebug() << "Thread " << m_poseId << " pause Flag: " << m_paused;
        m_mutex.lock();
        if (m_paused) {
            //qDebug() << "Thread " << m_poseId << " waiting.";
            m_condition.wait(&m_mutex); // 暂停时阻塞，等待唤醒
        }
        m_mutex.unlock();

        if (m_stop) break;

        //qDebug() << "Thread " << m_poseId << " started.";
        int st = clock();

        // ---- 核心逻辑（无动态内存分配）----
        //for (; m_poseId<24; m_poseId += 4) {
            //m_poseId = i;
            std::string transformed = cubePoseTransformer->Transform(m_cubeStatus, m_poseId);
            std::string solKociemba = kociembaSolver->cube_solve(
                const_cast<char*>(transformed.c_str()),
                nullptr);
            solKociemba = cubePoseTransformer->ReTransform(solKociemba, m_poseId);
            cubeExplorerSPFA->GetShortestPath(solKociemba);
        //}
        // ---------------------------------

        int ed = clock();
        //qDebug() << "Thread " << m_poseId << " finished.";
        emit sendAns(m_poseId, cubeExplorerSPFA->GetAnsCostTime());
        qDebug() << "Thread " << m_poseId << " Start Time: " << st << "Finish Time:" << ed;
        // 自动暂停
        m_mutex.lock();
        m_paused = true;
        m_mutex.unlock();
    }
}

void MyThread::setCubeStatus(const std::string& status) {
    QMutexLocker locker(&m_mutex);
    m_cubeStatus = status; // 复用已有的 std::string，避免重新分配
    //qDebug() << "Cube status set to:" << QString::fromStdString(m_cubeStatus);
}

void MyThread::resume() {
    QMutexLocker locker(&m_mutex);
    m_paused = false;
    m_condition.wakeOne(); // 唤醒线程
}

void MyThread::stop() {
    QMutexLocker locker(&m_mutex);
    m_stop = true;
    m_condition.wakeAll(); // 确保线程退出
}

CubeExplorerSPFA* MyThread::getCubeExplorerSPFA()
{
    return cubeExplorerSPFA;
}
