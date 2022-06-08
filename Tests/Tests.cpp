// Tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <windows.h>
#include <ctime>
#include <math.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>

//#define DeltaTime 0.033333333333
#define DeltaTime 0.01666666666

void clear_screen()
{
	COORD cursorPosition;	cursorPosition.X = 0;	cursorPosition.Y = 0;	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}
void cls()
{
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::fflush(stdout);

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}

class Munequito {
public:
	float gravity = 1;
	int jumpForce;
	struct vector2 {
		float x, y;
	};
	vector2 force{0.0f, 1.5f};
	COORD direction{ 1,1 };
	COORD pos{ 0,0 };

	void Setup(int a, int b, int jump) {
		pos.X = a; pos.Y = b;
		jumpForce = jump;
	}

	float t = 0;
	float y = 0;
	// y = 5t - t^2
	void Jump() {
		if (y >= 0) {
			y = jumpForce * t - std::pow(t, 2);
			t += DeltaTime * 4;
			pos.Y = 28 - y;
		}
		if (pos.X > 114) { direction.X = -1; } if (pos.X < 2) { direction.X = 1; }
		pos.X += 1 * direction.X;
		if (y < 0) { t = 0; y = 0; }
	}

	void Physics() {
		if (pos.Y < 28)
			pos.Y += gravity;
		if (force.y > 0)
			this->force.y -= gravity * DeltaTime;
		if (force.y > gravity) {
			pos.Y -= force.y;
		}
	}

	void Movement() {
		if (pos.X > 114) { direction.X = -1; } if (pos.X < 2) { direction.X = 1; }
		if (pos.Y > 28) { direction.Y = -1; } if (pos.Y < 2) { direction.Y = 1; }
		pos.X += 1 * direction.X; pos.Y += 1 * direction.Y;
	}

	void Draw(HANDLE& console) {	
		SetConsoleCursorPosition(console, pos);
		std::printf("O");
	}
};

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ShowConsoleCursor(false);
	
	Munequito munequitos[7];
	srand(time(NULL));

	for (int i = 0; i < sizeof(munequitos) / sizeof(munequitos[0]); i++) {
		munequitos[i].Setup(rand() % 115, 28, rand() % 6 + 5);
	}
	while (true) {
		for (int i = 0; i < sizeof(munequitos) / sizeof(munequitos[0]); i++) {
			munequitos[i].Jump();
			munequitos[i].Draw(hConsole);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
		system("CLS");
		//clear_screen();
		//cls();
	}
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
