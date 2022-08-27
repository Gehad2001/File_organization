#include <iostream>
#include<fstream>
#include "meal.h"
using namespace std;
meal::meal() {
	id = 0;
	name[0] = 0;
	price = 0;
	type[0] = 0;
}

void meal::InitRecord(VariableLengthRecord& recode) {
	recode.init(4);
	recode.AddField(0, 'F', 4); // id 
	recode.AddField(1, 'D', '|'); // name 
	recode.AddField(2, 'F', 4);//price
	recode.AddField(3, 'D', '|');
}

int meal::Pack(VariableLengthRecord& record) { 
	int result, recordSize = 0;
	recordSize = 4 + strlen(name) + 1 + 4 + strlen(type) + 1;

	record.Clear(recordSize);

	result = record.Pack(0, (void*)&id, 4);
	result = result && record.Pack(1, (void*)&name, strlen(name));
	result = result && record.Pack(2, (void*)&price, 4);
	result = result && record.Pack(3, (void*)&type, strlen(type));

	return result;
}

int meal::UnPack(VariableLengthRecord& record) {
	int result;
	result = record.Unpack(0, (char*)&id);
	result = result && record.Unpack(1, (char*)&name, true);
	result = result && record.Unpack(2, (char*)&price);
	result = result && record.Unpack(3, (char*)&type, true);


	return result;
}
void meal::Print(meal& c) {
	cout << "Meal" << endl;
	cout << "\tID : " << c.id << endl;
	cout << "\tName : " << c.name << endl;
	cout << "\tPrice : " << c.price << endl;
	cout << "\tType : " << c.type << endl;

}void meal::Print() {
	cout << "Meal" << endl;
	cout << "\tID : " << id << endl;
	cout << "\tName : " <<name << endl;
	cout << "\tPrice : " << price << endl;
	cout << "\tType : " << type << endl;
}

