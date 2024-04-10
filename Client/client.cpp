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
				throw runtime_error("������: ������������ ���� ������!");

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
	if (WSAStartup(wVersionRequested, &wsaData) != 0) { // �������� �� ����������� ���������� winsock2.h
		cout << "������!\n���������� �� �����������!" << endl;
		return 0;
	}

	while (true) {
		SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
		// �������� ������ � ����� ������� 
		sockaddr_in dest_addr;
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(1280);
		dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		if (connect(s, (sockaddr*)&dest_addr, sizeof(dest_addr)) == SOCKET_ERROR)
		{
			cout << "������ �����������!" << endl;
			closesocket(s);
			WSACleanup();
			return 1;
		}

		char buf[1024];

		while (true)
		{
			system("cls");

			cout << "===========������ � ��������==========" << endl;
			cout << "|   0. �����.                        |" << endl;
			cout << "|   1. ��������.                     |" << endl;
			cout << "|   2. ����������.                   |" << endl;
			cout << "|   3. ��������������.               |" << endl;
			cout << "|   4. ��������.                     |" << endl;
			cout << "|   5. ����� �� ����.                |" << endl;
			cout << "======================================" << endl;
			cout << endl << "�������� ��������: ";

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
					cerr << "������!" << endl;
				}

				system("pause");
				break;
			}
			case 2:
			{
				system("cls");
				send(s, "2\0", 2, NULL);

				string data;

				cout << "������� �����-���������: ";
				cin.ignore(10, '\n');
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				cout << "������� ����: ";
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				cout << "������� ���� (01 01 2024): ";
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				recv(s, buf, sizeof(buf), NULL);

				//��������� ������
				cout << endl << (buf[0] == '1' ? "������ ������� ���������" : "������!") << endl << endl;


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
					cerr << "������!" << endl;
				}

				cout << endl << "�������� ����� ������ ��� ��������������: ";
				int ch = choice();
				string data = to_string(ch);

				send(s, data.c_str(), data.size(), NULL);

				cout << "������� �����-���������: ";
				cin.ignore(10, '\n');
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				cout << "������� ����: ";
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				cout << "������� ���� (01 01 2024): ";
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				bytesReceived = recv(s, buf, sizeof(buf), NULL);
				if (bytesReceived != -1) {
					buf[bytesReceived] = '\0';
					cout << endl << buf << endl << endl;
				}
				else {
					cerr << "������!" << endl;
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
					cerr << "������!" << endl;
				}

				cout << endl << "�������� ����� ������ ��� ��������: ";
				int ch = choice();
				string data = to_string(ch);

				send(s, data.c_str(), data.size(), NULL);

				bytesReceived = recv(s, buf, sizeof(buf), NULL);
				if (bytesReceived != -1) {
					buf[bytesReceived] = '\0';
					cout << endl << buf << endl << endl;
				}
				else {
					cerr << "������!" << endl;
				}

				system("pause");
				break;
			}
			case 5:
			{
				system("cls");
				send(s, "5\0", 2, NULL);

				string data;

				cout << "������� ���� (01 01 2024): ";
				cin.ignore(10, '\n');
				getline(cin, data);
				send(s, data.c_str(), data.size() + 1, NULL);

				int bytesReceived = recv(s, buf, sizeof(buf), NULL);
				if (bytesReceived != -1) {
					buf[bytesReceived] = '\0';
					cout << endl << buf << endl << endl;
				}
				else {
					cerr << "������!" << endl;
				}

				system("pause");
				break;
			}
			default: { cout << "�������� �����!" << endl; system("pause"); system("cls"); }
			}
		}
	}
}