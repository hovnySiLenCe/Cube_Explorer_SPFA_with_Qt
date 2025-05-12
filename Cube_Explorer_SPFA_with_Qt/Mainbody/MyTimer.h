#pragma once

#include <chrono>

class MyTimer {
	typedef std::chrono::steady_clock::time_point   tp;
	typedef std::chrono::duration<double>           dd;
	typedef std::chrono::steady_clock               sc;

private:
	tp tpBegin, tpEnd;
	dd ddSpan;
	bool isStarted = false;

public:
	MyTimer() : tpBegin(tp()), ddSpan(dd(0)), isStarted(false) {}

	bool isRunning() {
		return isStarted;
	}

	void start() {
		isStarted = true;
		tpBegin = sc::now();
	}

	void stop() {
		isStarted = false;
		tpEnd = sc::now();
		ddSpan += std::chrono::duration<double>(tpEnd - tpBegin);
	}

	double getTime() {
        if (!isStarted) return ddSpan.count();
		tp tpTmp = sc::now();
		dd spanTmp = dd(0);
		spanTmp = ddSpan + std::chrono::duration<double>(tpTmp - tpBegin);
		//spanTmp = (int) pow(10, offset) * spanTmp / pow(10, offset);
		return spanTmp.count();
	}

	void reset() {
		isStarted = false;
		ddSpan = dd(0);
	}
};