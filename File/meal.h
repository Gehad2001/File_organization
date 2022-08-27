#pragma once
#include<iostream>
#include"varlen.h"
using namespace std;
class meal {
public:
	int id;
	char name[50];
	float price;
	char type[50]; 

	meal();
	//meal(int Id, char n[], float s);
	void InitRecord(VariableLengthRecord&);//set the record
	int Pack(VariableLengthRecord&);//write
	int UnPack(VariableLengthRecord&);//read
	void print(ostream&);
	void Print(meal&);
	void Print();

};
