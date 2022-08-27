#pragma once
#include"varlen.h"
class Customer {
public:
	int id;
	char name[50];
	char address[50];
	float recipe;
	char phone[50]; 

	Customer();
	void InitRecord(VariableLengthRecord&);
	int Pack(VariableLengthRecord&);
	int UnPack(VariableLengthRecord&);
	void Print(Customer&); 
	void Print();

};
