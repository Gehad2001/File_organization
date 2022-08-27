#include <iostream>
#include<fstream>
#include "supplier.h"
using namespace std;
supplier::supplier()
{
	id = 0;
	name[0] = 0;
	phone[0] = 0;
	city[0] = 0; 
	email[0] = 0; 
}

void supplier::InitRecord(VariableLengthRecord& recode)
{
	recode.init(5);
	recode.AddField(0, 'F', 4);     // id    
	recode.AddField(1, 'D', '|');    // name 
	recode.AddField(2, 'D', '|');    // phone
	recode.AddField(3, 'D', '|');    // city
	recode.AddField(4, 'D', '|');    // email
}

int supplier::Pack(VariableLengthRecord& record)
{
	int result, recordSize = 0;

	recordSize = 4 + strlen(name)+ 1 + strlen(phone) + 1 + strlen(city) + 1 + strlen(email) + 1; 
	record.Clear(recordSize);

	result = record.Pack(0, (void*)&id, 4);
	result = result && record.Pack(1, (void*)&name, strlen(name));
	result = result && record.Pack(2, (void*)&phone, strlen(phone));
	result = result && record.Pack(3, (void*)&city, strlen(city));
	result = result && record.Pack(4, (void*)&email, strlen(email));

	return result;
}

int  supplier::Unpack(VariableLengthRecord& record) {
	int result;
	result = record.Unpack(0, (char*)&id);
	result = result && record.Unpack(1, (char*)&name, true);
	result = result && record.Unpack(2, (char*)&phone , true );
	result = result && record.Unpack(3, (char*)&city, true );
	result = result && record.Unpack(4, (char*)&email, true );

	return result;
}
void supplier::Print(supplier& supplier1) {
	cout << " Supplier : " << endl;
	cout << "\t Name  : " << supplier1.name << endl;
	cout << "\t City : " << supplier1.city << endl;
	cout << "\t Phone : " << supplier1.phone << endl;
	cout << "\t Email : " << supplier1.email << endl;
	cout << "\t ID    : " << supplier1.id << endl;
}
void supplier::Print() {
	cout << " Supplier : " << endl;
	cout << "\t Name  : " << name << endl;
	cout << "\t City : " << city << endl;
	cout << "\t Phone : " << phone << endl;
	cout << "\t Email : " << email << endl;
	cout << "\t ID    : " << id << endl;
}