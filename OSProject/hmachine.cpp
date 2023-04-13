#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;
using std::thread;
using std::mutex;

vector<int> dataSet;
vector<string> logV;
vector<double> logMS;
string command1;
string command2;
string rangeX;
string rangeY;
int threadNum = 1;
int key; // count x
int x; // range x
int y; // range y

void commandLoad(string str) {
	dataSet.clear(); // 초기화
	threadNum = 1; // 스레드 수 초기화
	ifstream ifs(str);

	if (ifs.is_open()) {
		copy(istream_iterator<int>(ifs), istream_iterator<int>(), back_inserter(dataSet));
		ifs.close();
		cout << dataSet.size() << " integers loaded" << endl;
	} else {
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
	for (int i = start; i < end; i++) {
		*result += dataSet[i];
		*result %= 1000000;
	}

	// test code
	// printf("dataSet %d to %d : %d \n", start, end - 1, x);
}

void funcProd(int start, int end, unsigned long long* result) {
	for (int i = start; i < end; i++) {
		*result *= dataSet[i];
		*result %= 1000000;
	}

	// test code
	// printf("dataSet %d to %d : %llu \n", start, end - 1, *result);
}

void funcCount(int start, int end, int* result) {
	for (int i = start; i < end; i++) {
		if (dataSet[i] == key) {
			*result += 1;
		}
	}

	// test code
	// printf("dataSet %d to %d : %d \n", start, end - 1, count);
}

void funcRange(int start, int end, int* result) {
	for (int i = start; i < end; i++) {
		if (dataSet[i] >= x && dataSet[i] <= y) {
			*result += 1;
		}
	}

	// test code
	// printf("dataSet %d to %d : %d \n", start, end - 1, rangeCnt);
}

void commandSum() {
	int sum = 0;
	int start = 0;
	int end = 0;
	int chunkSize = dataSet.size() / threadNum;
	int remainder = dataSet.size() % threadNum;
	vector<thread> t(threadNum);
	logV.push_back(to_string(threadNum));

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
	cout << "[" << threadNum << " workers] " << "sum => " << sum << endl;
	logV.push_back(to_string(sum));
}

void commandProd() {
	unsigned long long int prod = 1;
	int start = 0;
	int end = 0;
	int chunkSize = dataSet.size() / threadNum;
	int remainder = dataSet.size() % threadNum;
	vector<thread> t(threadNum);
	logV.push_back(to_string(threadNum));

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
		cout << "[" << threadNum << " workers] " << "prod => " << prod << endl;
	}
	logV.push_back(to_string(prod));
}

void commandCount() {
	int cnt = 0;
	key = stoi(command2);
	int start = 0;
	int end = 0;
	int chunkSize = dataSet.size() / threadNum;
	int remainder = dataSet.size() % threadNum;
	vector<thread> t(threadNum);
	logV.push_back(to_string(threadNum));

	for (int i = 0; i < threadNum; i++) {
		end += chunkSize;
		if (remainder > 0) {
			end++;
			remainder--;
		}
		t[i] = thread(funcCount, start, end, &cnt);
		start = end;
	}
	for (int i = 0; i < threadNum; i++) {
		t[i].join();
	}
	cout << "[" << threadNum << " workers] " << "count " << key << " => " << cnt << endl;
	logV.push_back(to_string(cnt));
}

void commandRange() {
	int cnt = 0;
	int start = 0;
	int end = 0;
	int chunkSize = dataSet.size() / threadNum;
	int remainder = dataSet.size() % threadNum;
	vector<thread> t(threadNum);
	logV.push_back(to_string(threadNum));

	for (int i = 0; i < threadNum; i++) {
		end += chunkSize;
		if (remainder > 0) {
			end++;
			remainder--;
		}
		t[i] = thread(funcRange, start, end, &cnt);
		start = end;
	}
	for (int i = 0; i < threadNum; i++) {
		t[i].join();
	}
	cout << "[" << threadNum << " workers] " << "range " << x << ".." << y << " => " << cnt << endl;
	logV.push_back(to_string(cnt));
}

// 유효한 숫자인지 판별
bool checkNum(string cmd1, string cmd2) {
	bool x;
	x = atoi(cmd2.c_str()) != 0 || cmd2.compare("0") == 0; // 숫자가 아니면 0, 숫자이면 0이 아닌 다른 수
	if (x) {
		if (cmd1 == "count") {
			return true;
		}
		int num = atoi(cmd2.c_str());
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

void commandExit() {
	cout << "Bye!" << endl;
	exit(0);
}

// range x..y 유효한 명령어 판별
bool getRange(string str) {
	int dotCnt = 0;
	int previous = 0;
	int current = 0;

	for (int i = 0; i < command2.length(); i++) {
		if (command2.find(".") == -1) {
			break;
		} else if (command2.find(".", i) <= i) {
			dotCnt++;
		}
	}

	// ..을 기준으로 x, y를 나누기 때문에 .(dot)이 2개 필요
	if (dotCnt == 2) {
		current = command2.find(".");
		rangeX = command2.substr(previous, current - previous); // x 값
		previous = current + 2;
		current = command2.find(".", previous);
		rangeY = command2.substr(previous, current - previous); // y 값
		x = stoi(rangeX);
		y = stoi(rangeY);
		if (x > y) {
			return false;
		} else {
			return true;
		}
	} else {
		return false;
	}
	// x, y 값 출력 테스트
	// cout << rangeX << " " << rangeY << endl;
}

// 로그 출력
void commandLog() {
	int idx = 1;
	int k = 0;
	int n = 0;

	if (logV.size() == 0) { // 로그가 비어있으면
		cout << "log does not exist" << endl;
	} else {
		cout << "index" << setw(20) << "integers" << setw(20) << "command" << setw(20) << "thread" << setw(20) << "result" << setw(20) << "ms" << endl;
		for (int i = 0; i < logV.size() / 4; i++) {
			if (i >= 1) {
				k += 4;
			}
			cout << setw(5) << idx;
			for (int j = 0; j < 4; j++) {
				cout << setw(20) << logV[j + k];
			}
			printf("%20.2f\n", logMS[n]);
			n++;
			idx++;
		}
	}
}

void clearLog() {
	if (logV.size() == 0 && logMS.size() == 0) {
		cout << "log does not exist" << endl;
	} else {
		logV.clear();
		logMS.clear();
		cout << "log has been cleard" << endl;
	}
}

int main() {
	string input;
	string str;

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
			if (checkNum(command1, command2)) {
				cout << threadNum << " workers for each job" << endl;
			} else {
				cout << "invalid command" << endl;
			}
		} else if (command1 == "sum") {
			logV.push_back(to_string(dataSet.size()));
			logV.push_back(command1);
			auto start = chrono::high_resolution_clock::now();
			commandSum();
			auto end = chrono::high_resolution_clock::now();
			chrono::duration<double, milli> ms = end - start;
			printf("%.2fms\n", ms.count());
			logMS.push_back(ms.count());

		} else if (command1 == "prod") {
			logV.push_back(to_string(dataSet.size()));
			logV.push_back(command1);
			auto start = chrono::high_resolution_clock::now();
			commandProd();
			auto end = chrono::high_resolution_clock::now();
			chrono::duration<double, milli> ms = end - start;
			printf("%.2fms\n", ms.count());
			logMS.push_back(ms.count());
		} else if (command1 == "count") {
			if (checkNum(command1, command2)) { // command2가 숫자로 들어오는지?
				logV.push_back(to_string(dataSet.size()));
				logV.push_back(input);
				auto start = chrono::high_resolution_clock::now();
				commandCount();
				auto end = chrono::high_resolution_clock::now();
				chrono::duration<double, milli> ms = end - start;
				printf("%.2fms\n", ms.count());
				logMS.push_back(ms.count());
			} else {
				cout << "invalid command" << endl;
			}
		} else if (command1 == "range") {
			if (getRange(command2)) {
				logV.push_back(to_string(dataSet.size()));
				logV.push_back(input);
				auto start = chrono::high_resolution_clock::now();
				commandRange();
				auto end = chrono::high_resolution_clock::now();
				chrono::duration<double, milli> ms = end - start;
				printf("%.2fms\n", ms.count());
				logMS.push_back(ms.count());
			} else {
				cout << "invalid command" << endl;
			}
		} else if (command1 == "log") {
			if (command2 == "clear") { // 로그 비우기
				clearLog();
			}
			else {
				commandLog();
			}
		} else {
			cout << "invalid command" << endl;
		}
	}
}