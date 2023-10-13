#include <iostream>
#include <vector>
#define MinBase 2
#define MaxBase 20
#define MinIndex 3
#define MaxIndex 15

using namespace std;

class Power
{
public:
	int base;
	int index;
	unsigned long long pow1;
	unsigned long long pow2;
};

class BNT : public Power
{
public:
	BNT(int A, int B, int C, int x, int y, int z);
	int sum;
	int A, B, C;
	int x, y, z;
};

BNT::BNT(int A, int B, int C, int x, int y, int z): A(A), B(B), C(C), x(x), y(y), z(z)
{
	sum = A + B + C + x + y + z;
}

template <typename T> T power(int base, int index) {
	T pow = base;
	for (int i = index; i > 1; i--) {
		pow *= base;
	}
	return pow;
}

int main() {
	vector<Power> hashTable[10];
	vector<BNT> BNTs;
	Power po;
	for (int i = MinBase; i <= MaxBase; i++) {
		for (int j = MinIndex; j <= MaxIndex; j++) {
			if (i == MaxBase && j == MaxIndex) {
				po.base = i;
				po.index = j;
				po.pow1 = power<unsigned long long>(i, 5);
				po.pow2 = power<unsigned long long>(i, j - 5);
				hashTable[0].push_back(po);
			}
			else
			{
				po.base = i;
				po.index = j;
				po.pow1 = power<unsigned long long>(i, j);
				po.pow2 = 0;
				hashTable[po.pow1 % 10].push_back(po);
			}
		}
	}
	for (auto element : hashTable[0]) {
		cout << element.pow << endl;
	}
	return 0;
}
