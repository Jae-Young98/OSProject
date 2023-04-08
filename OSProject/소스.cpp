#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <windows.h>

using namespace std;
using std::thread;
using std::mutex;

vector<int> dataSet;
int threadNum = 1;


void commandLoad(string str) {
	dataSet.clear(); // 초기화
	threadNum = 1; // 스레드 수 초기화
	ifstream ifs(str);

	if (ifs.is_open()) {
		copy(istream_iterator<int>(ifs), istream_iterator<int>(), back_inserter(dataSet));
		ifs.close();
		cout << dataSet.size() << " integers loaded" << endl;
	}
	else {
		cout << "load failed" << endl;
	}
}

void commandHead() {
	if (dataSet.size() == 0) {
		cout << dataSet.size() << " integers" << endl;
	} else if (dataSet.size() <= 10) {
		copy(dataSet.begin(), dataSet.end(), ostream_iterator<int>(cout, " "));
		cout << endl;
		cout << dataSet.size() << " integers" << endl;
	} else {
		for (int i = 0; i < 10; i++) {
			cout << dataSet[i] << " ";
		}
		cout << endl;
		cout << dataSet.size() << " integers" << endl;
	}
}

void commandTail() {
	if (dataSet.size() == 0) {
		cout << dataSet.size() << " integers" << endl;
	} else if (dataSet.size() <= 10) {
		copy(dataSet.begin(), dataSet.end(), ostream_iterator<int>(cout, " "));
		cout << endl;
		cout << dataSet.size() << " integers" << endl;
	} else {
		for (int i = dataSet.size() - 10; i < dataSet.size(); i++) {
			cout << dataSet[i] << " ";
		}
		cout << endl;
		cout << dataSet.size() << " integers" << endl;
	}
}


void funcSum(int start, int end, int* result) {
	int x = 0;
	for (int i = start; i < end; i++) {
		x += dataSet[i];
	}
	*result += x;

	// test code
	// printf("Thread %d to %d : %d \n", start, end - 1, x);
}

void funcProd(int start, int end, long long* result) {
	long long int x = 1;
	for (int i = start; i < end; i++) {
		x *= dataSet[i];
	}
	*result *= x;

	// test code
	// printf("Thread %d to %d : %lld \n", start, end - 1, x);
}

void commandSum() {
	int sum = 0;
	int start = 0;
	int end = 0;
	int chunkSize = dataSet.size() / threadNum;
	int remainder = dataSet.size() % threadNum;
	vector<thread> t(threadNum);

	for (int i = 0; i < threadNum; i++) {
		end += chunkSize;
		if (remainder > 0) {
			end++;
			remainder--;
		}
		t[i] = thread(funcSum, start, end, &sum);
		start = end;
	}
	for (int i = 0; i < threadNum; i++) {
		t[i].join();
	}
	sum %= 1000000;
	cout << "[" << threadNum << " workers] " << "sum => " << sum << endl;
}

void commandProd() {
	long long int prod = 1;
	int start = 0;
	int end = 0;
	int chunkSize = dataSet.size() / threadNum;
	int remainder = dataSet.size() % threadNum;
	vector<thread> t(threadNum);

	if (dataSet.size() == 0) { // load 하지 않고 prod 했을시 1 출력 되는것 예외처리
		cout << "[" << threadNum << " workers] " << "prod => 0" << endl;
	} else {
		for (int i = 0; i < threadNum; i++) {
			end += chunkSize;
			if (remainder > 0) {
				end++;
				remainder--;
			}
			t[i] = thread(funcProd, start, end, &prod);
			start = end;
		}
		for (int i = 0; i < threadNum; i++) {
			t[i].join();
		}
		prod %= 1000000;
		cout << "[" << threadNum << " workers] " << "prod => " << prod << endl;
	}
}

void commandExit() {
	cout << "Bye!" << endl;
	exit(0);
}

// 스레드 생성시 유효한 숫자인지 판별
bool checkNum(string str) {
	bool x;
	x = atoi(str.c_str()) != 0 || str.compare("0") == 0;
	if (x) {
		int num = atoi(str.c_str());
		if (num >= 1 && num <= 8) {
			threadNum = num;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

// count, range
// time 시간 측정

int main() {
	string input;
	string command1;
	string command2;

	
	while (true) {
		cout << ">> ";
		getline(cin, input);
		command1 = input.substr(0, input.find(" ")); // 공백 앞 단어
		command2 = input.substr(input.find(" ") + 1, input.length()); // 공백 뒤 단어

		if (command1 == "load") {
			commandLoad(command2);
		} else if (command1 == "head") {
			commandHead();
		} else if (command1 == "exit") {
			commandExit();
		} else if (command1 == "tail") {
			commandTail();
		} else if (command1 == "thread") {
			if (checkNum(command2)) {
				cout << threadNum << " workers for each job" << endl;
			} else {
				cout << "invalid command" << endl;
			}
		} else if (command1 == "sum") {
			auto start = chrono::high_resolution_clock::now();
			commandSum();
			auto end = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
			cout << duration.count() << " ms" << endl;
		} else if (command1 == "prod") {
			auto start = chrono::high_resolution_clock::now();
			commandProd();
			auto end = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
			cout << duration.count() << " ms" << endl;
		}
		else {
			cout << "invalid command" << endl;
		}
	}
}