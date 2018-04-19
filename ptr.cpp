#include <iostream>

int func(int *ptr, int &b)
{
	char str[] = "dsdss";
	str[b] = 'X';
	*ptr++;
	int val = *ptr;
	val++;
	ptr = &val;
	b++;
	std::cout << str << std::endl;
}

int main()
{
	int a = 1;
	int *pointer = &a;
	func(pointer, a);
	std::cout << "pointer: " << pointer << ", a: " << a << std::endl;
return 0;
}

