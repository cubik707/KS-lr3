#include<stdio.h>
#include<iostream>
#include<winsock2.h>
#include<Windows.h>
#include<string>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

int choice()
{
	int choice;
	while (true)
	{
		try
		{
			cin >> choice;

			if (cin.fail())
				throw runtime_error("Ошибка: некорректный ввод данных!");

			return choice;
		}
		catch (const runtime_error& ex)
		{
			cerr << ex.what() << endl;
			cin.clear();
			cin.ignore(10, '\n');
		}
	}
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) { // Проверка на подключение библиотеки winsock2.h
		cout << "ОШИБКА!\nБиблиотека не загрузилась!" << endl;
		return 0;
	}

	while (true) {
		SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
		// указание адреса и порта сервера 
		sockaddr_in dest_addr;
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(1280);
		dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		if (connect(s, (sockaddr*)&dest_addr, sizeof(dest_addr)) == SOCKET_ERROR)
		{
			cout << "Ошибка подключения!" << endl;
			closesocket(s);
			WSACleanup();
			return 1;
		}

		char buf[1024];

		while (true)
		{
			system("cls");

			cout << "===========РАБОТА С СЕРВЕРОМ==========" << endl;
			cout << "|   0. Выход.                        |" << endl;
			cout << "|   1. Просмотр.                     |" << endl;
			cout << "|   2. Добавление.                   |" << endl;
			cout << "|   3. Редактирование.               |" << endl;
			cout << "|   4. Удаление.                     |" << endl;
			cout << "|   5. Поиск по дате.                |" << endl;
			cout << "======================================" << endl;
			cout << endl << "Выберите действие: ";

			switch (choice())
			{
			case 0: send(s, "0\0", 2, NULL);  closesocket(s); WSACleanup(); return 0;
			case 1:
			{
				system("cls");
				send(s, "1\0", 2, NULL);

				int bytesReceived = recv(s, buf, sizeof(buf), NULL);
				if (bytesReceived != -1) {
					buf[bytesReceived] = '\0';
					cout << buf << endl;
				}
				else {
					cerr << "Ошибка!" << endl;
				}

				system("pause");
				break;
			}
			case 2:
			{
				system("cls");
				send(s, "2\0", 2, NULL);

				string data;

				cout << "Введите завод-поставщик: ";
				cin.ignore(10, '\n');
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				cout << "Введите цену: ";
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				cout << "Введите дату (01 01 2024): ";
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				recv(s, buf, sizeof(buf), NULL);

				//Проверить работу
				cout << endl << (buf[0] == '1' ? "Данные успешно добавлены" : "Ошибка!") << endl << endl;


				system("pause");
				break;
			}
			case 3:
			{
				system("cls");
				send(s, "3\0", 2, NULL);

				int bytesReceived = recv(s, buf, sizeof(buf), NULL);
				if (bytesReceived != -1) {
					buf[bytesReceived] = '\0';
					cout << buf << endl;
				}
				else {
					cerr << "Ошибка!" << endl;
				}

				cout << endl << "Выберите номер записи для редактирования: ";
				int ch = choice();
				string data = to_string(ch);

				send(s, data.c_str(), data.size(), NULL);

				cout << "Введите завод-поставщик: ";
				cin.ignore(10, '\n');
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				cout << "Введите цену: ";
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				cout << "Введите дату (01 01 2024): ";
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				bytesReceived = recv(s, buf, sizeof(buf), NULL);
				if (bytesReceived != -1) {
					buf[bytesReceived] = '\0';
					cout << endl << buf << endl << endl;
				}
				else {
					cerr << "Ошибка!" << endl;
				}

				system("pause");
				break;
			}
			case 4:
			{
				system("cls");
				send(s, "4\0", 2, NULL);

				int bytesReceived = recv(s, buf, sizeof(buf), NULL);
				if (bytesReceived != -1) {
					buf[bytesReceived] = '\0';
					cout << buf << endl;
				}
				else {
					cerr << "Ошибка!" << endl;
				}

				cout << endl << "Выберите номер записи для удаления: ";
				int ch = choice();
				string data = to_string(ch);

				send(s, data.c_str(), data.size(), NULL);

				bytesReceived = recv(s, buf, sizeof(buf), NULL);
				if (bytesReceived != -1) {
					buf[bytesReceived] = '\0';
					cout << endl << buf << endl << endl;
				}
				else {
					cerr << "Ошибка!" << endl;
				}

				system("pause");
				break;
			}
			case 5:
			{
				system("cls");
				send(s, "5\0", 2, NULL);

				string data;

				cout << "Введите дату (01 01 2024): ";
				cin.ignore(10, '\n');
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				int bytesReceived = recv(s, buf, sizeof(buf), NULL);
				if (bytesReceived != -1) {
					buf[bytesReceived] = '\0';
					cout << endl << buf << endl << endl;
				}
				else {
					cerr << "Ошибка!" << endl;
				}

				system("pause");
				break;
			}
			default: { cout << "Неверный выбор!" << endl; system("pause"); system("cls"); }
			}
		}
	}
}