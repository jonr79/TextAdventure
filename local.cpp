#include <windows.h>		//I'm not sure
#include <iostream>			//Always needed
#include <fstream>			//For file accessing
#include <string>			//For Strings

#include "local.h"
;
using namespace std;

local::local()
{
	X = 1;
	Y = 1;
	script = "Class Created";
}
void local::setscript(string sz1)
{
	script = sz1;
}
void local::setX(int x)
{
	X = x;
}

void local::setY(int y)
{
	Y = y;
}
string local::getscript()
{
	return script;
}
int local::getX()
{
	return X;
}
int local::getY()
{
	return Y;
}

void local::setup(string map)
{
	ifstream fin;
	bool Lfound = false;
	int X2 = 0;
	int Y2 = 0;
	
	string szL;
	string szS;
	string szHolder;

	fin.open(map.c_str());
	if(fin.fail())
	{
		cout << "ERROR ERROR ERROR";
		system("pause");
		exit(0);
	}
	fin.seekg(0);	

	while(!Lfound)
	{
		fin >> szHolder;
		if(szHolder != "X:")
			if(szHolder == "MC:")
			{
				fin >> szHolder;
				fin >> szHolder >> szHolder;
				fin >> szHolder >> szHolder;
			}
			else
                fin >> szHolder;
		else
		{
			fin >> X2;
			fin >> szHolder;
			fin >> Y2;
			fin >> szHolder;
			getline(fin,szL);
			fin >> szHolder;
			getline(fin,szS);
		}

		if(X2 == X && Y2 == Y)
			Lfound = true;
		if(fin.eof() && !Lfound)
		{
			szS = " there is no path to follow.";
			Lfound = true;
		}
	}
	script = szS;
}