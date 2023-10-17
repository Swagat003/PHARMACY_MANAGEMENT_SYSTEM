#include <iostream>
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

int qstate;
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES res;

const char* HOST = "localhost";
const char* USER = "root";
const char* PASSW = "password";
const char* DB = "pharmacy";

class db_response{
	public:
		static void connectionFunction(){
			conn = mysql_init(NULL);
			conn=mysql_real_connect(conn,HOST,USER,PASSW,DB,3306,NULL,0);
			if (conn){
				cout<<"Database Connected..."<<conn<<endl;
				Sleep(3000);
				system("cls");
			}else{
				cout<<"Failed to connect!"<<endl;
				cout<<mysql_error(conn)<<endl;
			}
		}
};

int main() {
	
	return 0;
}
