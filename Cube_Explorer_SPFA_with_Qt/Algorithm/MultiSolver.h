#pragma once
#include <QDebug>
#include <QObject>
#include <QEventLoop>
#include <QTimer>
#include <QMutex>
#include "MyThread.h"

class MultiSolver : public QObject {
	Q_OBJECT

private:
	QMutex m_mutex;
	MyThread* m_thread[24];
	string ansOpsequence;
	int ansId;
	int finishedCnt;
	CubeExplorerSPFA* ansCubeExplorerSPFA;

public:
	MultiSolver();
	CubeExplorerSPFA* GetMultiThreadPath(const string& str, int threshold=800, int timeoutTime=250);
	~MultiSolver();

public slots:
	void slot_threadFinished(int id, int costTime, string ansOpseq);

signals:
	void allThreadsFinished();

};