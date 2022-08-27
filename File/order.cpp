#include <iostream>
#include<fstream>
#include"order.h" 
using namespace std;

Order::Order()
{
	id = 0 ;
	price = 0 ;
	count = 0 ;
	ordername[0] =  0 ;
}
void Order::InitRecord(VariableLengthRecord& record)
{
	record.init(4);
	record.AddField(0, 'F', 4);  // id 
	record.AddField(1, 'F', 4); // price 
	record.AddField(2, 'F', 2); // cound 
	record.AddField(3, 'D', '|');  // order name 

}

int Order::Pack(VariableLengthRecord& record)
{
	bool result;
	int recordSize = 0;  

	recordSize = 4 + 4 + 2 + strlen(ordername) + 1;

	record.Clear(recordSize);

	result = record.Pack(0, (void*)&id, 4);
	result = result && record.Pack(1, (void*)&price, 4);
	result = result && record.Pack(2, (void*)&count, 2);
	result = result && record.Pack(3, (void*)&ordername, strlen(ordername));

	return result;
}

int Order::Unpack(VariableLengthRecord& record)
{
	bool result;
	result = record.Unpack(0, (char*)&id);
	result = result && record.Unpack(1, (char*)&price);
	result = result && record.Unpack(2, (char*)&count);
	result = result && record.Unpack(3, (char*)&ordername, true);
	return result;
}

void Order::Print(Order& order1) {
	cout << "Order  " << endl;
	cout << "\tId      :  " << order1.id << endl;
	cout << "\tOrdername :  " << order1.ordername << endl;
	cout << "\tPrice   : " << order1.price << endl;
	cout << "\tCount   : " << order1.count << endl;
}

void Order::Print() {
	cout << "Order  " << endl;
	cout << "\tId      :  " << id << endl;
	cout << "\tOrdername :  " << ordername << endl;
	cout << "\tPrice   : " <<price << endl;
	cout << "\tCount   : " << count << endl;
}

