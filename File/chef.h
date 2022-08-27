#pragma once
#include <iostream>
#include "varlen.h"
using namespace std;
class chef
{
public:
	int ID;
	float salary;
	short workingHours; 
	char name[50];
	char phone[20];


	chef();
//	chef(short id, char na[], float sal);

	void InitRecord(VariableLengthRecord&);
	int Pack(VariableLengthRecord&);	
	int Unpack(VariableLengthRecord&);  
	void Print(ostream&);
	void Print(chef&); 
	void Print(); 

};