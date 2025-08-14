#pragma once
#include <QDebug>
#include <QObject>
#include <QEventLoop>
#include <QTimer>
#include "MyThread.h"

#define	MAX_THREAD_NUM 2

class MultiSolver : public QObject {
	Q_OBJECT

private:
	MyThread* m_thread[24];
	int ansTime, ansId;
	int finishedCnt;

public:
	MultiSolver();
	CubeExplorerSPFA* GetMultiThreadPath(const string& str, int threshold=800, int timeoutTime=200);
	~MultiSolver();

public slots:
	void slot_threadFinished(int id, int costTime);

signals:
	void allThreadsFinished();

};