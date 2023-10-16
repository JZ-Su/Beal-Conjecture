#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <iomanip>

#include <ctime>

#define MinBase 2
#define MaxBase 128
#define MinIndex 3
#define MaxIndex 20

using namespace std;

const vector<int> primeNum = { 2,3,5,7,11,13,17,19,23,29,31,37,41 };

class Power
{
public:
	int base;
	int index;
	int secondDigit;
	unsigned long long firstDigit;
};

class BNT
{
public:
	BNT(int A, int B, int C, int x, int y, int z);
	int sum;
	int A, B, C;
	int x, y, z;
};

BNT::BNT(int A, int B, int C, int x, int y, int z) : A(A), B(B), C(C), x(x), y(y), z(z)
{
	sum = A + B + C + x + y + z;
}

template <typename T> T pow(int base, int index) {
	T pow = base;
	for (int i = index; i > 1; i--) {
		pow *= base;
	}
	return pow;
}

bool matchBNT(Power x, Power y, Power z) {
	if (x.firstDigit + y.firstDigit == z.firstDigit) {
		if (x.secondDigit + y.secondDigit == z.secondDigit) {
			return true;
		}
		else if (x.secondDigit + y.secondDigit - 1 == z.secondDigit && x.firstDigit > z.firstDigit && y.firstDigit > z.firstDigit) {
			return true;
		}
	}
	return false;
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

void printBNT(BNT bnt) {
	cout << bnt.sum << ": " << bnt.A << ", " << bnt.x << ", " << bnt.B << ", " << bnt.y << ", " << bnt.C << ", " << bnt.z << endl;
}

int main() {
	vector<Power> hashTable[10];
	vector<BNT> BNTs;
	Power po;
	std::time_t beginTime = std::time(nullptr);
	for (int i = MinBase; i <= MaxBase; i++) {
		for (int j = MinIndex; j <= MaxIndex; j++) {
			po.base = i;
			po.index = j;
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
			cout << "base: " << setw(3) << po.base << "\t index: " << setw(2) << po.index << "\t secondDigit: " << setw(10) << po.secondDigit << "\t firstDigit: " << po.firstDigit << endl;
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < hashTable[i].size(); j++) {
			for (int x = 0; x < 10; x++) {
				for (int y = 0; y < hashTable[x].size(); y++) {
					int mantissa = (i + x) % 10;
					for (int m = 0; m < hashTable[mantissa].size(); m++) {
						if (matchBNT(hashTable[i][j], hashTable[x][y], hashTable[mantissa][m])) {
							cout << hashTable[i][j].base << " " << hashTable[i][j].index << " " << hashTable[x][y].base << " " << hashTable[x][y].index << " " << hashTable[mantissa][m].base << " " << hashTable[mantissa][m].index << endl;
							BNTs.push_back(BNT(hashTable[i][j].base, hashTable[x][y].base, hashTable[mantissa][m].base, hashTable[i][j].index, hashTable[x][y].index, hashTable[mantissa][m].index));
						}
					}
				}
			}
		}
	}

	quickSort(BNTs, 0, BNTs.size() - 1);

	for (int i = 0; i < BNTs.size(); i++) {
		printBNT(BNTs[i]);
	}

	// Question 1
	int count = 0, bnt = 0;
	cout << "Question 1: " << endl;
	for (int i = 0; i < BNTs.size(); i++) {
		if (BNTs[i].sum != bnt) {
			bnt = BNTs[i].sum;
			count++;
			printBNT(BNTs[i]);
			if (count == 5) break;
		}
	}

	// Question 2
	cout << "Question 2: " << endl;
	count = 0, bnt = 0;
	for (int i = 0; i < BNTs.size(); i++) {
		if (BNTs[i].sum != bnt ) {
			bnt = BNTs[i].sum;
			if (isPrime(bnt)) {
				count++;
				printBNT(BNTs[i]);
				if (count == 5) break;
			}
		}
	}

	// Question 3
	cout << "Question 3: " << endl;
	for (int i = 0; i < BNTs.size(); i++) {
		if (BNTs[i].A >= 3 && BNTs[i].B >= 3 && BNTs[i].C >= 3 && BNTs[i].A <= 20 && BNTs[i].B <= 20 && BNTs[i].C <= 20) {
			printBNT(BNTs[i]);
		}
	}

	// Question 5
	cout << "Question 5: " << endl;
	count = 0, bnt = 0;
	for (int i = 0; i < BNTs.size(); i++) {
		if (BNTs[i].sum != bnt) {
			bnt = BNTs[i].sum;
			if (!isPrime(bnt)) {
				count++;
				printBNT(BNTs[i]);
				if (count == 10) break;
			}
		}
	}

	// Question 7
	int low, high;

	std::time_t endTime = std::time(nullptr);

	cout << endl << "Costing time: " << endTime - beginTime << "s" << endl;
	return 0;
}
