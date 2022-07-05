#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <vector>
#include <time.h>
#include <ctime>

using namespace std;

class Account
{
public:
	long accountNumber;
	string names;
	string dob;
	string createdAt;
	float balance;

	long generateAccountNumber()
	{
		return rand() % 10000000000 + 10000000000;
	}

	string generatedDate()
	{
		auto t = time(nullptr);
		auto tm = *localtime(&t);
		stringstream currentTime;
		currentTime << put_time(&tm, "%d-%m-%Y");
		return currentTime.str();
	}

	Account()
	{
		accountNumber = generateAccountNumber();
		createdAt = generatedDate();
	}
};

void createAccount()
{
	int numberOfAccounts;
	ofstream file;
	// file.open("Accounts.txt", ios::app);
	file.open("accounts.csv", ios::in);

	if(!file){
		file.close();
		file.open("Accounts.csv",ios::app);
		file << "id,names,dob,createdAt,balance"<<endl;
	}else{
		file.close();
		file.open("Accounts.csv",ios::app);
	}

	cout << "Enter number of accounts to create" << endl;
	cin >> numberOfAccounts;


	for (int i = 0; i < numberOfAccounts; i++)
	{
		Account *account = new Account();
		cout << "Enter account holder name :";
		cin.ignore();
		getline(cin, account->names);
		cout << "Enter birth date (date-month-yeah): ";
		cin >> account->dob;
		cout << "Enter initial Balance :";
		cin >> account->balance;

		cout << "Account number of " << account->names << "  is: " << account->accountNumber << endl;
		// file << account->accountNumber << "\t" << account->names << "\t" << account->dob << "\t" << account->createdAt << "\t" << account->balance << endl;
		file << to_string(account->accountNumber)+ "," + account->names + "," + account->dob + "," + account->createdAt + "," + to_string(account->balance) << endl;
	}
	file.close();
}

void getAllAccount()
{
	ifstream file;
	file.open("Accounts.txt");
	cout << "Acount number - Holder name - Date of birth - \t Create at -\t Balance" << endl;
	string line;
	while (getline(file, line))
	{
		cout << line << endl;
	}
	file.close();
}

string searchAccount(long accountNumber)
{
	ifstream file;
	file.open("Accounts.txt");
	string line;
	string result = "";

	while (getline(file, line))
	{
		if (line.find(to_string(accountNumber)) != string::npos)
		{
			result = line;
		}
	}
	file.close();
	if (result == "")
		cout << "Account doesn't exist" << endl;
	return result;
}

void lineToArray(string const &str, const char delim, vector<string> &out)
{
	stringstream ss(str);
	string arrayValue;
	while (getline(ss, arrayValue, delim))
	{
		out.push_back(arrayValue);
	}
}

void updateAccount(string action)
{
	ifstream file;
	ofstream newfile;
	file.open("Accounts.txt");
	newfile.open("NewAccounts.txt");

	string line;
	long accountNumber;
	float amount;
	cout << "Enter account number :";
	cin >> accountNumber;
	cout << "Enter amount";
	cin >> amount;

	string fountAccount = searchAccount(accountNumber);
	if (fountAccount != "")
	{
		while (getline(file, line))
		{
			if (fountAccount == line)
			{
				vector<string> lineValues;
				lineToArray(fountAccount, '\t', lineValues);

				float balance = stof(lineValues[4]);
				if (action == "deposit")
				{
					balance += amount;
				}
				else
				{
					if (balance <= 0 || balance < amount)
					{
						cout << "Insufficient balance" << endl;
						break;
					}
					else if (amount == 0)
					{
						cout << "Can't withdaw zero amount" << endl;
						break;
					}
					balance -= amount;
				}
				cout << "New balance :" << balance << endl;
				newfile << lineValues[0] << "\t" << lineValues[1] << "\t" << lineValues[2] << "\t" << lineValues[3] << "\t" << balance << endl;
			}
			else
			{
				newfile << line << endl;
			}
		}
		file.close();
		newfile.close();
		remove("Accounts.txt");
		rename("NewAccounts.txt", "Accounts.txt");
	}
}

void updateAccountDetails()
{
	ifstream file;
	ofstream newfile;

	file.open("Accounts.txt");
	newfile.open("NewAccounts.txt");

	string line, newName, newDOB;
	long accountNumber;
	cout << "Enter account number :";
	cin >> accountNumber;

	string fountAccount = searchAccount(accountNumber);
	if (fountAccount != "")
	{

		cout << "Enter new name: ";
		cin.ignore();
		getline(cin, newName);

		cout << "Enter new DOB: ";
		cin >> newDOB;
		while (getline(file, line))
		{
			if (fountAccount == line)
			{
				vector<string> lineValues;
				lineToArray(line, '\t', lineValues);
				newfile << lineValues[0] << "\t" << newName << "\t" << newDOB << "\t" << lineValues[3] << "\t" << lineValues[4] << endl;
			}
			else
			{
				newfile << line << endl;
			}
		}
		file.close();
		newfile.close();
		remove("Accounts.txt");
		rename("NewAccounts.txt", "Accounts.txt");
	}
}

void closeAccount()
{
	long numberAccount;
	cout << "Enter account number: ";
	cin >> numberAccount;

	string foundAccount = searchAccount(numberAccount);
	if (foundAccount != "")
	{
		ifstream file;
		ofstream newfile;

		file.open("Accounts.txt");
		newfile.open("NewAccounts.txt");

		string line;
		while (getline(file, line))
		{
			if (foundAccount != line)
			{
				newfile << line << endl;
			}
		}
		file.close();
		newfile.close();
		remove("Account.txt");
		rename("NewAccounts.txt", "Accounts.txt");
	}
}
int getNumberOfData()
{
	ifstream file;
	file.open("Accounts.txt");
	int count = 0;
	string line;
	while (getline(file, line))
	{
		count++;
	}
	return count;
}

void sortByAmount()
{
	vector<Account> accounts;
	ifstream file;
	file.open("Accounts.txt");
	int numberOfData = getNumberOfData();
	string line;
	int looper = 0;

	while (getline(file, line))
	{

		vector<string> lineValues;
		lineToArray(line, '\t', lineValues);

		Account account;

		account.accountNumber = stol(lineValues[0]);
		account.names = lineValues[1];
		account.dob = lineValues[2];
		account.createdAt = lineValues[3];
		account.balance = stof(lineValues[4]);

		accounts.push_back(account);
	}

	for (int j = 0; j < numberOfData; ++j)
	{
		for (int i = j + 1; i < numberOfData; ++i)
		{
			if (accounts[i].balance > accounts[j].balance)
			{
				Account temp = accounts[j];
				accounts[j] = accounts[i];
				accounts[i] = temp;
			}
		}
	}

	for (int h = 0; h < numberOfData; h++)
	{
		cout << accounts[h].balance << '\t' << accounts[h].names << endl;
	}
}

void transfer()
{
	long from, to;
	float amount;

	cout << "Enter the account to transfer money: ";
	cin >> from;
	cout << "Enter the account to receive money: ";
	cin >> to;
	cout << "Enter the amount to transfer: ";
	cin >> amount;

	string foundAccoundfrom = searchAccount(from);
	string foundAccountto = searchAccount(to);

	if (foundAccountto != "")
	{
		ifstream file;
		file.open("Accounts.txt");
		ofstream newfile;
		newfile.open("NewAccounts.txt");
		string line;
		while (getline(file, line))
		{
			if (foundAccountto == line)
			{
				vector<string> lineValues;
				lineToArray(foundAccountto, '\t', lineValues);

				float balance = stof(lineValues[4]);
				balance += amount;

				cout << "New balance :" << balance << endl;
				newfile << lineValues[0] << "\t" << lineValues[1] << "\t" << lineValues[2] << "\t" << lineValues[3] << "\t" << balance << endl;
			}
			else
			{
				newfile << line << endl;
			}
		}
		file.close();
		newfile.close();
		remove("Account.txt");
		rename("NewAccounts.txt", "Accounts.txt");
	}
	
	if (foundAccoundfrom != "")
	{
		ifstream file;
		file.open("Accounts.txt");
		ofstream newfile;
		newfile.open("NewAccounts.txt");
		string line;
		while (getline(file, line))
		{
			if (foundAccoundfrom == line)
			{
				vector<string> lineValues;
				lineToArray(foundAccoundfrom, '\t', lineValues);

				float balance = stof(lineValues[4]);
				if (balance <= 0 || balance < amount)
				{
					cout << "Insufficient balance" << endl;
					break;
				}
				else if (amount == 0)
				{
					cout << "Can't transfer zero amount" << endl;
					break;
				}
				balance -= amount;

				cout << "New balance :" << balance << endl;
				newfile << lineValues[0] << "\t" << lineValues[1] << "\t" << lineValues[2] << "\t" << lineValues[3] << "\t" << balance << endl;
			}
			else
			{
				newfile << line << endl;
			}
		}
		file.close();
		newfile.close();
		remove("Account.txt");
		rename("NewAccounts.txt", "Accounts.txt");
	}
}

void menu()
{
	int ch;
	long num;

	do
	{
		cout << "\n\n\t\t\t\t======================\n";
		cout << "\t\t\t\tBANK MANAGEMENT SYSTEM";
		cout << "\n\t\t\t\t======================\n";

		cout << "\t\t\t\t    ::MAIN MENU::\n";
		cout << "\n\t\t\t\t1. NEW ACCOUNT";
		cout << "\n\t\t\t\t2. DEPOSIT AMOUNT";
		cout << "\n\t\t\t\t3. WITHDRAW AMOUNT";
		cout << "\n\t\t\t\t4. BALANCE ENQUIRY";
		cout << "\n\t\t\t\t5. ALL ACCOUNT HOLDER LIST";
		cout << "\n\t\t\t\t6. CLOSE AN ACCOUNT";
		cout << "\n\t\t\t\t7. MODIFY AN ACCOUNT";
		cout << "\n\t\t\t\t8. SORT BY AMOUNT";
		cout << "\n\t\t\t\t9. TRANSFER AMOUNT";
		cout << "\n\t\t\t\t10. EXIT";
		cout << "\n\n\t\t\t\tSelect Your Option (1-10): ";
		cin >> ch;

		switch (ch)
		{
		case 1:
			createAccount();
			break;
		case 2:
			system("clear");
			updateAccount("deposit");
			break;
		case 3:
			system("clear");
			updateAccount("withdraw");
			break;
		case 4:
			system("clear");
			cout << "\n\n\t\t\tEnter The account No. : ";
			cin >> num;
			cout << searchAccount(num) << endl;
			break;
		case 5:
			system("clear");
			cout << "\n\n\t\t\t\t\tACCOUNT HOLDER LIST\n";
			getAllAccount();
			break;
		case 6:
			system("clear");
			closeAccount();
			break;
		case 7:
			system("clear");
			updateAccountDetails();
			break;
		case 8:
			system("clear");
			sortByAmount();
			break;
		case 9:
			system("clear");
			transfer();
			break;
		case 10:
			system("clear");
			cout << "\n\n\t\t\tGoodbye";
			break;
		default:
			cout << "\a";
		}
		cin.ignore();
		cin.get();
	} while (ch != 10);
}

int main()
{
	menu();
}
