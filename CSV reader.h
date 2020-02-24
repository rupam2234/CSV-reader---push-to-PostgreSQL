#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

// required header for establish conncetion with postgreSQL

#include<windows.h>
#include<sql.h>
#include<sqlext.h> 
#include <wchar.h>

using namespace std;

class CSVreader
{
protected:

	string name;  // for storing the name (string) from the csv file
	string rollno;  // for storing the rollno (stirng) from the csv file
	string coursename;  // for storing the coursename (string) from the csv file
	string age;  // for storing the age (string) from the csv file

public:

	ifstream csvfile; // to read from the file

	string line; // location to store the line once we fetch it

	vector<string> fileElements;


	void openFile() // method to open a file from lacal directory
	{
		csvfile.open("test.csv"); //to open the file

		// check if the file is available or not

		if (!csvfile.is_open())  // if unable to open
		{
			cout << "///Error! Unable to open the file///" << endl;
		}

		else
			cout << "///file succesfully opened///  " << endl;

	}

	vector<string> getData() // method to store datas into the vector
	{
		cout << "\nReading data from the file....\n";
		while (getline(csvfile, line))
		{
			stringstream storage(line);  // "stroage" is the instance of the "stringstream" class

			getline(storage, name, ',');  // comma will be delimited 
			fileElements.push_back(name);  // pushing the data into the vector

			getline(storage, rollno, ',');
			fileElements.push_back(rollno);

			getline(storage, coursename, ',');
			fileElements.push_back(coursename);

			getline(storage, age, ',');
			fileElements.push_back(age);
        
		}

		// check elements inside the vector

		for (int i = 0; i < fileElements.size(); i++)
		{
			cout << fileElements[i] << " " << endl;
		}

		cout << "\nData fatched correctly...." << endl;
		
		return fileElements;

	}

};

class PushToDatabase : public CSVreader
{

public:

	SQLRETURN obtainResult; // to get the information when a connection with database is established
	SQLHENV environmentHandle; // it will call "SQLAllocHandle" and pass the address of "environmentHandle"
	SQLHDBC connectionHandler; //to allocate a connection handle before connecting to the data source/ database

	void connect()
	{

		wchar_t dsn[] = L"PostgreSQL"; // ODBS Database DSN   
		wchar_t id[] = L"postgres";     // database name (postgreSQL)
		wchar_t pass[] = L"Rupom@223456"; // Login pass of the database

		SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &environmentHandle);
		SQLSetEnvAttr(environmentHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC2, 0);
		SQLAllocHandle(SQL_HANDLE_DBC, environmentHandle, &connectionHandler);

		obtainResult = SQLConnect(connectionHandler, dsn, SQL_NTS, id, SQL_NTS, pass, SQL_NTS);

		if (obtainResult == SQL_SUCCESS || obtainResult == SQL_SUCCESS_WITH_INFO)
		{
			cout << "\nconnection established...\n" << endl; // when conncetion established succesfully
		}

		else

			cout << "Unable to connect to the database" << endl; // unable to connect to database
	}

	void pushtoDatabase(vector<string> fileElements) // taking vector as input string
	{
		for (int i = 0; i < fileElements.size();)
		{
                        // concatenating the SQL query inside a loop 
			
			// it will automatically run the Query for all the values

			char query[300] = "INSERT INTO datareader(name, rollno, coursename, age) VALUES('"; 
			strcat_s(query, fileElements[i].c_str());
			strcat_s(query, "', '");
			i++;
			strcat_s(query, fileElements[i].c_str());
			strcat_s(query, "', '");
			i++;
			strcat_s(query, fileElements[i].c_str());
			strcat_s(query, "', '");
			i++;
			strcat_s(query, fileElements[i].c_str());
			strcat_s(query, "');");
			i++;

			wchar_t* SQL_Quer; 

			int nChars = MultiByteToWideChar(CP_ACP, 0, query, -1, NULL, 0);  // char to Wchar conversion 

			SQL_Quer = new wchar_t[nChars];
			MultiByteToWideChar(CP_ACP, 0, query, -1, (LPWSTR)SQL_Quer, nChars);

			SQLHSTMT stmt;
			SQLAllocHandle(SQL_HANDLE_STMT, connectionHandler, &stmt);

			obtainResult = SQLExecDirect(stmt, SQL_Quer, SQL_NTS);

			if (obtainResult == SQL_SUCCESS || obtainResult == SQL_SUCCESS_WITH_INFO) // obtain the success massage for both type of return
				cout << "New Values Inserted Into The Database\n";

			else
				cout << "Unable To Insert Value (Check for repeated values)\n";
		}
	}
};
