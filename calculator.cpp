#include <iostream>
#include <string>
using namespace std;

	/*
	The program takes into account:
		- integers(any length)
		- "+", "-", "*", "(", ")"
		- unlimited number of characters and brackets
		- parentheses in parentheses cannot be written
		- the program gives not only an answer, but also shows a step-by-step solution
	*/

	//-11-(23*3)*(3-4*6)-(-2890+3)*0+1-333+2+(-5*3-1)+12*(12-34)-23+2021 (example equation)

int DigitCount(int number) {
	int result = 1;
	while ((number /= 10) != 0)
		result++;
	return result;
}
int* RemoveEntry(int*& arr, int delete_id, int& size) {
	int* new_arr = new int[size - 1];
	for (int i = 0, j = 0; i < size; i++)
		if (i != delete_id) {
			new_arr[j] = arr[i];
			j++;
		}
	size--;
	delete[]arr;
	arr = new_arr;
	return arr;
}
void Show(int* arr, int size) {
	cout << endl;
	for (int i = 0; i < size; i++) {
		if (arr[i] == 2147483647) cout << '+';
		else if (arr[i] == 2147483646) cout << '-';
		else if (arr[i] == 2147483645) cout << '*';
		else if (arr[i] == 2147483644) cout << '(';
		else if (arr[i] == 2147483643) cout << ')';
		else cout << arr[i];
	}
}
int main() {
	string str;
	cin >> str;
	// counting numbers and signs
	int arrSize = 0, countBracket = 0;
	for (int i = 0; i < str.length(); i++)
		if (isdigit(str[i]) == 0) { 
			if (str[i] == '(' || str[i] == ')') countBracket++;
			else arrSize++;
		}
	arrSize = arrSize * 2 + 1 + countBracket; 
	int* arr = new int[arrSize]; 
	// writing to an integer array of characters from str
	string myStr = str; bool flag = true, bracket = false;
	for (int i = 0, j = 0; i < myStr.length(); j++)
		switch (myStr[i]) { 
		case '+': 
			if (flag && i == 0) {
				arr[j] = stoi(myStr);
				myStr.erase(0, DigitCount(arr[j]) + 1);
				arrSize -= 2; flag = false;
			} else if (bracket) {
				myStr.erase(0, 1); arr[j] = stoi(myStr);
				myStr.erase(0, DigitCount(arr[j]) + 1);
				arrSize -= 2; bracket = false; i = 0;
			} else { arr[j] = 2147483647; i = 1; }
			break;
		case '-':  
			if (flag && i == 0) {
				arr[j] = stoi(myStr);
				myStr.erase(0, DigitCount(arr[j]) + 1);
				arrSize -= 2; flag = false;
			} else if (bracket) {
				myStr.erase(0, 1); arr[j] = stoi(myStr);
				myStr.erase(0, DigitCount(arr[j]) + 1);
				arrSize -= 2; bracket = false; i = 0;
			} else { arr[j] = 2147483646; i = 1; }
			break;
		case '*': 
			if (myStr[i + 1] == '(') {
				arr[j] = 2147483645; i = 0; j++;
				arr[j] = 2147483644;
				myStr.erase(0, 2);
			} else if (arr[j - 1] == 2147483643) {
				arr[j] = 2147483645; i = 0;
				myStr.erase(0, 1);
			} else {
				arr[j] = 2147483645; i = 0;
				myStr.erase(0, 1);
			} break;
		case '(':
			if (j == 0) {
				arr[j] = 2147483644; i = 0;
				myStr.erase(0, 1); bracket = true;
			} else {
				arr[j] = 2147483644; i = 1;
				myStr.erase(1, 1); bracket = true;
			} break;
		case ')': 
			arr[j] = 2147483643; i = 0;
			myStr.erase(0, 1);
			break;
		default: 
			arr[j] = abs(stoi(myStr)); i = 0;
			if (flag || (arr[j - 1] == 2147483644 && arr[j - 2] == 2147483645))
				myStr.erase(0, DigitCount(arr[j]));
			else if (arr[j - 1] == 2147483645)
				myStr.erase(0, DigitCount(arr[j]));
			else myStr.erase(0, DigitCount(arr[j]) + 1);
			flag = false; bracket = false;
		}
	////// equation check
	Show(arr, arrSize);
	// search for multiplication 1
	for (int i = 0; i < arrSize; i++)
		if (arr[i] == 2147483645
			&& arr[i + 1] != 2147483644 && arr[i - 1] != 2147483643) {
			arr[i - 1] = arr[i - 1] * arr[i + 1];
			RemoveEntry(arr, i + 1, arrSize);
			RemoveEntry(arr, i, arrSize);
			if (arr[i - 2] == 2147483644 && arr[i] == 2147483643) {
				RemoveEntry(arr, i, arrSize);
				RemoveEntry(arr, i - 2, arrSize);
			}
		}
	////// equation check
	Show(arr, arrSize);
	// search for brackets
	int resBracket = 0, charBracket = 0; flag = true; bool flag2 = false, flag3 = false;
	for (int i = 0; i < arrSize; ) {
		if (arr[i] == 2147483644) flag2 = true;
		else if (!flag2) i++;
		if (flag2) {
			if (arr[i] == 2147483647) {
				if (flag) resBracket += arr[i - 1] + arr[i + 1];
				else resBracket += arr[i + 1];
				flag = false;
			}
			else if (arr[i] == 2147483646) {
				if (flag) resBracket += arr[i - 1] - arr[i + 1];
				else resBracket -= arr[i + 1];
				flag = false;
			}
			if (flag3) RemoveEntry(arr, i - 1, arrSize);
			else { flag3 = true; i += 2; }
		}
		if (arr[i] == 2147483643) { flag2 = false; RemoveEntry(arr, i, arrSize); i++; }
		if ((flag2 == false && flag3 == true) || i >= arrSize)
			for (int j = 0; j < arrSize; j++)
				if (arr[j] == 2147483644) {
					arr[j + 1] = resBracket;
					if (resBracket >= 0)
						RemoveEntry(arr, j, arrSize);
					else {
						arr[j] = 2147483646;
						if (arr[j - 1] == 2147483646)
							RemoveEntry(arr, j - 1, arrSize);
						else RemoveEntry(arr, j, arrSize);
					}
					flag3 = false; flag = true;
					resBracket = 0; i = 0;
					break;
				}
	}
	////// equation check
	Show(arr, arrSize);
	// search for multiplication 2
	for (int i = 0; i < arrSize; i++)
		if (arr[i] == 2147483645) {
			arr[i - 1] = arr[i - 1] * arr[i + 1];
			RemoveEntry(arr, i + 1, arrSize);
			RemoveEntry(arr, i, arrSize);
		}
	////// equation check
	Show(arr, arrSize);
	// search for addition and subtraction
	int res = 0; flag = true;
	for (int i = 0; i < arrSize; i++) {
		if (arrSize == 1) { res = arr[i]; break; }
		if (arr[i] == 2147483647) {
			if (flag) res += arr[i - 1] + arr[i + 1];
			else res += arr[i + 1];
			i++;
			flag = false;
		}
		if (arr[i] == 2147483646) {
			if (flag) res += arr[i - 1] - arr[i + 1];
			else res -= arr[i + 1];
			i++;
			flag = false;
		}
	}
	// result output
	cout << "\n\n" << str << " = " << res << endl;

	delete[]arr; 
}