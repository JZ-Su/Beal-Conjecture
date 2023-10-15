#include <iostream>
#include <vector>
#include <cmath>
#define MinBase 2
#define MaxBase 80
#define MinIndex 3
#define MaxIndex 15

using namespace std;

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
	if (x.firstDigit + y.firstDigit == z.firstDigit)
	{
		if (x.secondDigit + y.secondDigit == z.secondDigit) {
			return true;
		}
		else if (x.secondDigit + y.secondDigit - 1 == z.secondDigit && x.firstDigit > z.firstDigit && y.firstDigit > z.firstDigit)
		{
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

int main() {
	vector<Power> hashTable[10];
	vector<BNT> BNTs;
	Power po;
	for (int i = MinBase; i <= MaxBase; i++) {
		for (int j = MinIndex; j <= MaxIndex; j++) {
				po.base = i;
				po.index = j;
				if (j * log2(i) >= 64.0) {
					po.secondDigit = floor(std::pow(2, j * log2(i) - 64));
					po.firstDigit = pow<unsigned long long>(i, j);
					hashTable[(po.firstDigit % 10 + static_cast<unsigned long long>(po.secondDigit) * 6) % 10].push_back(po);
				}
				else {
					po.secondDigit = 0;
					po.firstDigit = pow<unsigned long long>(i, j);
					hashTable[po.firstDigit % 10].push_back(po);
				}
				cout << "base: " << po.base << "\t index: " << po.index << "\t oversize: " << po.secondDigit << "\t power: " << po.firstDigit << endl;
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < hashTable[i].size(); j++) {
			for (int x = 0; x < 10; x++) {
				for (int y = 0; y < hashTable[x].size(); y++) {
					int mantissa = (i + x) % 10;
					for (int m = 0; m < hashTable[mantissa].size(); m++) {
						if (matchBNT(hashTable[i][j], hashTable[x][y], hashTable[mantissa][m])) {
							BNTs.push_back(BNT(hashTable[i][j].base, hashTable[x][y].base, hashTable[mantissa][m].base, hashTable[i][j].index, hashTable[x][y].index, hashTable[mantissa][m].index));
						}
					}
				}
			}
		}
	}

	quickSort(BNTs, 0, BNTs.size() - 1);

	for (int i = 0; i < BNTs.size(); i++) {
		cout << BNTs[i].sum << ": " << BNTs[i].A << ", " << BNTs[i].x << ", " << BNTs[i].B << ", " << BNTs[i].y << ", " << BNTs[i].C << ", " << BNTs[i].z << endl;
	}

	return 0;
}
