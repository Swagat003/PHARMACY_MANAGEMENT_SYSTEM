#include <iostream>
#include <bits/stdc++.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <mysql.h>
#include <mysqld_error.h>

using namespace std;

const char *HOST = "localhost";
const char *USER = "root";
const char *PASSW = "password";
const char *DB = "pharmacy";

// mysql connection variables
int qstate;
MYSQL *conn;
MYSQL_ROW row;
MYSQL_RES* res;

class db_response
{
public:
	static void connectionFunction()
	{
		conn = mysql_init(NULL);
		conn = mysql_real_connect(conn, HOST, USER, PASSW, DB, 3306, NULL, 0);
		if (conn)
		{
			cout << "Database Connected..." << endl
				 << endl;
			Sleep(2000);
			system("cls");
		}
		else
		{
			cout << "Failed to connect!" << endl;
			cout << mysql_error(conn) << endl;
		}
	}
	static void connectionClose()
	{
		mysql_close(conn);
	}
};

class madicine_tb{
	public:
	string storeid[5000];
    string storename[5000];
    string storecompany[5000];
    string storearrival_date[5000];
    string storeexpire_date[5000];
    string storeprice[5000];
    string storequantity[5000];
    int storeIndex = 0;
	// int storeIndexN = 0;
	void getData();
};
madicine_tb *mtb = new madicine_tb();

// Other functions
void welcome();

// Menu functions
void BuyMedicine();
void ShowItemList();
void AddIteminStock();
void UpdateStockItem();
void DeleteStockItem();

int main()
{
	system("title Pharamacy Management System");
	system("color e0");

	db_response::connectionFunction();

	bool exit = false;
	int choose = 0;
	char exitSurity='n';

	while (!exit)
	{
		system("cls");
		welcome();
		cout << "1. Buy Medicine" << endl;
		cout << "2. Show Item List" << endl;
		cout << "3. Add Item in Stock" << endl;
		cout << "4. Update Stock Item" << endl;
		cout << "5. Delete Stock Item" << endl;
		cout << "0. EXIT" << endl << endl;
		cout << "Choose One: ";
//		cin >> choose;
		scanf("%d",&choose);

		switch (choose)
		{
		case 1:
			BuyMedicine();
			break;
		case 2:
			ShowItemList();
			break;
		case 3:
			AddIteminStock();
			break;
		case 4:
			UpdateStockItem();
			break;
		case 5:
			DeleteStockItem();
			break;
		case 0:
		ExitProgram:
			cout << "Program terminating. Are you sure? (y/n): ";
			cin >> exitSurity;
			if (exitSurity == 'y' || exitSurity == 'Y')
			{
				exit = true;
				break;
			}
			else if (exitSurity == 'n' || exitSurity == 'N')
			{
				system("cls");
			}
			else
			{
				cout << "Invalid Input!" << endl;
				goto ExitProgram;
			}
			break;
		default:
			cout << "Please choose between 1 - 7. Press Any Key To Continue...";
			getch();
			system("cls");
			break;
		}
	}

	db_response::connectionClose();
	return 0;
}

void welcome()
{
	cout << "Welcome To Pharmacy Management System" << endl
		 << endl;
	cout << "Menu" << endl;
}

void BuyMedicine()
{

}

void ShowItemList()
{
	mtb->getData();
	Sleep(2000);
	cout << "Press Any Key To Continue...";
	getch();
	system("cls");

}

void AddIteminStock()
{

}

void UpdateStockItem()
{

}

void DeleteStockItem()
{

}

void madicine_tb::getData(){
	qstate = mysql_query(conn,"select * from madicine_tb");
	if(!qstate){			
		res = mysql_store_result(conn);
		printf("+-------+----------------------+----------------------+---------------+-------------+-------+------------+\n");
		printf("| %-5s | %-20s | %-20s | %-13s | %-11s | %-5s | %-10s |\n","ID","NAME","COMPANY","Arrival date","expire date","Price","Quantity");
		printf("+-------+----------------------+----------------------+---------------+-------------+-------+------------+\n");
		while((row = mysql_fetch_row(res))){
			if( atoi(row[6])>0){
				printf("| %-5s | %-20s | %-20s | %-13s | %-11s | %-5s | %-10s |\n",row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
				storeid[storeIndex] = row[0];
                storename[storeIndex] = row[1];
                storecompany[storeIndex] = row[2];
                storearrival_date[storeIndex] = row[3];
                storeexpire_date[storeIndex] = row[4];
                storeprice[storeIndex] = row[5];
                storequantity[storeIndex] = row[6];
                storeIndex++;
			}
		}
		printf("+-------+----------------------+----------------------+---------------+-------------+-------+------------+\n");
	}
}
