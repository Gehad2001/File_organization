#include <iostream>
#include<fstream>
#include"customer.h"
#include"varlen.h"
using namespace std;

Customer::Customer() {
	id = 0;
	name[0] = '\0';
	address[0] = '\0';
	recipe = 0;
	phone [0]= '\0';
}

void Customer::InitRecord(VariableLengthRecord& recode) {
	recode.init(5);
	recode.AddField(0, 'F', 4); // id 
	recode.AddField(1, 'D', '|'); // name 
	recode.AddField(2, 'F', 4); // recipe 
	recode.AddField(3, 'D', '|'); // address 
	recode.AddField(4, 'D', '|'); // phone
}

int Customer::Pack(VariableLengthRecord& record) { 
	int result, recordSize = 0;
	recordSize = 4 + strlen(name) + 1  + 4 + strlen(address) + 1 + strlen(phone) + 1;
	record.Clear(recordSize);

	result = record.Pack(0, (void*)&id, 4);
	result = result & record.Pack(1, (void*)&name, strlen(name));
	result = result & record.Pack(2, (void*)&recipe, 4);
	result = result & record.Pack(3, (void*)&address, strlen(address)); 
	result = result & record.Pack(4, (void*)&phone, strlen(phone));

	return result; 
}

int Customer::UnPack(VariableLengthRecord& record) {
	int result;
	result = record.Unpack(0, (char*)&id);
	result = result && record.Unpack(1, (char*)&name, true);
	result = result && record.Unpack(2, (char*)&recipe);
	result = result && record.Unpack(3, (char*)&address, true);
	result = result && record.Unpack(4, (char*)&phone, true);

	return result;
}

void Customer::Print(Customer& c) { 

	cout << "Customer : " << endl;
	cout << "\tID : " << id << endl;
	cout << "\tName : " << name << endl;
	cout << "\tAddress : " << address << endl;
	cout << "\trecipe : " << recipe << endl;
	cout << "\tPhone : " << phone << endl; 


}
void Customer::Print() {

	cout << "Customer : " << endl;
	cout << "\tID : " << id << endl;
	cout << "\tName : " << name << endl;
	cout << "\tAddress : " << address << endl;
	cout << "\trecipe : " << recipe << endl;
	cout << "\tPhone : " << phone << endl;


}