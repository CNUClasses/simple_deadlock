//============================================================================
// Name        : deadlocks.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <thread>
#include <mutex>
#include <iostream>
using namespace std;
const int NUMB_TIMES=100;

mutex m;
void fun2(){
	m.lock();
}
void fun1(){
	m.lock();
	fun2();
}


mutex m1;
mutex m2;
void f1(int i){
	while(i>0){
		lock_guard<mutex> lg1(m1);
		lock_guard<mutex> lg2(m2);
	}
}

void f2(int i){
	while(i>0){
		lock_guard<mutex> lg1(m2);	//mutexes aquired out of order
		lock_guard<mutex> lg2(m1);
	}
}

int main() {
	// superficial blocking? Its actually undefined
	//from the C++ 11 standard
	//30.4.1.2.1/4 [Note: A program may deadlock if the
	//thread that owns a mutex object calls lock() on that object.]
	//it MAY, or it may not block.  It may work on 1 compiler and
	//not another
//	m.lock();
//	m.lock();

	//a realistic way to
	//do the above but same situation
//	fun1();

	//the threaded way with 2 mutexes
	thread t1(f1, NUMB_TIMES);
	thread t2(f2, NUMB_TIMES);
	t1.join();
	t2.join();
}
