#pragma once

#include <chrono>

class MyTimer {
	typedef std::chrono::steady_clock::time_point   tp;
	typedef std::chrono::duration<double>           dd;
	typedef std::chrono::steady_clock               sc;

private:
	tp tpBegin;
	dd ddSpan;

public:
	MyTimer() : tpBegin(tp()), ddSpan(dd(0)) {}

	void start() {
		tpBegin = sc::now();
	}

	void stop() {
		tp tpEnd = sc::now();
		ddSpan += std::chrono::duration<double>(tpEnd - tpBegin);
	}

	double getTime() {
		tp tpEnd = sc::now();
		dd spanTmp = dd(0);
		spanTmp += std::chrono::duration<double>(tpEnd - tpBegin);
		return spanTmp.count();
	}

	void reset() {
		ddSpan = dd(0);
	}
};