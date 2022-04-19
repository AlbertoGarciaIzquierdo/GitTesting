// Tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>

static void CheckThread() {
    std::cout << "I'm running" << std::endl;
}
static void ChangeThread() {
    std::cout << "Now I'm doing different things" << std::endl;
}

int main()
{
    std::cout << "Hello World!\n";
    std::thread t[5];
    t[0] = std::thread{ CheckThread };

    t[0].join();

    std::cout << "T0: " << t[0].get_id() << " T1: " << t[1].get_id() << std::endl;
    t[0].detach();
    t[0] = std::thread{ ChangeThread };
    t[0].join();
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
