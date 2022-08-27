#include <iostream>
#include<fstream>
#include"varlen.h"
#include "chef.h"
using namespace std;
chef::chef()
{
	ID =0;
	salary = 0;
	name[0] = 0;
	phone[0] = 0; 
	workingHours = 0;
}
//chef::chef(short id, char na[], float sal)
//{
//	ID = id;
//	strcpy_s(name, na);
//	salary = sal;
//}
void chef::InitRecord(VariableLengthRecord& record)
{
	record.init(5);
	record.AddField(0, 'F', 4) ; // id 
	record.AddField(1, 'F', 4); // salary 
	record.AddField(2, 'F', 2); // working Hours  
	record.AddField(3, 'D', '|'); // name 
	record.AddField(4, 'D', '|'); // phone 
}
int chef::Pack(VariableLengthRecord& record)
{
	int result, recordSize = 0;   

	recordSize = 4 + 4 + 2 + strlen(name)+ 1 + strlen(phone) + 1;

	record.Clear(recordSize);

	result = record.Pack(0, (void*)&ID, sizeof(int));
	result = result && record.Pack(1, (void*)&salary, sizeof(float)); 
	result = result && record.Pack(2, (void*)&workingHours, sizeof(short));
	result = result && record.Pack(3, (void*)&name, strlen(name));
	result = result && record.Pack(4, (void*)&phone, strlen(phone));


	return result;
}
int chef::Unpack(VariableLengthRecord& record)
{
	int result;
	result = record.Unpack(0, (char*)&ID);
	result = result && record.Unpack(1, (char*)&salary); 
	result = result && record.Unpack(2, (char*)&workingHours);
	result = result && record.Unpack(3, (char*)&name, true);
	result = result && record.Unpack(4, (char*)&phone, true);
	return result;
}
void chef::Print(chef& c) {
	cout << "Chef : " << endl;
	cout << "\tID : " << c.ID << endl;
	cout << "\tName : " << c.name << endl;
	cout << "\tWorking Hours : " << c.workingHours << endl;
	cout << "\tSalary : " << c.salary << endl;
	cout << "\tPhone : " << c.phone << endl;
}

void chef::Print() {
	cout << "Chef : " << endl;
	cout << "\tID : " << ID << endl;
	cout << "\tName : " << name << endl;
	cout << "\tWorking Hours : " << workingHours << endl;
	cout << "\tSalary : " << salary << endl;
	cout << "\tPhone : " << phone << endl;
}
