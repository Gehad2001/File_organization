#pragma once
#include <iostream>
#include "varlen.h"

using namespace std;

class supplier
{
public:
	int id;
	char name[50] , city[50] , phone[20] , email[50];

	supplier();
	//supplier(int iid, char na, int ph);

	void InitRecord(VariableLengthRecord&);   // set Record
	int  Pack(VariableLengthRecord&);	      // Write
	int  Unpack(VariableLengthRecord&);       // Read
	void Print(ostream&);
	void Print(supplier&);
	void Print();
};