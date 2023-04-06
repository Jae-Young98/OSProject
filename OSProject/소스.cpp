#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <windows.h>

using namespace std;
using std::thread;
using std::mutex;

vector<int> dataSet;
int threadNum = 1;
int* arr = new int[1000000];
int length = 0;

void commandLoad(string str) {
	// int i = 0;
	dataSet.clear(); // 초기화
	ifstream ifs(str);

	if (ifs.is_open()) {
		copy(istream_iterator<int>(ifs), istream_iterator<int>(), back_inserter(dataSet));
		ifs.close();
		cout << dataSet.size() << " integers loaded" << endl;
	}
	else {
		cout << "load failed" << endl;
	}
	// copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
	/*
	if (ifs.is_open()) {
		while (!ifs.eof()) {
			ifs >> arr[i];
			i++;
			length++;
		}
		cout << length << " integers loaded" << endl;
		ifs.close();
	} else {
		cout << "load failed" << endl;
	}
	*/
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
	/*
	if (length > 0 && length <= 10) {
		for (int i = 0; i < length - 1; i++) { 
			cout << arr[i] << " ";
		}
		cout << arr[length - 1] << endl; // 마지막 정수 출력 이후 공백(space bar) 없이
		cout << length << " integers" << endl;
	} else if (length > 10) {
		for (int i = 0; i < 9; i++) {
			cout << arr[i] << " ";
		}
		cout << arr[9] << endl;
		cout << length << " integers" << endl;
	} else {
		cout << length << " integers" << endl;
	}
	*/
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
	/*
	if (length > 0 && length <= 10) {
		for (int i = 0; i < length - 1; i++) {
			cout << arr[i] << " ";
		}
		cout << arr[length - 1] << endl;
		cout << length << " integers" << endl;
	} else if (length > 10) {
		for (int i = length - 10; i < length - 1; i++) {
			cout << arr[i] << " ";
		}
		cout << arr[length - 1] << endl;
		cout << length << " integers" << endl;
	} else {
		cout << length << " integers" << endl;
	}
	*/
}

void commandExit() {
	cout << "Bye!" << endl;
	exit(0);
}

bool checkNum(string str) {
	bool x = false;
	int num = stoi(str);
	if (num >= 1 && num <= 8) {
		x = true;
	}

	return x;
}
// thread init 함수
// sum, prod, count, range
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
		} //else if (command1 == "thread") {
			//if (checkNum(command2)) {
			//	cout << "test" << endl;
			//}
		//}
		else {
			cout << "invalid command" << endl;
		}
	}
}