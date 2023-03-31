#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <windows.h>

using namespace std;
using std::thread;
using std::mutex;

void load(string str) {
	int length = 0;
	int i = 0;
	int* arr = new int[1000000];

	ifstream ifs(str);

	if (ifs.is_open()) {
		while (!ifs.eof()) {
			ifs >> arr[i];
			i++;
			length++;
		}
		cout << length << " integers loaded" << endl;
	} else {
		cout << "invalid command" << endl;
	}
}

int main() {
	string input;
	string command1;
	string command2;
	string fileName;
	
	while (true) {
		cout << ">> ";
		getline(cin, input);
		command1 = input.substr(0, input.find(" ")); // 공백 앞 단어
		command2 = input.substr(input.find(" ") + 1, input.length()); // 공백 뒤 단어

		if (command1 == "load") {
			load(command2);
		} else {
			cout << "invalid command" << endl;
		}
		/* 테스트 2 폐기예정
		cin >> command >> fileName;
		if (command == "load") {
			load(fileName);
		} else {
			cout << "invalid command" << endl;
		}
		*/
	}
}