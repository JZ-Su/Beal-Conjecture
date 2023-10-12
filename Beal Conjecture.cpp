#include <iostream>
#include <list>
#include <memory>

class power
{
public:
	power(int base, int index);
	~power();

private:
	int base;
	int index;
	int pow;
};

power::power(int _base,int _index):base(_base), index(_index)
{
	pow = _base;
	for (int i = _index; i > 0; i--)
	{
		pow *= pow;
	}
}

power::~power()
{
}

int main()
{
    std::cout << "Hello World!\n";
}
