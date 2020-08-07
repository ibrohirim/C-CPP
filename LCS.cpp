// LCS.cpp : Defines the entry point for the console application.
//

#ifndef __APPLE__
#include "sys/stdafx.h"
#endif
#include <iostream> 
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

using namespace std;
// declaration of global arrays for the "arrows" and the number values
// and declaration of the number values of the "arrows"
int b[100][100];
int c[100][100];
int up = 0;
int left1 = 1;
int diag = 2;
//creation of the "arrows" table with the aid of the numerical table
void lcsLength(string x, string y) {
	int m = x.length();
	int n = y.length();
	for (int i = 1; i <= m; i++) {
		c[i][0] = 0;
	}
	for (int j = 0; j <= n; j++) {
		c[0][j] = 0;
	}
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			if (x[i-1] == y[j-1]) {
				c[i][j] = c[i - 1][j - 1] + 1;
				b[i][j] = diag;
			}
			else if (c[i - 1][j] >= c[i][j - 1]){
				c[i][j] = c[i - 1][j];
				b[i][j] = up;
			}
			else {
				c[i][j] = c[i][j - 1];
				b[i][j] = left1;
			}
		}
	}
}

//walking through the "arrow" table
void printLcs(string x, int i, int j) {
	if (i == 0 || j == 0) {
		return;
	}
	if (b[i][j] == diag) {
		printLcs(x, i - 1, j - 1);
		cout << x[i-1];
	}
	else if (b[i][j] == up) {
		printLcs(x, i - 1, j);
	}
	else {
		printLcs(x, i, j - 1);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream ifs;
	ofstream of;

	//checks whether the command-line parameteras has been entered
	if (argc != 2) {
		cout << "You forgot the command-line paramter!" << endl;
		return 1;
	}

	ifs.open(argv[1], ios::in);
	//checks whether the file exists in the working directory
	if (!ifs) {
		cout << "Cannot find file" << endl;
		return 1;
	}

	//reading in the strings and calling the lcs functions on them
	string str1, str2;
	getline(ifs, str1);
	getline(ifs, str2);
	cout << "String X: " << str1 << endl;
	cout << "String Y: " << str2 << endl;
	lcsLength(str1, str2);
	cout << "LCS: ";
	printLcs(str1, str1.length(), str2.length());
	cout << endl;


	return 0;
}

