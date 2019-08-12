#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <string.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <Windows.h>

using namespace std;

class CSVReader
{
	string fileName;
	string delimeter;

public:
	CSVReader(string filename, string delm = ",") :
		fileName(filename), delimeter(delm)
	{ }

	// Function to fetch data from a CSV File
	vector<vector<string>> getData();
};

/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
vector<vector<string>> CSVReader::getData()
{
	ifstream file(fileName);

	vector<vector<string>> dataList;

	string line = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
		vector<string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
		dataList.push_back(vec);
	}
	// Close the File
	file.close();

	return dataList;
}

int search (string path) {

	wstring stemp = wstring(path.begin(), path.end());
	LPCWSTR pathtemp = stemp.c_str();
	WIN32_FIND_DATA file;
	HANDLE search_handle = FindFirstFile(pathtemp, &file);
	int i = 0;

	if (search_handle) {
		do {
			//wcout << file.cfilename << endl;
			wstring ws(file.cFileName);
			string str(ws.begin(), ws.end());
			//cout << str << endl;
			i++;
		} while (FindNextFile(search_handle, &file));
		FindClose(search_handle);
	}
	return i;
}


int main()
{
	string path1 = "C:\\Users\\Flo\\Desktop\\C2 Projekt\\Integration_Final\\datensatz*.CSV";
	string path2 = "C:\\Users\\Flo\\Desktop\\C2 Projekt\\Integration_Final\\area*.CSV";
	double voltage[2500], time[2500];
	double area = 0;
	int i = 0, n = 0, m = 0;
	char filename[] = "datensatzX.CSV";
	char areafilename[] = "areaX.CSV";
	string convfilename, convareafilename;

	cout << "Search for files "<< path1 << endl;
	cout << "Integrating..." << endl;

	
	n = search(path1);
	m = search(path2);
	m += 1;

	areafilename[4] = m + '0';
	convareafilename = areafilename;

	ofstream myFile;
	myFile.open(convareafilename);

	for (int j = 1; j <= n; j++) {

		filename[9] = j + '0';
		convfilename = filename;

		// Creating an object of CSsVWriter
		CSVReader reader(convfilename, ",");

		// Get the data from CSV File
		vector<vector<string>> dataList = reader.getData();

		// Print the content row by row on screen
		for (vector<string> vec : dataList)
		{
			voltage[i] = stod(vec[4]);
			time[i] = stod(vec[3]);
			//cout << i << " : " << time[i] <<" , " << voltage[i] << endl;
			if (i > 0) {
				if (voltage[i - 1] > 0.05)
				{
					area += (time[i] - time[i - 1]) * (voltage[i] + voltage[i - 1]) / 2;
				}
			}
			i++;
		}
		myFile << area << "," << endl;
		area = 0;
		i = 0;
		for (int k = 0; k < 2500; k++) {
			voltage[k] = 0;
			time[k] = 0;
		}
	}

	myFile.close();
	return 0;
}