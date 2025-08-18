#include "MyThread.h"

MyThread::MyThread(int poseId, TaskQueue* q):
    m_poseId(poseId), m_queue(q)
{
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
            //QThread::msleep(100);
            //continue;
            //qDebug() << "Thread " << m_poseId << " waiting.";
            m_condition.wait(&m_mutex); // 暂停时阻塞，等待唤醒
        }
        //qDebug() << "Thread " << m_poseId << " OUT.";
        m_mutex.unlock();

        if (m_stop) break;

        //qDebug() << "Thread " << m_poseId << " started.";
        int st = clock();
        //qDebug() << "Thread " << m_poseId << " Start Time: " << st;
        std::string transformed = "";
        char* solKociemba;
        // ---- 核心逻辑（无动态内存分配）----
//        for (int i = m_poseId; i < MAX_TASK_NUM; i += MAX_THREAD_NUM) {
        while (true)
        {
            auto task = m_queue->pop(m_stop);
            if (task==-1) break; // stop时退出
            int i = task;
            qDebug() << "Thread " << m_poseId << " processing pose " << i;
            qDebug() << "Status " << QString::fromStdString(m_cubeStatus);
            transformed = cubePoseTransformer->Transform(m_cubeStatus, i);
            qDebug() << "transformed " << QString::fromStdString(transformed);
            solKociemba = kociembaSolver->cube_solve(
                const_cast<char*>(transformed.c_str()),
                nullptr);
            qDebug() << "solKociemba " << solKociemba;
            if (!solKociemba) {
                emit sendAns(i, -1, "");
                break;
            }
            cubeExplorerSPFA->GetShortestPath(cubePoseTransformer->ReTransform(solKociemba, i));
            emit sendAns(i, cubeExplorerSPFA->GetAnsCostTime(), cubeExplorerSPFA->GetAnsOpSequence());
        }
        // ---------------------------------

        int ed = clock();
        //qDebug() << "Thread " << m_poseId << " finished.";
        //emit sendAns(m_poseId, cubeExplorerSPFA->GetAnsCostTime());
        //qDebug() << "Thread " << m_poseId << "Finish Time:" << ed << "Duration: " << ed - st;
        // 自动暂停
        //m_mutex.lock();
        m_paused = true;
        //m_mutex.unlock();

        // 添加微小延迟，防止立即重新获取锁导致CPU占用过高
        //QThread::usleep(1000); // 1毫秒
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
