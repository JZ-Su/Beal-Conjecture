#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <iomanip>
#include <Windows.h>
#include <fstream>

#include "class.h"

#include <ctime>
#include <future>
#include <chrono>
#include <limits>

#define MinBase 2
#define MaxBase 512
#define MinExponent 3
#define MaxExponent 95

using namespace std;

class BNT: public bnt
{
public:
	BNT(int A, int B, int C, int x, int y, int z) :bnt(A, B, C, x, y, z) {};
	void print() {
		cout << this->sum << ": " << this->A << ", " << this->x << ", " << this->B << ", " << this->y << ", " << this->C << ", " << this->z << endl;
	};
};

const vector<int> primeNum = { 2,3,5,7,11,13,17,19,23,29,31,37,41 };
int loading = 0;

template <typename T> T pow(int base, int index) {
	T pow = base;
	for (int i = index; i > 1; i--) {
		pow *= base;
	}
	return pow;
}

int partition(vector<BNT>&, int, int);
void quickSort(vector<BNT>&, int, int);
bool isPrime(int);
void loadingProgress();
void Question1(vector<BNT>&);
void Question2(vector<BNT>&);
void Question3(vector<BNT>&);
void Question4(vector<BNT>&);
void Question5(vector<BNT>&);
void Question6();
void Question7(vector<BNT>&);
void Question8(vector<BNT>&);
void PrintAll(vector<BNT>&);

vector<BNT> matchBNT(int i, vector<Power> hashTable[]) {
	vector<BNT> BNTlist;
	for (int j = 0; j < hashTable[i].size(); j++) {
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < hashTable[x].size(); y++) {
				if (_Gcd(hashTable[i][j].base, hashTable[x][y].base) == 1) continue;
				int mantissa = (i + x) % 10;
				for (int m = 0; m < hashTable[mantissa].size(); m++) {
					if ((hashTable[i][j] + hashTable[x][y]) == hashTable[mantissa][m]) {
						BNTlist.push_back(BNT(hashTable[i][j].base, hashTable[x][y].base, hashTable[mantissa][m].base, hashTable[i][j].exponent, hashTable[x][y].exponent, hashTable[mantissa][m].exponent));
					}
				}
			}
		}
	}
	loading++;
	return BNTlist;
};

int main() {
	std::time_t beginTime = time(nullptr);
	thread ld(loadingProgress);
	vector<Power> hashTable[10];
	vector<BNT> BNTs;
	Power po;
	for (int i = MinBase; i <= MaxBase; i++) {
		for (int j = MinExponent; j <= MaxExponent; j++) {
			po.base = i;
			po.exponent = j;
			if (j * log2(i) >= 64.0) {
				po.secondDigit = floor(std::pow(2, j * log2(i) - 64));
				if (po.secondDigit <= 0) break;
				po.firstDigit = pow<unsigned long long>(i, j);
				hashTable[(po.firstDigit % 10 + static_cast<unsigned long long>(po.secondDigit) * 6) % 10].push_back(po);
			}
			else {
				po.secondDigit = 0;
				po.firstDigit = pow<unsigned long long>(i, j);
				hashTable[po.firstDigit % 10].push_back(po);
			}
		}
	}
	loading++;

	future<vector<BNT>> BNTlist[10];
	for (int i = 0; i < 10; i++) {
		BNTlist[i] = async(matchBNT, i, hashTable);
	}

	bool flag;
	while (true) {
		flag = true;
		for (int i = 0; i < 10; i++) {
			if (BNTlist[i].wait_for(chrono::seconds(1)) != future_status::ready) {
				flag = false;
				break;
			}
		}
		if (flag) {
			break;
		}
	}
	for (int i = 0; i < 10; i++) {
		for (const auto& value : BNTlist[i].get()) {
			BNTs.push_back(value);
		}
		vector<Power>().swap(hashTable[i]);
	}

	quickSort(BNTs, 0, BNTs.size() - 1);
	loading++;
	ld.join();

	std::time_t endTime = time(nullptr);
	cout << endl << "Costing time: " << endTime - beginTime << "s" << endl << endl;

	char questionNum = '0';
	cout << "Question 1: Find the first five distinct and lowest BNTs." << endl;
	cout << "Question 2: Find the first five BNTs that are prime numbers." << endl;
	cout << "Question 3: Find the BNTs where A, B, C belong to [3, 20] and x, y, z belong to [3, 15]." << endl;
	cout << "Question 4: Write the outputs to file (Filename: BNTs.txt)." << endl;
	cout << "Question 5: Find the first ten BNTs that are composite numbers." << endl;
	cout << "Question 6: Find the BNTs that are square numbers between 1000 and 100000." << endl;
	cout << "Question 7: Input a minimum number(>=" << BNTs[0].sum << ") and a maximum number(<=" << BNTs[BNTs.size() - 1].sum << ") to generate an ASCII histogram." << endl;
	cout << "Question 8: Find the highest BNT. (A, B and C must all be greater than 2)" << endl;
	cout << "Enter 9 to show all the BNTs." << endl;

	do {
		cout << endl << "Input a question number (1~9, 0 to exit): ";
		cin.get(questionNum);
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		cout << endl;
		switch (questionNum)
		{
		case '1':
			Question1(BNTs);
			break;
		case '2':
			Question2(BNTs);
			break;
		case '3':
			Question3(BNTs);
			break;
		case '4':
			Question4(BNTs);
			break;
		case '5':
			Question5(BNTs);
			break;
		case '6':
			Question6();
			break;
		case '7':
			Question7(BNTs);
			break;
		case '8':
			Question8(BNTs);
			break;
		case '9':
			PrintAll(BNTs);
			break;
		case '0':
			return 0;
		default:
			cout << "Please input a valid number! " << endl;
			break;
		}
	} while (true);

	return 0;
}

int partition(vector<BNT>& arr, int low, int high) {
	int pivot = arr[high].sum;
	int i = low - 1;
	for (int j = low; j < high; j++) {
		if (arr[j].sum < pivot) {
			i++;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[high]);
	return i + 1;
}

void quickSort(vector<BNT>& arr, int low, int high) {
	if (low < high) {
		int pivotIndex = partition(arr, low, high);
		quickSort(arr, low, pivotIndex - 1);
		quickSort(arr, pivotIndex + 1, high);
	}
}

bool isPrime(int num) {
	for (int i = 0; i < primeNum.size(); i++) {
		if (num % primeNum[i] == 0) {
			return false;
		}
		else if (num / primeNum[i] < primeNum[i]) {
			return true;
		}
	}
	return true;
}

void loadingProgress() {
	do
	{
		cout << "\rLoading.   [";
		for (int i = 0; i < loading; i++) {
			cout << "o";
		}
		for (int i = loading; i < 12; i++) {
			cout << ".";
		}
		cout << "]";
		Sleep(500);

		cout << "\rLoading..  [";
		for (int i = 0; i < loading; i++) {
			cout << "o";
		}
		for (int i = loading; i < 12; i++) {
			cout << ".";
		}
		cout << "]";
		Sleep(500);

		cout << "\rLoading... [";
		for (int i = 0; i < loading; i++) {
			cout << "o";
		}
		for (int i = loading; i < 12; i++) {
			cout << ".";
		}
		cout << "]";
		Sleep(500);
	} while (loading != 12);
	cout << "\rLoading Completed!         " << endl;
}

void Question1(vector<BNT>& BNTs) {
	int count = 0, bnt = 0;
	cout << "Question 1: " << endl;
	for (int i = 0; i < BNTs.size(); i++) {
		if (BNTs[i].sum != bnt) {
			bnt = BNTs[i].sum;
			count++;
			BNTs[i].print();
			if (count == 5) break;
		}
	}
};

void Question2(vector<BNT>& BNTs) {
	int count = 0, bnt = 0;
	cout << "Question 2: " << endl;
	for (int i = 0; i < BNTs.size(); i++) {
		if (BNTs[i].sum != bnt) {
			bnt = BNTs[i].sum;
			if (isPrime(bnt)) {
				count++;
				BNTs[i].print();
				if (count == 5) break;
			}
		}
	}
};

void Question3(vector<BNT>& BNTs) {
	cout << "Question 3: " << endl;
	for (int i = 0; i < BNTs.size(); i++) {
		if (BNTs[i].A >= 3 && BNTs[i].B >= 3 && BNTs[i].C >= 3 && BNTs[i].A <= 20 && BNTs[i].B <= 20 && BNTs[i].C <= 20 && BNTs[i].x <= 15 && BNTs[i].y <= 15 && BNTs[i].z <= 15) {
			BNTs[i].print();
		}
	}
};

void Question4(vector<BNT>& BNTs) {
	ofstream file;
	file.open("BNTs.txt", ios::out | ios::trunc);

	int count = 0, bnt = 0;
	file << "Question 1: " << endl;
	for (int i = 0; i < BNTs.size(); i++) {
		if (BNTs[i].sum != bnt) {
			bnt = BNTs[i].sum;
			count++;
			file << BNTs[i].sum << ": " << BNTs[i].A << ", " << BNTs[i].x << ", " << BNTs[i].B << ", " << BNTs[i].y << ", " << BNTs[i].C << ", " << BNTs[i].z << endl;
			if (count == 5) break;
		}
	}

	file << endl;

	file << "Question 2: " << endl;
	count = 0;
	for (int i = 0; i < BNTs.size(); i++) {
		if (BNTs[i].sum != bnt) {
			bnt = BNTs[i].sum;
			if (isPrime(bnt)) {
				count++;
				file << BNTs[i].sum << ": " << BNTs[i].A << ", " << BNTs[i].x << ", " << BNTs[i].B << ", " << BNTs[i].y << ", " << BNTs[i].C << ", " << BNTs[i].z << endl;
				if (count == 5) break;
			}
		}
	}

	file << endl;

	file << "Question 3: " << endl;
	for (int i = 0; i < BNTs.size(); i++) {
		if (BNTs[i].A >= 3 && BNTs[i].B >= 3 && BNTs[i].C >= 3 && BNTs[i].A <= 20 && BNTs[i].B <= 20 && BNTs[i].C <= 20 && BNTs[i].x <= 15 && BNTs[i].y <= 15 && BNTs[i].z <= 15) {
			file << BNTs[i].sum << ": " << BNTs[i].A << ", " << BNTs[i].x << ", " << BNTs[i].B << ", " << BNTs[i].y << ", " << BNTs[i].C << ", " << BNTs[i].z << endl;
		}
	}

	file.close();

	cout << "Writing Completed!" << endl;
};

void Question5(vector<BNT>& BNTs) {
	cout << "Question 5: " << endl;
	int count = 0, bnt = 0;
	for (int i = 0; i < BNTs.size(); i++) {
		if (BNTs[i].sum != bnt) {
			bnt = BNTs[i].sum;
			if (!isPrime(bnt)) {
				count++;
				BNTs[i].print();
				if (count == 10) break;
			}
		}
	}
};

void Question6() {
	int x = 0;
	for (int i = 32; i <= 316; i++) {
		if (i % 3 == 0) {
			// 2^(4x-1)+2^(4x-1)=16^x
			// BNT=9x+18
			x = (pow(i, 2) - 18) / 9;
			cout << i << "^2= " << pow(i, 2) << ": 2, " << 4 * x - 1 << ", 2, " << 4 * x - 1 << ", 16, " << x << endl;
		}
		else {
			// 2^x+2^x=2^(x+1)
			// BNT=3x+7
			x = (pow(i, 2) - 7) / 3;
			cout << i << "^2= " << pow(i, 2) << ": 2, " << x << ", 2, " << x << ", 2, " << x + 1 << endl;
		}
	}
};

void Question7(vector<BNT>& BNTs) {
	int minimum, maximum;
	for (;;) {
		cout << "Input a minimum number (" << BNTs[0].sum << "~" << BNTs[BNTs.size() - 1].sum << "): ";
		cin >> minimum;
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		if (cin.fail()) {
			cout << "Please input a valid value. " << endl;
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			continue;
		}
		if (minimum < BNTs[0].sum || minimum > BNTs[BNTs.size() - 1].sum) {
			cout << "Please input a valid value." << endl;
		}
		else break;
	}
	for (;;) {
		cout << "Input a maximum number (" << minimum << "~" << BNTs[BNTs.size() - 1].sum << "): ";
		cin >> maximum;
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		if (cin.fail()) {
			cout << "Please input a valid value. " << endl;
			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			continue;
		}
		if (maximum < minimum || maximum > BNTs[BNTs.size() - 1].sum) {
			cout << "Please input a valid value. " << endl;
		}
		else break;
	}
	int counter[1500] = { 0 };
	for (int i = 0; i < BNTs.size(); i++) {
		counter[BNTs[i].sum]++;
	}
	cout << "BNT | count | histogram " << endl;
	for (int i = minimum; i <= maximum; i++) {
		//if (!counter[i]) continue;
		cout << left << setw(3) << i << " | " << left << setw(5) << counter[i] << " | ";
		for (int j = 0; j < counter[i]; j++) {
			cout << "o";
		}
		cout << endl;
	}
};

void Question8(vector<BNT>& BNTs) {
	int i = BNTs.size() - 1;
	while (BNTs[i].A == 2 || BNTs[i].B == 2 || BNTs[i].C == 2) {
		i--;
		if (i == 0) break;
	}
	BNTs[i].print();
};

void PrintAll(vector<BNT>& BNTs) {
	for (int i = 0; i < BNTs.size(); i++) {
		BNTs[i].print();
	}
}
