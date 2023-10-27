#include <iostream>
#include <bits/stdc++.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdlib.h>
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
MYSQL_RES *res;

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

class madicine_tb
{
public:
	string storeid[5000];
	string storename[5000];
	string storecompany[5000];
	string storearrival_date[5000];
	string storeexpire_date[5000];
	string storeprice[5000];
	string storequantity[5000];
	int storeIndex;
	int sl;
	void getData();
	void printData();
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
	char exitSurity = 'n';

	while (!exit)
	{
		system("cls");
		welcome();
		cout << "1. Buy Medicine" << endl;
		cout << "2. Show Item List" << endl;
		cout << "3. Add Item in Stock" << endl;
		cout << "4. Update Stock Item" << endl;
		cout << "5. Delete Stock Item" << endl;
		cout << "0. EXIT" << endl
			 << endl;
		cout << "Choose One: ";
		scanf("%d", &choose);

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
	system("cls");

	char choose;
	string getId = "";
	string itemId[5000];
	string itemName[5000];
	int quantity[5000];
	char chooseEditOrBuy;
	int itemIndex = 0;
	float totalPrice = 0.0;
	bool purchase, itemFalse = false;
	int storeIndexN = 0;
	int qty;

	welcome();
	mtb->printData();
	cout<<endl;
	for (itemIndex = 0;; itemIndex++)
	{
		cout <<endl<< "Enter a Column ID's (q to exit): ";
		cin >> getId;
		
		if (getId == "q")
		{
			break;
		}
		else
		{
			cout << "Enter a Quantity: ";
			cin>>qty;
			for (int i = 0; i < mtb->storeIndex; i++)
			{
				if (getId == mtb->storeid[i])
				{
					itemId[itemIndex] = getId;
					itemName[itemIndex]=mtb->storename[i];
					quantity[itemIndex]=qty;
					itemFalse = false;
					break;
				}
				else
				{
					itemFalse = true;
				}
			}
			if (itemFalse == true)
			{
				cout << "Enter a valid number." << endl;
				itemIndex--;
				itemFalse = false;
			}
		}
	}

	cout << "You choose this item id's: ";
	for (int i = 0; i < itemIndex; i++)
	{
		cout <<endl<<"\t"<< itemName[i] << " X "<<quantity[i];
	}

CHOOSEEDITORBUY:
	cout << endl
		 << "Do you want to edit items(e) or buy this items(b): ";
	cin >> chooseEditOrBuy;

	if (chooseEditOrBuy == 'e')
	{
		for (int i = 0;; i++)
		{
			cout << "Remove item id's (q to exit): ";
			cin >> getId;
			if (getId == "q")
			{
				break;
			}
			else
			{
				for (int j = 0; j < itemIndex; j++)
				{
					if (itemId[j] == getId)
					{
						for (int k = j; k < itemIndex; k++)
						{
							itemId[k] = itemId[k + 1];
							itemName[k]=itemName[k+1];
							quantity[k]=quantity[k+1];
						}
						itemId[itemIndex--] = "\0";
						itemName[itemIndex] = "\0";
						quantity[itemIndex] = 0;
					}
				}
			}
		}

		cout << "You choose this Item id's: ";
		for (int i = 0; i < itemIndex; i++)
		{
			cout <<endl<<"\t"<< itemName[i] << " X "<<quantity[i];
		}

		goto CHOOSEEDITORBUY;
	}
	else if (chooseEditOrBuy == 'b')
	{
		for (int i = 0; i < itemIndex; i++)
		{
			for (int j = 0; j < mtb->storeIndex; j++)
			{
				if (itemId[i] == mtb->storeid[j])
				{
					qstate = mysql_query(conn, "select quantity from madicine_tb");
					if (!qstate)
					{
						res = mysql_store_result(conn);
						while ((row = mysql_fetch_row(res)))
						{
							if (atoi(row[0]) > 0)
							{
								mtb->storequantity[storeIndexN] = row[0];
								storeIndexN++;
							}
						}
						storeIndexN = 0;
					}
					else
					{
						cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
					}

					totalPrice += (strtof((mtb->storeprice[j]).c_str(), 0))*quantity[i];

					float quan1 = strtof((mtb->storequantity[j]).c_str(), 0);
					quan1 -= 1*quantity[i];
					float quan2 = strtof((mtb->storequantity[j]).c_str(), 0);
					float quan = quan2 - quan1;
					string getResult;
					stringstream strquan;
					strquan << quan;
					strquan >> getResult;

					string getQuan1;
					stringstream strquan1;
					strquan1 << quan1;
					strquan1 >> getQuan1;

					purchase = true;

					string update_query = "update madicine_tb set quantity = '" + getQuan1 + "' where id = '" + mtb->storeid[j] + "'";
					const char *qu = update_query.c_str();

					qstate = mysql_query(conn, qu);

					if (qstate)
					{
						cout << "Failed To Update!" << mysql_errno(conn) << endl;
					}
				}
			}
		}

		if (purchase == true)
		{
			cout << endl
				 << "Purchase Successfully Done." << endl;
			cout << endl
				 << "Total Price: " << totalPrice << endl;
		}
	}
	mtb->getData();

	Sleep(1000);
	cout << endl
		 << "Press Any Key To Continue...";
	getch();
	system("cls");
}

void ShowItemList()
{
	system("cls");
	welcome();
	mtb->printData();

	Sleep(1000);
	cout << endl
		 << "Press Any Key To Continue...";
	getch();
	system("cls");
}

void AddIteminStock()
{
	cin.ignore(100, '\n');
	string name, company, arv_dt, exp_dt, prc, qt;
	cout << "Enter Name: ";
	getline(cin, name);
	cout << "Enter Company: ";
	getline(cin, company);
	cout << "Enter Arival Date (YYYY-MM-DD): ";
	getline(cin, arv_dt);
	cout << "Enter Expire Date (YYYY-MM-DD): ";
	getline(cin, exp_dt);
	cout << "Enter Price: ";
	cin >> prc;
	cout << "Enter Quantity: ";
	cin >> qt;

	string query = "INSERT INTO madicine_tb (Name , Company, Arrival_date, expire_date, price, quantity) VALUES ('" + name + "','" + company + "','" + arv_dt + "','" + exp_dt + "','" + prc + "','" + qt + "' )";
	if (mysql_query(conn, query.c_str()))
	{
		cout << mysql_error(conn) << endl;
		Sleep(3000);
	}
	else
	{
		cout << "Data inserted..." << endl
			 << endl;
	}
	Sleep(1500);
}

void UpdateStockItem()
{
	system("cls");

	string name = "";
	string company = "";
	string arrival_date = "";
	string expire_date = "";
	string price = "";
	string quantity = "";

	int itemID;
	int itemIndex;
	bool HaveException = false;
	bool NotInDatabase = false;

	welcome();

	mtb->printData();

ExitMenu:
	try
	{

		cout << endl;
		cout << "Enter Item ID: ";
		cin >> itemID;
		cout << endl;
	}
	catch (exception e)
	{
		cout << "Please Enter a valid NUMBER." << endl;
		HaveException = true;
		goto ExitMenu;
	}

	if (!HaveException)
	{
		stringstream streamid;
		string strid;
		streamid << itemID;
		streamid >> strid;

		for (int i = 0; i < mtb->sl; i++)
		{
			if (strid != mtb->storeid[i])
			{
				NotInDatabase = true;
			}
			else
			{
				itemIndex = i;
				NotInDatabase = false;
				break;
			}
		}
	}

	if (!NotInDatabase)
	{
		cout << endl;
		cout << "Name: " << mtb->storename[itemIndex] << endl;
		cout << "ID: " << mtb->storeid[itemIndex] << endl;
		cout << "Arrival Date: " << mtb->storearrival_date[itemIndex] << endl;
		cout << "Expire Date: " << mtb->storeexpire_date[itemIndex] << endl;
		cout << "Price: " << mtb->storeprice[itemIndex] << endl;
		cout << "Quantity: " << mtb->storequantity[itemIndex] << endl
			 << endl;

		cin.ignore(1, '\n');
		string defaultString = "NA";
		cout << "Enter Name (" << defaultString << "  to not change): ";
		getline(cin, name);
		if (name == defaultString)
		{
			name = mtb->storename[itemIndex];
		}
		cout << "Enter Company (" << defaultString << "  to not change): ";
		getline(cin, company);
		if (company == defaultString)
		{
			company = mtb->storecompany[itemIndex];
		}
		cout << "Enter Arrival Date (" << defaultString << "  to not change) (yyyy-mm-dd): ";
		getline(cin, arrival_date);
		if (arrival_date == defaultString)
		{
			arrival_date = mtb->storearrival_date[itemIndex];
		}
		cout << "Enter Expire Date (" << defaultString << "  to not change) (yyyy-mm-dd): ";
		getline(cin, expire_date);
		if (expire_date == defaultString)
		{
			expire_date = mtb->storeexpire_date[itemIndex];
		}
		cout << "Enter Price (" << defaultString << "  to not change): ";
		getline(cin, price);
		if (price == defaultString)
		{
			price = mtb->storeprice[itemIndex];
		}
		cout << "Enter Quantity (" << defaultString << "  to not change): ";
		cin >> quantity;
		if (quantity == defaultString)
		{
			quantity = mtb->storequantity[itemIndex];
		}

		string update_query = "update madicine_tb set name = '" + name + "', company = '" + company + "', arrival_date = '" + arrival_date + "', expire_date = '" + expire_date + "', price = '" + price + "', quantity = '" + quantity + "' where id = '" + mtb->storeid[itemIndex] + "'";
		const char *qu = update_query.c_str();
		qstate = mysql_query(conn, qu);

		if (!qstate)
		{
			mtb->getData();
			cout << endl
				 << "Successfully Updated." << endl;
		}
		else
		{
			cout << "Failed To Update!" << mysql_errno(conn) << endl;
		}
	}
	else
	{
		cout << "Item Not Found in database." << endl;
	}

	Sleep(1000);
	cout << endl
		 << "Press Any Key To Continue...";
	getch();
	system("cls");
}

void DeleteStockItem()
{
	system("cls");
	welcome();
	mtb->printData();
	string id;
	cout << endl
		 << "Enter ID to delete: ";
	cin >> id;

	string query = "DELETE FROM madicine_tb WHERE ID = '" + id + "'";
	if (mysql_query(conn, query.c_str()))
	{
		cout << mysql_error(conn) << endl;
		Sleep(3000);
	}
	else
	{
		cout << "Item deleted..." << endl
			 << endl;
	}
	Sleep(2000);
}

void madicine_tb::getData()
{
	sl = 1;
	storeIndex = sl - 1;
	qstate = mysql_query(conn, "select * from madicine_tb");
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while ((row = mysql_fetch_row(res)))
		{
			if (atoi(row[6]) > 0)
			{
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
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
}

void madicine_tb::printData()
{
	getData();
	qstate = mysql_query(conn, "select * from madicine_tb");
	if (!qstate)
	{
		res = mysql_store_result(conn);
		printf("+-----+-------+----------------------+----------------------+---------------+-------------+-------+------------+\n");
		printf("|%-5s| %-5s | %-20s | %-20s | %-13s | %-11s | %-5s | %-10s |\n", "SlNo", "ID", "NAME", "COMPANY", "Arrival date", "expire date", "Price", "Quantity");
		printf("+-----+-------+----------------------+----------------------+---------------+-------------+-------+------------+\n");
		while ((row = mysql_fetch_row(res)))
		{
			if (atoi(row[6]) > 0)
			{
				printf("| %-3d | %-5s | %-20s | %-20s | %-13s | %-11s | %-5s | %-10s |\n", sl, row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
			}
			sl++;
		}
		printf("+-----+-------+----------------------+----------------------+---------------+-------------+-------+------------+\n");
	}
	else
	{
		cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
	}
}
