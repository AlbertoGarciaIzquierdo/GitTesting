// Test3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

class Duck {
public:
	int getSum(int a, int b)
	{
		int bias = 3;
		int sum;
		auto lf1 = [=](int x, int y) { return x + y; };
		auto lf2 = [&](int x, int y) { return x + y; };
		auto lf3 = [bias](int x, int y) { return x + y; };
		auto lf4 = [&bias](int x, int y) { return x + y; };

		// Calculate the sum using the lambda function
		std::cout << lf1(a,bias) << " " << lf2(a, bias) << " " << lf3(a, bias) << " " << lf4(a, bias);

		return(a+b);
	}
};

class RubberDuck : public Duck {

};

int main()
{
	Duck duck;
	duck.getSum(1,2);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
