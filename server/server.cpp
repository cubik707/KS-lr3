#include<stdio.h>
#include<iostream>
#include <winsock2.h> //для работы с сокетами
#include<locale.h>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

const string FILE_OF_DETAILS = "Details.txt";

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

class Detail
{
private:
	string factory;
	float price;
	int day;
	int month;
	int year;
public:
	Detail() : factory(""), price(0), day(0), month(0), year(0) {}
	Detail(string factory, float price, int day, int month, int year) : factory(factory), price(price), day(day), month(month), year(year) {}
	Detail(const Detail& obj) : factory(obj.factory), price(obj.price), day(obj.day), month(obj.month), year(obj.year) {}

	void setFactory() { cout << "Введите завод-поставщик: "; getline(cin, factory); }
	void setPrice()
	{
		cout << "Введите стоимость: ";
		while (true)
		{
			try
			{
				cin >> price;

				if (cin.fail())
					throw runtime_error("Ошибка: некорректный ввод данных!");

				break;
			}
			catch (const runtime_error& ex)
			{
				cerr << ex.what() << endl;
				cin.clear();
				cin.ignore(10, '\n');
			}
		}
	}
	void setDay()
	{
		cout << "День: ";
		this->day = choice();
	}

	void setMonth()
	{
		cout << "Месяц: ";
		this->month = choice();
	}

	void setYear()
	{
		cout << "Год: ";
		this->year = choice();
	}



	const string getFactory() { return this->factory; }
	const float getPrice() { return this->price; }
	const int getDay() { return this->day; }
	const int getMonth() { return this->month; }
	const int getYear() { return this->year; }
};

void addFile(vector<Detail> vec_of_details)
{
	ofstream fadd(FILE_OF_DETAILS, ios::app);

	int i = 0;
	if (vec_of_details.size() > 0)
	{
		i = vec_of_details.size() - 1;
		if (i > 0)
		{
			fadd << endl;
		}
	}

	fadd << vec_of_details[i].getFactory() << endl << vec_of_details[i].getPrice() << endl
		<< vec_of_details[i].getDay() << " " << vec_of_details[i].getMonth() << " " << vec_of_details[i].getYear();

	fadd.close();
}

void writeFile(vector<Detail> vec_of_details)
{
	ofstream fout(FILE_OF_DETAILS, ios::out);

	for (int i = 0; i < vec_of_details.size(); i++)
	{
		if (i == vec_of_details.size() - 1)
		{
			fout << vec_of_details[i].getFactory() << endl << vec_of_details[i].getPrice() << endl
				<< vec_of_details[i].getDay() << " " << vec_of_details[i].getMonth() << " " << vec_of_details[i].getYear();
		}
		else
		{
			fout << vec_of_details[i].getFactory() << endl << vec_of_details[i].getPrice() << endl
				<< vec_of_details[i].getDay() << " " << vec_of_details[i].getMonth() << " " << vec_of_details[i].getYear() << endl;
		}
	}

	fout.close();
}

void readFile(vector<Detail>& vec_of_details)
{
	ifstream fin(FILE_OF_DETAILS, ios::in);

	if (!fin.is_open())
		return;

	if (fin.peek() == fin.eof())
		return;

	string factory;
	float price;
	int day, month, year;
	while (!fin.eof())
	{
		getline(fin, factory);
		fin >> price;
		fin >> day;
		fin >> month;
		fin >> year;

		Detail detail(factory, price, day, month, year);
		vec_of_details.push_back(detail);
		fin.ignore(10, '\n');
	}

	fin.close();
}

DWORD WINAPI ThreadFunc(LPVOID client_socket)
{
	SOCKET s2 = ((SOCKET*)client_socket)[0];
	char buf[255];
	vector<Detail> vec_of_details;
	readFile(vec_of_details);

	while (recv(s2, buf, sizeof(buf), NULL))
	{
		switch (buf[0])
		{
		case '0': {
			cout << "Выбран выход." << endl;
			closesocket(s2);
			return 0;
			break;
		}

		case '1': {
			cout << "Выбран просмотр данных." << endl;

			stringstream ss; //переделать отправку не через поток(?)
			int i = 0;
			for (auto it : vec_of_details)
			{
				i++;
				ss << i << ". Завод-поставщик: " << it.getFactory()
					<< "\t\t\tЦена: " << it.getPrice()
					<< "\t\t\tДата поставки: " << it.getDay() << " " << it.getMonth() << " " << it.getYear() << endl;
			}

			string data = ss.str();

			send(s2, data.c_str(), data.length(), NULL);
			break;
		}
		case '2': {
			cout << "Выбрано добавление данных." << endl;

			int bytesReceived = recv(s2, buf, sizeof(buf), 0);
			if (bytesReceived != -1) {
				buf[bytesReceived] = '\0';
			}
			else {
				cerr << "Ошибка!" << endl;
			}
			string factory = buf;

			bytesReceived = recv(s2, buf, sizeof(buf), 0);
			if (bytesReceived != -1) {
				buf[bytesReceived] = '\0';
			}
			else {
				cerr << "Ошибка!" << endl;
			}
			string price = buf;
			float pr = stof(price);

			bytesReceived = recv(s2, buf, sizeof(buf), 0);
			if (bytesReceived != -1) {
				buf[bytesReceived] = '\0';
			}
			else {
				cerr << "Ошибка!" << endl;
			}
			stringstream ss;
			string date = buf;
			ss << date;
			int day, month, year;
			ss >> day;
			ss.seekg(3);
			ss >> month;
			ss.seekg(6);
			ss >> year;

			Detail detail(factory, pr, day, month, year);
			vec_of_details.push_back(detail);
			addFile(vec_of_details);

			send(s2, "1\0", 2, NULL);
			break;
		}
		case '3': {
			cout << "Выбрано редактирование данных." << endl;

			stringstream ss;
			int i = 0;
			for (auto it : vec_of_details)
			{
				i++;
				ss << i << ". Завод-поставщик: " << it.getFactory()
					<< "\t\t\tЦена: " << it.getPrice()
					<< "\t\t\tДата поставки: " << it.getDay() << " " << it.getMonth() << " " << it.getYear() << endl;
			}

			string data = ss.str();

			send(s2, data.c_str(), data.length(), NULL);

			int bytesReceived = recv(s2, buf, sizeof(buf), 0);
			if (bytesReceived != -1) {
				buf[bytesReceived] = '\0';
			}
			else {
				cerr << "Ошибка!" << endl;
			}
			string ch = buf;
			int index = stoi(ch);
			index--;

			if (index < 0 || index >= vec_of_details.size())
			{
				send(s2, "Некорректный номер записи!", sizeof("Некорректный номер записи!"), NULL);
			}
			else
			{
				int bytesReceived = recv(s2, buf, sizeof(buf), 0);
				if (bytesReceived != -1) {
					buf[bytesReceived] = '\0';
				}
				else {
					cerr << "Ошибка!" << endl;
				}
				string factory = buf;

				bytesReceived = recv(s2, buf, sizeof(buf), 0);
				if (bytesReceived != -1) {
					buf[bytesReceived] = '\0';
				}
				else {
					cerr << "Ошибка!" << endl;
				}
				string price = buf;
				float pr = stof(price);

				bytesReceived = recv(s2, buf, sizeof(buf), 0);
				if (bytesReceived != -1) {
					buf[bytesReceived] = '\0';
				}
				else {
					cerr << "Ошибка!" << endl;
				}
				stringstream ss;
				string date = buf;
				ss << date;
				int day, month, year;
				ss >> day;
				ss.seekg(3);
				ss >> month;
				ss.seekg(6);
				ss >> year;

				Detail detail(factory, pr, day, month, year);
				vec_of_details[index] = detail;
				writeFile(vec_of_details);

				send(s2, "Данные успешно изменены.", sizeof("Данные успешно изменены."), NULL);
			}
			break;
		}
		case '4': {
			cout << "Выбрано удаление данных." << endl;

			stringstream ss;
			int i = 0;
			for (auto it : vec_of_details)
			{
				i++;
				ss << i << ". Завод-поставщик: " << it.getFactory()
					<< "\t\t\tЦена: " << it.getPrice()
					<< "\t\t\tДата поставки: " << it.getDay() << " " << it.getMonth() << " " << it.getYear() << endl;
			}

			string data = ss.str();

			send(s2, data.c_str(), data.length(), NULL);

			int bytesReceived = recv(s2, buf, sizeof(buf), 0);
			if (bytesReceived != -1) {
				buf[bytesReceived] = '\0';
			}
			else {
				cerr << "Ошибка!" << endl;
			}
			string ch = buf;
			int index = stoi(ch);
			index--;

			if (index < 0 || index >= vec_of_details.size())
			{
				send(s2, "Некорректный номер записи!", sizeof("Некорректный номер записи!"), NULL);
			}
			else
			{
				vec_of_details.erase(vec_of_details.begin() + index);
				writeFile(vec_of_details);

				send(s2, "Данные успешно удалены.", sizeof("Данные успешно удалены."), NULL);
			}
			break;
		}
		case '5': {
			cout << "Выбран поиск по дате." << endl;

			int bytesReceived = recv(s2, buf, sizeof(buf), 0);
			if (bytesReceived != -1) {
				buf[bytesReceived] = '\0';
			}
			else {
				cerr << "Ошибка!" << endl;
			}
			stringstream ss;
			string date = buf;
			ss << date;
			int day, month, year;
			ss >> day;
			ss.seekg(3);
			ss >> month;
			ss.seekg(6);
			ss >> year;

			float min = vec_of_details[0].getPrice();
			int index = 0;
			for (int i = 1; i < vec_of_details.size(); i++)
			{
				if (min > vec_of_details[i].getPrice())
				{
					min = vec_of_details[i].getPrice();
					index = i;
				}
			}

			stringstream ss2;

			for (int i = 0; i < vec_of_details.size(); i++)
			{
				if (day == vec_of_details[i].getDay() && month == vec_of_details[i].getMonth() && year == vec_of_details[i].getYear() && index != i)
				{
					ss2 << i << ". Завод-поставщик: " << vec_of_details[i].getFactory()
						<< "\t\t\tЦена: " << vec_of_details[i].getPrice()
						<< "\t\t\tДата поставки: " << vec_of_details[i].getDay() << " " << vec_of_details[i].getMonth() << " " << vec_of_details[i].getYear() << endl;
				}
			}

			if (ss2.str() == "")
			{
				send(s2, "Нет деталей поставленных в данную дату.", sizeof("Нет деталей поставленных в данную дату."), NULL);
			}
			else
			{
				string data = ss2.str();

				send(s2, data.c_str(), data.length(), NULL);
			}
			closesocket(s2);
			return 0;

			break;
		}
		default: { cout << "Неверный выбор!" << endl; system("pause"); system("cls"); }
		}

	}
}

int numcl = 0;
void print() {
	if (numcl) cout << numcl << "клинт(-ов) подключено" << endl;
	else cout << "Нет подключенных клиентов" << endl;
}

int main() {
	setlocale(LC_ALL, "rus");
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		cout << "Ошибка!" << endl;
		return 0;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, NULL);
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(1280);
	local_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(s, (sockaddr*)&local_addr, sizeof(local_addr));
	int c = listen(s, 5);

	cout << "Сервер готов к работе!" << endl;
	cout << endl;

	// извлекаем сообщение из очереди
	SOCKET client_socket;    // сокет для клиента
	sockaddr_in client_addr; // адрес клиента (заполняется системой)
	int client_addr_size = sizeof(client_addr);

	int i = 0;
	// цикл извлечения запросов на подключение из очереди
	while ((client_socket = accept(s, (sockaddr*)&client_addr, &client_addr_size)))
	{
		numcl++;
		print();
		// Вызов нового потока для обслуживания клиента
		DWORD thID;// thID идентификатор типа DWORD
		CreateThread(NULL, NULL, ThreadFunc, &client_socket, NULL, &thID);
	}
	return 0;
}