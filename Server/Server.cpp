
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include "conio.h"
#include "fstream"
#include "vector"

#pragma warning(disable: 4996)

using namespace std;

SOCKET Connections[100];
int Counter_Socket = 0;

class User {
public:
	void setLogin(string name)
	{
		this->login = name;
	}
	string getLogin()
	{
		return login;
	}
	void setPassword(string fam)
	{
		this->password = fam;
	}
	string getPassword()
	{
		return password;
	}
	void setStatus(int age)
	{
		this->status = age;
	}
	int getStatus()
	{
		return status;
	}
	void write(ostream& fout)
	{
		size_t len_log = login.length() + 1;
		size_t len_pass = password.length() + 1;
		fout.write((char*)&len_log, sizeof(len_log));
		fout.write((char*)login.c_str(), len_log);
		fout.write((char*)&len_pass, sizeof(len_pass));
		fout.write((char*)password.c_str(), len_pass);
		fout.write((char*)&status, sizeof(status));
	}
	void read(istream& fin)
	{
		size_t len_log, len_pass;
		fin.read((char*)&len_log, sizeof(len_log));
		char* buf_log = new char[len_log];
		fin.read(buf_log, len_log);
		login = buf_log;
		delete[]buf_log;
		fin.read((char*)&len_pass, sizeof(len_pass));
		char* buf_pass = new char[len_pass];
		fin.read(buf_pass, len_pass);
		password = buf_pass;
		delete[]buf_pass;
		fin.read((char*)&status, sizeof(status));
	}

private:
	string login;
	string password;
	int status;
};

class Insurance
{
public:

	Insurance()
	{
		policyName = string();
		price = int();
		type = string();
		time = int();
	}
	Insurance(string policyName, int price, string type, int time) :policyName(policyName), price(price), type(type), time(time) {}
	string getName()
	{
		return policyName;
	}
	int getPrice()
	{
		return price;
	}
	string getPolicy()
	{
		return type;
	}
	int getTime()
	{
		return time;
	}
	void setName(string policyName)
	{
		this->policyName = policyName;
	}
	void setPrice(int field)
	{
		this->price = field;
	}
	void setType(string field)
	{
		this->type = field;
	}
	void setTime(int field)
	{
		this->time = field;
	}
	void write(ostream& fout)
	{
		size_t len_TP = policyName.length() + 1;
		fout.write((char*)&len_TP, sizeof(len_TP));
		fout.write((char*)policyName.c_str(), len_TP);
		fout.write((char*)&price, sizeof(price));
		size_t len_min = type.length() + 1;
		fout.write((char*)&len_min, sizeof(len_min));
		fout.write((char*)type.c_str(), len_min);
		fout.write((char*)&time, sizeof(time));
	}
	void read(istream& fin)
	{
		size_t len_TP;
		fin.read((char*)&len_TP, sizeof(len_TP));
		char* buf_log = new char[len_TP];
		fin.read(buf_log, len_TP);
		policyName = buf_log;
		delete[]buf_log;
		fin.read((char*)&price, sizeof(price));
		size_t len_min;
		fin.read((char*)&len_min, sizeof(len_min));
		char* buf_min = new char[len_min];
		fin.read(buf_min, len_min);
		type = buf_min;
		delete[]buf_min;
		fin.read((char*)&time, sizeof(time));

	}
private:
	string policyName;
	int price;
	string type;
	int time;
};

class Task {
public:
	void setTask(string name)
	{
		this->name = name;
	}
	string getTask()
	{
		return name;
	}
	void setDescription(string fam)
	{
		this->description = fam;
	}
	string getDescription()
	{
		return description;
	}


private:
	string name;
	string description;
};

class Solution {
public:
	void setTask(string name)
	{
		this->name = name;
	}
	string getTask()
	{
		return name;
	}
	void setDescription(string fam)
	{
		this->description = fam;
	}
	string getDescription()
	{
		return description;
	}
	void setPoints(int points)
	{
		this->points = points;
	}
	int getScore()
	{
		return points;
	}

private:
	string name;
	string description;
	int points;
};

int getAmountOfUsers();
int getAmountOfPolicy();
vector<User> getUser();
vector<Insurance> getInsurance();
int userAccessLocal(int index);
int userAccessGlobal(int index);
void userHandler(int index);



int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "ru");
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error" << endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for (int ii = 0; ii < 100; ii++) {
		cout << "Запуск" << endl;

		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0) {
			cout << "Ошибка \n";
		}
		else {
			cout << "Пользователь подключен\n";
			int msg_size;

			Connections[ii] = newConnection;
			Counter_Socket++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)userHandler, (LPVOID)(ii), NULL, NULL);

		}
	}



	return 0;
}



int getAmountOfUsers()
{
	int count;
	ifstream fin_count("AmountOfUsers.txt");
	fin_count.read((char*)&count, sizeof(count));
	fin_count.close();
	return count;
}
int getAmountOfPolicy()
{
	int count;
	ifstream fin_count("PolicyAmount.txt");
	fin_count.read((char*)&count, sizeof(count));
	fin_count.close();
	return count;
}
vector<User> getUser()
{
	int count = getAmountOfUsers();
	ifstream fin("User.txt", ios::binary);
	shared_ptr<User[]> ab(new User[count]);
	vector<User> Data_vect = {};
	for (int i = 0; i < count; i++)
	{
		ab[i].read(fin);
		Data_vect.push_back(ab[i]);
	}
	fin.close();
	return Data_vect;
}
vector<Insurance> getInsurance()
{
	int count = getAmountOfPolicy();
	ifstream fin("Catalog.txt", ios::binary);
	shared_ptr<Insurance[]> ab(new Insurance[count]);
	vector<Insurance> Data_vect = {};
	for (int i = 0; i < count; i++)
	{
		ab[i].read(fin);
		Data_vect.push_back(ab[i]);
	}
	fin.close();
	return Data_vect;
}
int userAccessLocal(int index)
{
	int count = getAmountOfUsers();
	vector<User> Data_vect = getUser();
	shared_ptr<int> check(new int(-1));

	int msg_size;
	recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
	cout << "Размер логина:" << msg_size << endl;
	char* login_read = new char[msg_size + 1];
	login_read[msg_size] = '\0';
	recv(Connections[index], login_read, msg_size, NULL);
	string login = login_read;
	cout << "Логин: " << login << endl;

	recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
	char* password_read = new char[msg_size + 1];
	password_read[msg_size] = '\0';
	recv(Connections[index], password_read, msg_size, NULL);
	string password = password_read;
	cout << "Пароль: " << password << endl;

	int counter = 0;
	for (auto ok : Data_vect)
	{
		if (ok.getLogin() == login && ok.getPassword() == password)
		{
			*check = ok.getStatus(); break;
		}
		counter++;
	}

	if (*check == -1)
	{
		cout << "\nДанные введены неверно\n" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}
	return *check;
}
int userAccessGlobal(int index)
{
	int* b = new int(0);
	int i;
	for (i = 3; i > 0; i--)
	{
		*b = userAccessLocal(index);
		if (*b == -1)
		{

			if (i == 3)
			{
				int answer = 4;
				send(Connections[index], (char*)&answer, sizeof(int), NULL);

			}
			if (i == 2)
			{
				int answer = 5;
				send(Connections[index], (char*)&answer, sizeof(int), NULL);

			}

		}
		else break;
	}
	if (i == 0)
	{
		delete  b;
		return -1;
	}
	int x = *b;
	delete  b;
	return x;
}
void userHandler(int index) {
	int menu;
	int gl_check = 1;
	while (gl_check == 1) {
		recv(Connections[index], (char*)&menu, sizeof(int), NULL);
		switch (menu) {
		case 1:
		{
			int* access = new int(0);
			*access = userAccessGlobal(index);
			if (*access == -1)
			{
				int answer = -1;
				send(Connections[index], (char*)&answer, sizeof(int), NULL);
				cout << "Возврат в главное меню" << endl;;
				break;
			}
			int answer = *access;
			send(Connections[index], (char*)&answer, sizeof(int), NULL);
			cout << "Вход выполнен" << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			delete access;
			int exit = 0;
			int add_menu;
			switch (answer)
			{
			case 1:
			{
				cout << "Меню пользователя" << endl;
				do {
					recv(Connections[index], (char*)&menu, sizeof(int), NULL);
					switch (menu) {
					case 1:
					{
						int count = getAmountOfPolicy();
						vector<Insurance> Data_vect = getInsurance();
						send(Connections[index], (char*)&count, sizeof(int), NULL);

						for (auto ok : Data_vect)
						{
							string name = ok.getName();
							string type = ok.getPolicy();
							int time = ok.getTime();
							int size = name.size();
							send(Connections[index], (char*)&size, sizeof(int), NULL);
							send(Connections[index], name.c_str(), size, NULL);
							size = type.size();
							send(Connections[index], (char*)&size, sizeof(int), NULL);
							send(Connections[index], type.c_str(), size, NULL);
							int price = ok.getPrice();
							send(Connections[index], (char*)&time, sizeof(int), NULL);
							send(Connections[index], (char*)&price, sizeof(int), NULL);
						}
						break;
					}
					case 2:
					{
						int size;
						recv(Connections[index], (char*)&size, sizeof(int), NULL);
						char* login_read = new char[size + 1];
						login_read[size] = '\0';
						recv(Connections[index], login_read, size, NULL);
						string login_access = login_read;
						ofstream fout_fake(login_access + ".txt", ios::app);
						fout_fake.close();
						ifstream fin(login_access + ".txt");
						int count = 0;
						long file_size;
						fin.seekg(0, ios::end);
						file_size = fin.tellg();
						if (file_size == 0)
						{
							send(Connections[index], (char*)&count, sizeof(int), NULL);
						}
						else {
							fin.seekg(0);
							while (!fin.eof()) {
								fin >> login_access;
								count++;
							}
							cout << count << endl;

							send(Connections[index], (char*)&count, sizeof(int), NULL);

							fin.seekg(0);

							while (!fin.eof()) {
								fin >> login_access;
								int size = login_access.size();
								send(Connections[index], (char*)&size, sizeof(int), NULL);
								send(Connections[index], login_access.c_str(), size, NULL);

							}
						}
						fin.close();

						break;
					}
					case 3:
					{
						int size;
						recv(Connections[index], (char*)&size, sizeof(int), NULL);
						char* login_read = new char[size + 1];
						login_read[size] = '\0';
						recv(Connections[index], login_read, size, NULL);
						string login_access = login_read;
						delete[] login_read;

						recv(Connections[index], (char*)&size, sizeof(int), NULL);
						char* login_re = new char[size + 1];
						login_re[size] = '\0';
						recv(Connections[index], login_re, size, NULL);
						string name = login_re;
						delete[] login_re;

						vector<Insurance> Data_vect_Insurance = getInsurance();

						ifstream fin_like(login_access + ".txt");
						long file_size;
						fin_like.seekg(0, ios::end);
						file_size = fin_like.tellg();
						if (file_size == 0)
						{
							int check = 0;
							for (auto ok : Data_vect_Insurance)
							{
								if (ok.getName() == name) check = 1;
							}
							if (check == 0)
							{
								send(Connections[index], (char*)&check, sizeof(int), NULL);

							}
							else {
								check = 1;
								ofstream fout_like(login_access + ".txt");
								fout_like << name;
								fout_like.close();
								send(Connections[index], (char*)&check, sizeof(int), NULL);
							}
						}
						else {
							int check = 0;
							for (auto ok : Data_vect_Insurance)
							{
								if (ok.getName() == name) check = 1;
							}
							if (check == 0)
							{
								send(Connections[index], (char*)&check, sizeof(int), NULL);

							}
							else {
								check = 0;
								ifstream fin(login_access + ".txt");
								fin.seekg(0);
								string Insurance;
								while (!fin.eof()) {
									fin >> Insurance;

									if (Insurance == name) check = 1;
								}
								if (check == 0)
								{
									check = 1;
									ofstream fout_like_yeah(login_access + ".txt", ios::app);
									fout_like_yeah << endl;
									fout_like_yeah << name;
									fout_like_yeah.close();
									send(Connections[index], (char*)&check, sizeof(int), NULL);
								}
								else {
									check = 2;
									send(Connections[index], (char*)&check, sizeof(int), NULL);
								}



							}

						}
						fin_like.close();

						break;
					}
					case 4:
					{
						int size;
						recv(Connections[index], (char*)&size, sizeof(int), NULL);
						char* login_read = new char[size + 1];
						login_read[size] = '\0';
						recv(Connections[index], login_read, size, NULL);
						string login_access = login_read;
						delete[] login_read;

						recv(Connections[index], (char*)&size, sizeof(int), NULL);
						char* login_re = new char[size + 1];
						login_re[size] = '\0';
						recv(Connections[index], login_re, size, NULL);
						string name = login_re;
						delete[] login_re;


						ifstream fin_like(login_access + ".txt");
						long file_size;
						fin_like.seekg(0, ios::end);
						file_size = fin_like.tellg();
						if (file_size == 0)
						{
							send(Connections[index], (char*)&file_size, sizeof(int), NULL);
						}
						else {
							int check = 0;
							vector<string> Insurances;
							ifstream fin(login_access + ".txt");
							fin.seekg(0);
							string Insurance;
							while (!fin.eof()) {
								fin >> Insurance;

								if (Insurance == name) check = 1;
								else {
									Insurances.push_back(Insurance);
								}
							}
							if (check == 1)
							{
								int helping = 0;
								ofstream fout_like_yeah(login_access + ".txt");
								for (string Insurance : Insurances)
								{
									if (helping = 0) {
										fout_like_yeah << Insurance;
										helping = 1;
									}
									else {
										fout_like_yeah << endl;
										fout_like_yeah << Insurance;
									}
								}

								fout_like_yeah.close();
								send(Connections[index], (char*)&check, sizeof(int), NULL);
							}
							else {
								check = 0;
								send(Connections[index], (char*)&check, sizeof(int), NULL);
							}
							fin.close();

						}
						fin_like.close();

						break;
					}
					case 5:
					{
						int size;
						recv(Connections[index], (char*)&size, sizeof(int), NULL);
						char* login_read = new char[size + 1];
						login_read[size] = '\0';
						recv(Connections[index], login_read, size, NULL);
						string login_access = login_read;
						delete[] login_read;

						recv(Connections[index], (char*)&size, sizeof(int), NULL);
						char* login_re = new char[size + 1];
						login_re[size] = '\0';
						recv(Connections[index], login_re, size, NULL);
						string name = login_re;
						delete[] login_re;

						cout << name << endl;
						ifstream fin_like("issue.txt");
						long file_size;
						fin_like.seekg(0, ios::end);
						file_size = fin_like.tellg();
						fin_like.close();
						if (file_size == 0)
						{
							int check = 0;
							cout << "Case 1" << endl;
							check = 1;
							ofstream fout_like("issue.txt");
							fout_like << login_access << endl;
							fout_like << name;
							fout_like.close();
							send(Connections[index], (char*)&check, sizeof(int), NULL);

						}
						else {
							int check = 0;
							cout << "Case 2" << endl;
							ofstream fout_like("issue.txt", ios::app);
							fout_like << endl;
							fout_like << login_access << endl;
							fout_like << name;
							fout_like.close();
							send(Connections[index], (char*)&check, sizeof(int), NULL);
						}

						break;
					}
					case 6:
					{
						exit = 1;
						break;
						break;
					}
					}
				} while (exit == 0);
				break;
			}
			case 2:
			{
				cout << "Меню с решениями" << endl;
				do {
					recv(Connections[index], (char*)&menu, sizeof(int), NULL);
					switch (menu) {
					case 1:
					{
						ifstream fin_task("Unsolved_T.txt");
						string task;
						string decide;
						string count_str;
						int count_task;
						int status = 1;
						while (!fin_task.eof())
						{
							send(Connections[index], (char*)&status, sizeof(int), NULL);
							getline(fin_task, task);

							int size = task.size();
							send(Connections[index], (char*)&size, sizeof(int), NULL);
							send(Connections[index], task.c_str(), size, NULL);
							getline(fin_task, count_str);


							count_task = atoi(count_str.c_str());
							send(Connections[index], (char*)&count_task, sizeof(int), NULL);
							for (int i = 0; i < count_task; i++)
							{
								getline(fin_task, decide);
								size = decide.size();
								send(Connections[index], (char*)&size, sizeof(int), NULL);
								send(Connections[index], decide.c_str(), size, NULL);

							}


						}
						status = 0;
						send(Connections[index], (char*)&status, sizeof(int), NULL);
						fin_task.close();
						break;
					}
					case 2:
					{
						ifstream fin_task("Solved_T.txt");
						string task;
						string decide;
						string count_str;
						string marking;
						int count_task;
						int mark;
						int status = 1;
						while (!fin_task.eof())
						{
							send(Connections[index], (char*)&status, sizeof(int), NULL);
							getline(fin_task, task);

							int size = task.size();
							send(Connections[index], (char*)&size, sizeof(int), NULL);
							send(Connections[index], task.c_str(), size, NULL);
							getline(fin_task, count_str);


							count_task = atoi(count_str.c_str());
							send(Connections[index], (char*)&count_task, sizeof(int), NULL);
							for (int i = 0; i < count_task; i++)
							{
								getline(fin_task, decide);
								size = decide.size();
								send(Connections[index], (char*)&size, sizeof(int), NULL);
								send(Connections[index], decide.c_str(), size, NULL);

								getline(fin_task, marking);
								mark = atoi(marking.c_str());
								send(Connections[index], (char*)&mark, sizeof(int), NULL);
							}


						}
						status = 0;
						send(Connections[index], (char*)&status, sizeof(int), NULL);
						fin_task.close();
						break;
					}
					case 3:
					{
						ifstream fin_task_list("Unsolved_T.txt");
						string task_list;
						string decide_list;
						string count_str_list;
						int count_task_list;
						int status_list = 1;
						while (!fin_task_list.eof())
						{
							send(Connections[index], (char*)&status_list, sizeof(int), NULL);
							getline(fin_task_list, task_list);

							int size = task_list.size();
							send(Connections[index], (char*)&size, sizeof(int), NULL);
							send(Connections[index], task_list.c_str(), size, NULL);
							getline(fin_task_list, count_str_list);


							count_task_list = atoi(count_str_list.c_str());

							for (int i = 0; i < count_task_list; i++)
							{
								getline(fin_task_list, decide_list);
								size = decide_list.size();


							}


						}
						status_list = 0;
						send(Connections[index], (char*)&status_list, sizeof(int), NULL);
						fin_task_list.close();

						int size;
						recv(Connections[index], (char*)&size, sizeof(int), NULL);
						char* login_read = new char[size + 1];
						login_read[size] = '\0';
						recv(Connections[index], login_read, size, NULL);
						string name = login_read;

						ifstream fin_task("Unsolved_T.txt");
						int status = 1;
						string task;
						string decide;
						string count_str;
						int count_task;
						while (!fin_task.eof() && status == 1)
						{
							getline(fin_task, task);
							if (task == name)
							{
								cout << "Уровень 2" << endl;
								status = 5;
								send(Connections[index], (char*)&status, sizeof(int), NULL);

								ofstream fout_res("Solved_T.txt", ios::app);
								fout_res << endl;
								fout_res << task << endl;
								size = task.size();
								send(Connections[index], (char*)&size, sizeof(int), NULL);
								send(Connections[index], task.c_str(), size, NULL);
								getline(fin_task, count_str);


								count_task = atoi(count_str.c_str());
								fout_res << count_task;
								send(Connections[index], (char*)&count_task, sizeof(int), NULL);
								int mark;
								for (int i = 0; i < count_task; i++)
								{
									getline(fin_task, decide);
									fout_res << endl;
									fout_res << decide << endl;
									size = decide.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], decide.c_str(), size, NULL);
									recv(Connections[index], (char*)&mark, sizeof(int), NULL);
									fout_res << mark;
								}
								fout_res.close();
								break;
							}
							else {
								cout << "Уровень 1" << endl;
								status = 1;
								send(Connections[index], (char*)&status, sizeof(int), NULL);

								getline(fin_task, count_str);
								count_task = atoi(count_str.c_str());
								for (int i = 0; i < count_task; i++)
								{
									getline(fin_task, decide);
								}
							}
						}
						if (status == 1) status = 2;

						cout << "Выключение" << endl;

						fin_task.close();
						break;
					}
					case 4:
					{
						exit = 1;
						break;
					}
					}
				} while (exit == 0);

				break;
			}
			case 3:
			{
				cout << "Меню администратора" << endl;
				do {
					recv(Connections[index], (char*)&menu, sizeof(int), NULL);
					switch (menu) {
					case 1: {
						cout << "Админ - пользователи" << endl;
						int add_exit = 0;
						do {
							recv(Connections[index], (char*)&add_menu, sizeof(int), NULL);
							switch (add_menu)
							{
							case 1:
							{
								cout << "Админ - просмотр" << endl;
								int count = getAmountOfUsers();
								vector<User> Data_vect = getUser();
								send(Connections[index], (char*)&count, sizeof(int), NULL);

								for (auto ok : Data_vect)
								{
									string login = ok.getLogin();
									string pass = ok.getPassword();
									int size = login.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], ok.getLogin().c_str(), size, NULL);
									size = pass.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], ok.getPassword().c_str(), size, NULL);
									int status = ok.getStatus();
									send(Connections[index], (char*)&status, sizeof(int), NULL);
								}

								break;
							}
							case 2:
							{
								cout << "Админ, новый пользователь" << endl;
								string login;
								string pass;
								int size;
								int status;
								vector<User> Data_vect = getUser();

								recv(Connections[index], (char*)&size, sizeof(int), NULL);
								char* login_read = new char[size + 1];
								login_read[size] = '\0';
								recv(Connections[index], login_read, size, NULL);
								login = login_read;

								recv(Connections[index], (char*)&size, sizeof(int), NULL);
								char* pass_read = new char[size + 1];
								pass_read[size] = '\0';
								recv(Connections[index], pass_read, size, NULL);
								pass = pass_read;

								recv(Connections[index], (char*)&status, sizeof(int), NULL);
								int check = 0;
								for (auto ok : Data_vect)
								{
									if (ok.getLogin() == login) check = 1;
								}
								if (check == 0)
								{
									ofstream fin("User.txt", ios::binary | ios::app);
									shared_ptr<User> ab(new User);
									ab->setLogin(login);
									ab->setPassword(pass);
									ab->setStatus(status);
									ab->write(fin);

									fin.close();
									ifstream fin_coun("AmountOfUsers.txt");
									int count;
									fin_coun.read((char*)&count, sizeof(count));
									count += 1;
									fin_coun.close();
									ofstream fin_count("AmountOfUsers.txt");
									fin_count.write((char*)&count, sizeof(count));
									fin_count.close();


									send(Connections[index], (char*)&check, sizeof(int), NULL);

								}
								else {
									check = 1;
									send(Connections[index], (char*)&check, sizeof(int), NULL);
								}
								delete[] login_read, pass_read;
								break;
							}
							case 3:
							{
								vector<User> Data_vect = getUser();
								int size;
								recv(Connections[index], (char*)&size, sizeof(int), NULL);
								char* login_read = new char[size + 1];
								login_read[size] = '\0';
								recv(Connections[index], login_read, size, NULL);
								string login = login_read;
								int check = 0;
								vector<User>::iterator iter = Data_vect.begin();
								for (auto ok : Data_vect)
								{
									if (ok.getLogin() == login)
									{
										check = 1;
										Data_vect.erase(iter);
										ofstream fout_user("User.txt");
										for (auto ok : Data_vect)
										{
											ok.write(fout_user);
										}
										fout_user.close();
										ifstream fin_coun("AmountOfUsers.txt");
										int count;
										fin_coun.read((char*)&count, sizeof(count));
										count -= 1;
										fin_coun.close();
										ofstream fin_count("AmountOfUsers.txt");
										fin_count.write((char*)&count, sizeof(count));
										fin_count.close();
										count = 1;
										send(Connections[index], (char*)&count, sizeof(int), NULL);
										break;
									}
									++iter;
								}
								if (check == 0) send(Connections[index], (char*)&check, sizeof(int), NULL);

								break;
							}
							case 4:
							{
								add_exit = 1;
								break;
							}
							}
						} while (add_exit == 0);
						break;
					}
					case 2: {
						cout << "Админ - полисы" << endl;
						int add_exit = 0;
						do {
							recv(Connections[index], (char*)&add_menu, sizeof(int), NULL);
							switch (add_menu)
							{
							case 1:
							{
								int count = getAmountOfPolicy();
								vector<Insurance> Data_vect = getInsurance();
								send(Connections[index], (char*)&count, sizeof(int), NULL);

								for (auto ok : Data_vect)
								{
									string name = ok.getName();
									string type = ok.getPolicy();
									int time = ok.getTime();
									int size = name.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], name.c_str(), size, NULL);
									size = type.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], type.c_str(), size, NULL);
									int price = ok.getPrice();
									send(Connections[index], (char*)&time, sizeof(int), NULL);
									send(Connections[index], (char*)&price, sizeof(int), NULL);
								}

								break;
							}
							case 2:
							{
								cout << "Админ - добавить" << endl;
								string name;
								string type;
								int size;
								int time;
								int price;
								vector<Insurance> Data_vect = getInsurance();

								recv(Connections[index], (char*)&size, sizeof(int), NULL);
								char* login_read = new char[size + 1];
								login_read[size] = '\0';
								recv(Connections[index], login_read, size, NULL);
								name = login_read;

								recv(Connections[index], (char*)&size, sizeof(int), NULL);
								char* pass_read = new char[size + 1];
								pass_read[size] = '\0';
								recv(Connections[index], pass_read, size, NULL);
								type = pass_read;

								recv(Connections[index], (char*)&time, sizeof(int), NULL);
								recv(Connections[index], (char*)&price, sizeof(int), NULL);
								int check = 0;
								for (auto ok : Data_vect)
								{
									if (ok.getName() == name) check = 1;
								}
								if (check == 0)
								{
									ofstream fin("Catalog.txt", ios::binary | ios::app);
									shared_ptr<Insurance> ab(new Insurance);
									ab->setName(name);
									ab->setType(type);
									ab->setTime(time);
									ab->setPrice(price);
									ab->write(fin);

									fin.close();
									ifstream fin_coun("PolicyAmount.txt");
									int count;
									fin_coun.read((char*)&count, sizeof(count));
									count += 1;
									fin_coun.close();
									ofstream fin_count("PolicyAmount.txt");
									fin_count.write((char*)&count, sizeof(count));
									fin_count.close();


									send(Connections[index], (char*)&check, sizeof(int), NULL);

								}
								else {
									check = 1;
									send(Connections[index], (char*)&check, sizeof(int), NULL);
								}
								delete[] login_read, pass_read;
								break;
							}
							case 3:
							{
								vector<Insurance> Data_vect = getInsurance();
								int size;
								recv(Connections[index], (char*)&size, sizeof(int), NULL);
								char* login_read = new char[size + 1];
								login_read[size] = '\0';
								recv(Connections[index], login_read, size, NULL);
								string name = login_read;
								int check = 0;
								vector<Insurance>::iterator iter = Data_vect.begin();
								for (auto ok : Data_vect)
								{
									if (ok.getName() == name)
									{
										check = 1;
										Data_vect.erase(iter);
										ofstream fout_user("Catalog.txt");
										for (auto ok : Data_vect)
										{
											ok.write(fout_user);
										}
										fout_user.close();
										ifstream fin_coun("PolicyAmount.txt");
										int count;
										fin_coun.read((char*)&count, sizeof(count));
										count -= 1;
										fin_coun.close();
										ofstream fin_count("PolicyAmount.txt");
										fin_count.write((char*)&count, sizeof(count));
										fin_count.close();
										count = 1;
										send(Connections[index], (char*)&count, sizeof(int), NULL);
										break;
									}
									++iter;
								}
								if (check == 0) send(Connections[index], (char*)&check, sizeof(int), NULL);

								break;
							}
							case 4:
							{
								vector<Insurance> Data_vect = getInsurance();
								int count = getAmountOfPolicy();
								send(Connections[index], (char*)&count, sizeof(int), NULL);
								int chet = count;
								vector<Insurance> Sort_Data;
								for (int i = 0; i < count; i++)
								{
									int* counter = new int(0);
									vector<Insurance>::iterator it;
									it = Data_vect.begin();
									auto x = Data_vect[0];
									for (auto ok : Data_vect)
									{
										if (x.getPrice() > ok.getPrice())
										{
											x = ok;
											it = it + *counter;
										}
										*counter = *counter + 1;
									}
									Sort_Data.push_back(x);
									Data_vect.erase(it);
								}
								for (auto ok : Sort_Data)
								{
									string name = ok.getName();
									string type = ok.getPolicy();
									int time = ok.getTime();
									int size = name.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], name.c_str(), size, NULL);
									size = type.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], type.c_str(), size, NULL);
									int price = ok.getPrice();
									send(Connections[index], (char*)&time, sizeof(int), NULL);
									send(Connections[index], (char*)&price, sizeof(int), NULL);
								}

								break;
							}
							case 5:
							{
								vector<Insurance> Data_vect = getInsurance();
								int count = getAmountOfPolicy();

								int chet = 0;
								vector<Insurance> Sort_Data;

								for (auto ok : Data_vect)
								{
									if (ok.getTime() >= 365.0)
									{
										Sort_Data.push_back(ok);
										chet++;
									}

								}
								send(Connections[index], (char*)&chet, sizeof(int), NULL);
								for (auto ok : Sort_Data)
								{
									string name = ok.getName();
									string type = ok.getPolicy();
									int time = ok.getTime();
									int size = name.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], name.c_str(), size, NULL);
									size = type.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], type.c_str(), size, NULL);
									int price = ok.getPrice();
									send(Connections[index], (char*)&time, sizeof(int), NULL);
									send(Connections[index], (char*)&price, sizeof(int), NULL);
								}

								break;
							}
							case 6:
							{
								add_exit = 1;
								break;
							}
							}
						} while (add_exit == 0);
						break;
					}
					case 3: {
						cout << "Админ, задачи" << endl;
						int add_exit = 0;
						do {
							recv(Connections[index], (char*)&add_menu, sizeof(int), NULL);
							switch (add_menu)
							{
							case 1:
							{

								ifstream fin("issue.txt");
								string temp;
								int count = 0;

								fin.seekg(0);
								while (!fin.eof()) {
									getline(fin, temp);
									getline(fin, temp);
									count++;
								}
								cout << count << endl;

								send(Connections[index], (char*)&count, sizeof(int), NULL);

								fin.seekg(0);

								while (!fin.eof()) {
									getline(fin, temp);
									int size = temp.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], temp.c_str(), size, NULL);
									getline(fin, temp);
									size = temp.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], temp.c_str(), size, NULL);

								}
								cout << "finish" << endl;
								fin.close();
								break;
							}
							case 2:
							{
								ifstream fin_task("Unsolved_T.txt");
								string task;
								string decide;
								string count_str;
								int count_task;
								int status = 1;
								while (!fin_task.eof())
								{
									send(Connections[index], (char*)&status, sizeof(int), NULL);
									getline(fin_task, task);

									int size = task.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], task.c_str(), size, NULL);
									getline(fin_task, count_str);


									count_task = atoi(count_str.c_str());
									send(Connections[index], (char*)&count_task, sizeof(int), NULL);
									for (int i = 0; i < count_task; i++)
									{
										getline(fin_task, decide);
										size = decide.size();
										send(Connections[index], (char*)&size, sizeof(int), NULL);
										send(Connections[index], decide.c_str(), size, NULL);

									}


								}
								status = 0;
								send(Connections[index], (char*)&status, sizeof(int), NULL);
								fin_task.close();
								break;
							}
							case 3:
							{
								ifstream fin_task("Solved_T.txt");
								string task;
								string decide;
								string count_str;
								string marking;
								int count_task;
								int mark;
								int status = 1;
								while (!fin_task.eof())
								{
									send(Connections[index], (char*)&status, sizeof(int), NULL);
									getline(fin_task, task);

									int size = task.size();
									send(Connections[index], (char*)&size, sizeof(int), NULL);
									send(Connections[index], task.c_str(), size, NULL);
									getline(fin_task, count_str);


									count_task = atoi(count_str.c_str());
									send(Connections[index], (char*)&count_task, sizeof(int), NULL);
									for (int i = 0; i < count_task; i++)
									{
										getline(fin_task, decide);
										size = decide.size();
										send(Connections[index], (char*)&size, sizeof(int), NULL);
										send(Connections[index], decide.c_str(), size, NULL);

										getline(fin_task, marking);
										mark = atoi(marking.c_str());
										send(Connections[index], (char*)&mark, sizeof(int), NULL);
									}


								}
								status = 0;
								send(Connections[index], (char*)&status, sizeof(int), NULL);
								fin_task.close();
								break;
							}
							case 4:
							{
								int size;
								int count_task;
								recv(Connections[index], (char*)&size, sizeof(int), NULL);
								char* login_read = new char[size + 1];
								login_read[size] = '\0';
								recv(Connections[index], login_read, size, NULL);
								string task = login_read;
								delete[] login_read;

								recv(Connections[index], (char*)&count_task, sizeof(int), NULL);
								ofstream fout_task("Unsolved_T.txt", ios::app);
								fout_task << endl;
								fout_task << task << endl;
								fout_task << count_task;
								for (int i = 0; i < count_task; i++)
								{
									recv(Connections[index], (char*)&size, sizeof(int), NULL);
									char* login_re = new char[size + 1];
									login_re[size] = '\0';
									recv(Connections[index], login_re, size, NULL);
									string decide = login_re;
									fout_task << endl;
									fout_task << decide;
									delete[] login_re;
								}
								fout_task.close();


								break;
							}
							case 5:
							{
								add_exit = 1;
								break;
							}

							}
						} while (add_exit == 0);
						break;
					}
					case 4: {
						exit = 1;
						break;
					}
					}
				} while (exit == 0);
				break;
			}
			}
			break;
		}
		case 2:
		{
			cout << "Админ, новый пользователь" << endl;
			string login;
			string pass;
			int size;
			int status;
			vector<User> Data_vect = getUser();

			recv(Connections[index], (char*)&size, sizeof(int), NULL);
			char* login_read = new char[size + 1];
			login_read[size] = '\0';
			recv(Connections[index], login_read, size, NULL);
			login = login_read;

			recv(Connections[index], (char*)&size, sizeof(int), NULL);
			char* pass_read = new char[size + 1];
			pass_read[size] = '\0';
			recv(Connections[index], pass_read, size, NULL);
			pass = pass_read;

			recv(Connections[index], (char*)&status, sizeof(int), NULL);
			int check = 0;
			for (auto ok : Data_vect)
			{
				if (ok.getLogin() == login) check = 1;
			}
			if (check == 0)
			{
				ofstream fin("User.txt", ios::binary | ios::app);
				shared_ptr<User> ab(new User);
				ab->setLogin(login);
				ab->setPassword(pass);
				ab->setStatus(status);
				ab->write(fin);

				fin.close();
				ifstream fin_coun("AmountOfUsers.txt");
				int count;
				fin_coun.read((char*)&count, sizeof(count));
				count += 1;
				fin_coun.close();
				ofstream fin_count("AmountOfUsers.txt");
				fin_count.write((char*)&count, sizeof(count));
				fin_count.close();


				send(Connections[index], (char*)&check, sizeof(int), NULL);

			}
			else {
				check = 1;
				send(Connections[index], (char*)&check, sizeof(int), NULL);
			}
			delete[] login_read, pass_read;
			break;
		}
		case 3:
		{
			cout << "Отключение" << endl;
			gl_check = 0;
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			break;
		}
		}
	}
}
