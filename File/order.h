#pragma once
#include<iostream>
#include"varlen.h"
class Order
{
public:
	int id;
	float price;
	short count;
	char ordername[50];

	Order();
	void InitRecord(VariableLengthRecord&); //set Record  Configuration

	int Pack(VariableLengthRecord&);	

	int Unpack(VariableLengthRecord&); 

	void Print(Order&); 

	void Print();

};

