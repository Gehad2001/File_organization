#include <iostream>
#include<fstream>
#include "waiter.h"
waiter::waiter() {
	id = 0;
	name[0] = 0;
	salary = 0.00;
	phone[0] = 0; 
}

void waiter::InitRecord(VariableLengthRecord& recode) {
	recode.init(4);
	recode.AddField(0, 'F', 4); // id 
	recode.AddField(1, 'D', '|'); // name 
	recode.AddField(2, 'F', 4);//salary
	recode.AddField(3, 'D', '|');//salary
}

int waiter::Pack(VariableLengthRecord& record) { // write in memory 
	int result, recordSize = 0;
	recordSize = 4 + strlen(name) + 1 + 4 + strlen(phone) + 1; 
	record.Clear(recordSize); 

	result = record.Pack(0, (void*)&id, 4);
	result = result && record.Pack(1, (void*)&name, strlen(name));

	result = result && record.Pack(2, (void*)&salary, 4);
	result = result && record.Pack(3, (void*)&phone, strlen(phone));

	return result;
}

int waiter::UnPack(VariableLengthRecord& record) {
	int result;
	result = record.Unpack(0, (char*)&id);
	result = result && record.Unpack(1, (char*)&name, true);
	result = result && record.Unpack(2, (char*)&salary);
	result = result && record.Unpack(3, (char*)&phone , true);


	return result;
}

void waiter::Print(waiter& c) {
	cout << "\tID : " << id << endl; 
	cout << "\tName : " << name << endl;
	cout << "\tSalary : " << salary << endl;
	cout << "\tPhone : " << phone << endl;

}void waiter::Print() {
	cout << "\tID : " << id << endl; 
	cout << "\tName : " << name << endl;
	cout << "\tSalary : " << salary << endl;
	cout << "\tPhone : " << phone << endl;

}

