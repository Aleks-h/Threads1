// Threads1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include <Windows.h>
#include <conio.h>
#include <ctime>
#include <iostream>
using namespace std;
int g_array[360]; // делаем массив для ранд чисел
int g_count = 0; // счетчик для массива
int a = 0;
HANDLE g_hSemaphor = NULL;
HANDLE hStdout;
int Lid = 0;//чтобы понять какой поток первый
static DWORD WINAPI first(void* pv)
{
	int i = 0;
	int sync = 0;
	while (i < 50) // 50 стро
	{
		WaitForSingleObject(g_hSemaphor, INFINITE);
		srand(time(NULL));
		g_count++;
		g_array[g_count] = rand() % 256; // Заполняем массив
		COORD pos;
		pos.X = 1;
		pos.Y = i;
		SetConsoleCursorPosition(hStdout, pos); // Захватываем позицию курсора для разделения на столбцы
		if (!sync)
		{
			cout << "first" << endl; // Выводим фамилию в название
			sync++;
		}
		cout << g_array[g_count]; // Выводим значение добавленные потоком
		Sleep(1000);
		i++;
		ReleaseSemaphore(g_hSemaphor, 1, NULL);
	}
	if (!Lid)
		Lid = 1;
	return 0;
}
static DWORD WINAPI Second(void* pv)
{
	int i = 0;
	int sync = 0;
	while (i < 50)
	{
		WaitForSingleObject(g_hSemaphor, INFINITE);
		srand(time(NULL));
		g_count++;
		g_array[g_count] = rand() % 1000;
		COORD pos;
		pos.X = 18;
		pos.Y = i;
		SetConsoleCursorPosition(hStdout, pos);
		if (!sync)
		{
			cout << "second" << endl; // Выводим фамилию в название
			sync++;
		}
		cout << g_array[g_count];
		Sleep(1000);
		i++;
		ReleaseSemaphore(g_hSemaphor, 1, NULL);
	}
	if (!Lid)
		Lid = 2;
	return 0;
}
static DWORD WINAPI Third(void* pv)
{
	int i = 0;
	int sync = 0;
	while (i < 50)
	{
		WaitForSingleObject(g_hSemaphor, INFINITE);
		srand(time(NULL));
		g_count++;
		g_array[g_count] = rand() % 256;
		COORD pos;
		pos.X = 36;
		pos.Y = i;
		SetConsoleCursorPosition(hStdout, pos);
		if (!sync)
		{
			cout << "third" << endl; // выводим отчество
			sync++;
		}
		cout << g_array[g_count];
		Sleep(1000);
		i++;
		ReleaseSemaphore(g_hSemaphor, 1, NULL);
	}
	if (!Lid)
		Lid = 3;
	return 0;
}
int main()
{
	DWORD dw;
	HANDLE hThreads[3]; // создаем массив потоков
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);// получаем контекст консольноо окна
	hThreads[0] = ::CreateThread(NULL, 0, first, NULL, CREATE_SUSPENDED, &dw); // создаем потоки в спящем режим
	hThreads[1] = ::CreateThread(NULL, 0, Second, NULL, CREATE_SUSPENDED, &dw);
	hThreads[2] = ::CreateThread(NULL, 0, Third, NULL, CREATE_SUSPENDED, &dw);
	g_hSemaphor = CreateSemaphore(NULL, 2, 3, NULL);
	::ResumeThread(hThreads[0]); //будим
	::ResumeThread(hThreads[1]);
	::ResumeThread(hThreads[2]);
	::WaitForMultipleObjects(3, hThreads, TRUE, INFINITE); // Ждем окончания работы потоко
	::CloseHandle(hThreads[0]);
	::CloseHandle(hThreads[1]);
	::CloseHandle(hThreads[2]);
	::CloseHandle(g_hSemaphor);
	switch (Lid) // выводим лидирующий
	{
	case 1: cout << "\n 1 thread finished first!\n";
		break;
	case 2: cout << "\n 2 thread finished first!\n";
		break;
	case 3: cout << "\n 3 thread finished first!\n";
		break;
	default: break;
	}
	cout << endl << "Press Any Key...";
	_getch();
	return 0;
}
