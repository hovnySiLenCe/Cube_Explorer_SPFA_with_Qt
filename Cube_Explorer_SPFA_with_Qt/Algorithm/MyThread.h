#pragma once
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <string>

#include "threadSafeKociemba.h"
#include "CubePoseTransformer.h"
#include "CubeExplorerSPFA.h"

#define MAX_THREAD_NUM 12
#define MAX_TASK_NUM 24

class MyThread: public QThread {
    Q_OBJECT

public:
    explicit MyThread(int poseId);
    void setCubeStatus(const std::string& status); // 设置新数据
    void resume();  // 恢复线程运行一次
    void stop();    // 完全停止线程
    CubeExplorerSPFA* getCubeExplorerSPFA();

protected:
    void run() override;

signals:
    void sendAns(int poseId, int ansTime, string ansOpSeq);

private:
    QMutex m_mutex;
    QWaitCondition m_condition;
    bool m_paused, m_stop;

    std::string m_cubeStatus; // 避免重复分配内存
    int m_poseId;

    // 求解相关容器
    threadSafeKociemba* kociembaSolver;
    CubeExplorerSPFA* cubeExplorerSPFA;
    CubePoseTransformer* cubePoseTransformer;

};