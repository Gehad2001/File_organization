#pragma once
#include"varlen.h"
class waiter {
public:

	int id;
	char name[50];
	float salary;
	char phone[20];

	waiter();
	void InitRecord(VariableLengthRecord&);
	int Pack(VariableLengthRecord&);
	int UnPack(VariableLengthRecord&);
	void Print(waiter&);
	void Print();
};
