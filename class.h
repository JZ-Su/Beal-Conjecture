#pragma once

class Power
{
public:
	int base;
	int exponent;
	int secondDigit;
	unsigned long long firstDigit;
	Power() {
		base = 0;
		exponent = 0;
		secondDigit = 0;
		firstDigit = 0;
	};
	Power operator+ (const Power& p) {
		Power res;
		res.firstDigit = this->firstDigit + p.firstDigit;
		if (res.firstDigit < this->firstDigit && res.firstDigit < p.firstDigit) {
			res.secondDigit = this->secondDigit + p.secondDigit + 1;
		}
		else
		{
			res.secondDigit = this->secondDigit + p.secondDigit;
		}
		return res;
	}
	bool operator == (const Power& p) {
		if (this->firstDigit == p.firstDigit && this->secondDigit == p.secondDigit) return true;
		else return false;
	}
};

class bnt
{
public:
	int sum;
	int A, B, C;
	int x, y, z;
	bnt(int A, int B, int C, int x, int y, int z) : A(A), B(B), C(C), x(x), y(y), z(z) {
		sum = A + B + C + x + y + z;
	};
	virtual void print() {};
};
