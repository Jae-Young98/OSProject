#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <windows.h>

using namespace std;
using std::thread;
using std::mutex;

int* arr = new int[1000000];
int length = 0;

void commandLoad(string str) {
	int i = 0;
	
	ifstream ifs(str);

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
}

void commandHead() {
	if (length > 0 && length <= 10) {
		for (int i = 0; i < length - 1; i++) { 
			cout << arr[i] << " ";
		}
		cout << arr[length - 1] << endl; // ������ ���� ��� ���� ����(space bar) ����
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
}

void commandTail() {
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
}

void commandExit() {
	cout << "Bye!" << endl;
	exit(0);
}

// thread init �Լ�

int main() {
	string input;
	string command1;
	string command2;
	
	while (true) {
		cout << ">> ";
		getline(cin, input);
		command1 = input.substr(0, input.find(" ")); // ���� �� �ܾ�
		command2 = input.substr(input.find(" ") + 1, input.length()); // ���� �� �ܾ�

		if (command1 == "load") {
			commandLoad(command2);
		} else if (command1 == "head") {
			commandHead();
		} else if (command1 == "exit") {
			commandExit();
		} else if (command1 == "tail") {
			commandTail();
		} else {
			cout << "invalid command" << endl;
		}
	}
}