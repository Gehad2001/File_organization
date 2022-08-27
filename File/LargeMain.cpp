#include<iostream>
#include<fstream>
#include"waiter.h"
#include"chef.h"
#include"customer.h"
#include"meal.h"
#include"order.h"
#include"supplier.h"

using namespace std;

struct Index {
	int id;
	int ofSet ;
};

int main() {


	supplier  supplier1;
	chef  chef1;
	Index index;
	Customer  customer;
	waiter  waiter1;
	meal  meal1;
	Order  order1;
	VariableLengthRecord outRecord, inRecord;
	Index n[50];


	cout << " 1 - Supplier " << endl;
	cout << " 2 - Chef " << endl;
	cout << " 3 - Customer " << endl;
	cout << " 4 - Waiter " << endl;
	cout << " 5 - Meal " << endl;
	cout << " 6 - Order " << endl;
	cout << " 10- Exit " << endl;

	int data_File = -1 ; 
	cout << "Choose The File" << endl;
	cin >> data_File; 

	while (data_File != 10)
	{
		if (data_File == 1) { // Supplier 
			cout << "1  - Write   \n";
			cout << "11 - Write in Avail List    \n";
			cout << "2  - Read    \n";
			cout << "3  - Search  \n";
			cout << "4  - Update  \n";
			cout << "5  - Delete  \n";
			cout << "0  - End     \n";


			int p = -1;
			cin >> p;

			// supplier  
			while (p != 0)
			{
				if (p == 1) {

					// Write 
					fstream TestOut; 
					fstream NewIndex;
					char n = 'r';

					while (n != 'n') {
						TestOut.open("Files//supplierData.txt", ios::out | ios::binary | ios::app);
						TestOut.seekp(0, ios::end);

						cout << "Enter Name : " << endl;
						cin >> supplier1.name;
						cout << "Enter City : " << endl;
						cin >> supplier1.city;
						cout << "Etner Phone : " << endl;
						cin >> supplier1.phone;
						cout << "Etner Email : " << endl;
						cin >> supplier1.email;
						cout << "Etner ID : " << endl;
						cin >> supplier1.id;

						supplier1.InitRecord(outRecord);
						outRecord.WriteHeader(TestOut);  // Only Once. 
						cout << "pack chef " << supplier1.Pack(outRecord) << endl;
						outRecord.Write(TestOut, index.ofSet, p);
						TestOut.close();

						// Index File
						index.id = supplier1.id;
						NewIndex.open("Files//supplierIndex.txt", ios::binary | ios::out | ios::app);
						NewIndex.write((char*)&index, sizeof(index));
						NewIndex.close();

						cout << "Are you want Continue !?" << endl;
						cin >> n;

						cout << "************************************ \n";
					}


				} 
				else if (p == 11) {
					// Write 
					fstream TestOut;
					fstream NewIndex;
					char n = 'r';

					while (n != 'n') {
						cout << "\tWrite in Avail List : " << endl; 
						cout << "Enter Name : " << endl;
						cin >> supplier1.name;
						cout << "Enter City : " << endl;
						cin >> supplier1.city;
						cout << "Etner Phone : " << endl;
						cin >> supplier1.phone;
						cout << "Etner Email : " << endl;
						cin >> supplier1.email;
						cout << "Etner ID : " << endl;
						cin >> supplier1.id;

						TestOut.open("Files//supplierData.txt", ios::binary | ios::out | ios::in);

						supplier1.InitRecord(outRecord);
						outRecord.WriteHeader(TestOut);  // Only Once. 
						cout << "pack chef " << supplier1.Pack(outRecord) << endl;
						outRecord.WriteinAvailableList(TestOut, index.ofSet);
						TestOut.close();

						// Index File
						index.id = supplier1.id;
						NewIndex.open("Files//supplierIndex.txt", ios::binary | ios::out | ios::app);
						NewIndex.write((char*)&index, sizeof(index));
						NewIndex.close();

						cout << "Are you want Continue !?" << endl;
						cin >> n;

						cout << "************************************ \n";
					}


				}

				else if (p == 2) {
					// Read 

					cout << "\t\tRead From File " << endl;

					fstream TestIn("Files//supplierData.txt", ios::in | ios::binary);
					TestIn.seekg(0, ios::beg);
					inRecord.ReadHeader(TestIn);


					fstream indexFile;
					indexFile.open("Files//supplierIndex.txt", ios::binary | ios::in);

					int num = inRecord.RecordeNumbers(indexFile);
					cout << "The Number of Recodes : " << num << endl;

					int idOffSet;
					indexFile.seekg(0, ios::beg);
					for (int i = 0; i < num; i++) {
						indexFile.seekg(4, ios::cur); // skip id 
						indexFile.read((char*)&idOffSet, sizeof(int));
						cout << "OFSet : " << idOffSet << endl;
						TestIn.seekg(idOffSet, ios::beg);
						cout << "read " << inRecord.Read(TestIn) << endl;
						int t = supplier1.Unpack(inRecord);
						cout << "unpack " << t << endl;
						supplier1.Print(supplier1);

					}
					indexFile.close(); 
					TestIn.close();

					cout << "************************************ \n";

				}// end read 

				else  if (p == 3) {
					// Search 
					cout << "\t\tIN Search " << endl;

					fstream SearchStream;
					fstream readIndex;

					int Num = 0;
					int ofset = 0;
					char t = 't';
					while (t != 'n')
					{
						SearchStream.open("Files//supplierData.txt", ios::in | ios::binary);

						cout << "Etner the ID of Recorde " << endl; cin >> Num;

						fstream NewIndex;
						readIndex.open("Files//supplierIndex.txt", ios::binary | ios::in);
						int searchid = inRecord.SearchID(readIndex, SearchStream, Num, ofset);
						readIndex.close();
						if (searchid) {
							cout << "ID : " << Num << endl;
							cout << "OF Set : " << ofset << endl;

							cout << "read " << inRecord.Read(SearchStream) << endl;

							cout << "unpack " << supplier1.Unpack(inRecord) << endl;

							supplier1.Print(supplier1);
						}

						SearchStream.close();

						cout << "Are you want Sreach to another recorde !?" << endl;
						cin >> t;

						cout << "************************************ \n";
					}
				}// end Write  

				else if (p == 4) { // update  
				char t = 't';
				while (t != 'n') {
					supplier  supplier2;
					fstream DataFile;
					fstream indexFile;

					int id;
					cout << "Enter ID to update : " << endl;
					cin >> id;
					int offSet = 0;

					indexFile.open("Files//supplierIndex.txt", ios::binary | ios::in | ios::out);
					DataFile.open("Files//supplierData.txt", ios::binary | ios::in | ios::out);

					int newSize = 0;
					int oldSize = 0;
					DataFile.seekg(0, ios::beg);
					int test = inRecord.SearchID(indexFile, DataFile, id, offSet);

					if (test) {
						cout << "read " << inRecord.Read(DataFile, oldSize) << endl;
						cout << "unpack " << supplier2.Unpack(inRecord) << endl;
						supplier2.Print(supplier2);
						DataFile.close();

						supplier newRecorde;
						cout << "Enter the updated Data of Recorde " << endl;
						cout << "Enter Name : " << endl;
						cin >> newRecorde.name;
						cout << "Enter City : " << endl;
						cin >> newRecorde.city;
						cout << "Etner Phone : " << endl;
						cin >> newRecorde.phone;
						cout << "Etner Email : " << endl;
						cin >> newRecorde.email;
						cout << "Etner ID : " << endl;
						cin >> newRecorde.id;

						DataFile.open("Files//supplierData.txt", ios::binary | ios::in | ios::out);

						//DataFile.seekg(0, ios::beg);
						//inRecord.ReadHeader(DataFile);  
						newSize = 4 + strlen(newRecorde.name) + 1 + strlen(newRecorde.phone) + 1 + strlen(newRecorde.city) + 1 + strlen(newRecorde.email) + 1;

						 if (oldSize >= newSize) {
							 cout << "In First Case " << endl;

							DataFile.seekp(offSet, ios::beg);

							cout << "To APP : " << DataFile.tellp() << endl;
							newRecorde.InitRecord(outRecord);
							newRecorde.Pack(outRecord);
							outRecord.Write(DataFile, newSize);

							outRecord.UpdateRecorde(indexFile, DataFile, newSize, oldSize, supplier2.id, newRecorde.id);
						}

						else {

							 indexFile.close();
							 //   Add the end of Flie 
							 cout << "In Second Case " << endl;
							 DataFile.seekp(0, ios::end);
							 cout << "To APP : " << DataFile.tellp() << endl;
							 newRecorde.InitRecord(outRecord);
							 cout << "pack chef " << newRecorde.Pack(outRecord) << endl;
							 outRecord.Write(DataFile, index.ofSet, p);
							 DataFile.close();
							 index.id = newRecorde.id;

							 // Index File
							 fstream NewIndex;
							 NewIndex.open("Files//supplierIndex.txt", ios::binary | ios::out | ios::app);
							 NewIndex.write((char*)&index, sizeof(index));
							 NewIndex.close();

							 // -----------------------------------------------------------------
							 indexFile.open("Files//supplierIndex.txt", ios::binary | ios::in | ios::out);
							 DataFile.open("Files//supplierData.txt", ios::binary | ios::in | ios::out);

							 outRecord.Delete(DataFile, indexFile, offSet);
							 int renum = inRecord.RecordeNumbers(indexFile);

							// cout << "postion of deleted record = " << id << endl;

							 --id;

							 indexFile.seekg(0, ios::beg);

							 for (int i = 0; i < renum; i++) { // read all index 
								 indexFile.read((char*)&n[i].id, sizeof(int));
								 indexFile.read((char*)&n[i].ofSet, sizeof(int));
								
							 }
							 indexFile.close();
							 remove("Files//supplierIndex.txt");

							 indexFile.open("Files//supplierIndex.txt", ios::out | ios::in | ios::binary | ios::trunc);

							 indexFile.seekp(0, ios::beg);
							 for (int i = 0; i < renum; i++) {
								 if (i == id) {
									 continue;
								 }
								 else {
									 indexFile.write((char*)&n[i], sizeof(n[i]));
									 cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
								 }
							 }
							 indexFile.close();
						}

						DataFile.close();

					}



					cout << "Are you want Update another recorde !?" << endl;
					cin >> t;

					cout << "************************************ \n";
				}


				}

				else if (p == 5) {

				cout << "\t\tIN DELET " << endl;
				Index n[50];
				fstream deletStream;
				fstream deleteIndex;
				int Num = 0;
				int ofset = 0;
				char t = 't';

				int oldSize;
				while (t != 'n')
				{
					deletStream.open("Files//supplierData.txt", ios::out | ios::in | ios::binary);

					cout << "Etner the ID of Recorde " << endl; cin >> Num;

					deleteIndex.open("Files//supplierIndex.txt", ios::binary | ios::in | ios::out);

					deletStream.seekg(0, ios::beg);
					int test = inRecord.SearchID(deleteIndex, deletStream, Num, ofset);

					if (test) {
						cout << "read " << inRecord.Read(deletStream, oldSize) << endl;
						cout << "unpack " << supplier1.Unpack(inRecord) << endl;
						supplier1.Print(supplier1);

						deletStream.close();// error at the end of file 

						deletStream.open("Files//supplierData.txt", ios::out | ios::in | ios::binary);

						outRecord.Delete(deletStream, deleteIndex , ofset);

						deletStream.close();

						/************************delete in index***********************************/
						int renum = inRecord.RecordeNumbers(deleteIndex);
						--Num;
						//cout << "postion of deleted record = " << Num << endl;

						deleteIndex.seekg(0, ios::beg);

						for (int i = 0; i < renum; i++) { // read all index 
							deleteIndex.read((char*)&n[i].id, sizeof(int));
							deleteIndex.read((char*)&n[i].ofSet, sizeof(int));
							//cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
						}
						deleteIndex.close();
						remove("Files//supplierIndex.txt");

						deleteIndex.open("Files//supplierIndex.txt", ios::out | ios::in | ios::binary | ios::trunc);

						deleteIndex.seekp(0, ios::beg);
						for (int i = 0; i < renum; i++) {
							if (i == Num) {
								continue;
							}
							else {
								deleteIndex.write((char*)&n[i], sizeof(n[i]));
								cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
							}
						}
						deleteIndex.close();
					}


					cout << "Are you want Delete to another recorde !?" << endl;
					cin >> t;

				}
				}
				


				cout << "1  - Write   \n";
				cout << "11 - Write in Avail List    \n";
				cout << "2  - Read    \n";
				cout << "3  - Search  \n";
				cout << "4  - Update  \n";
				cout << "5  - Delete  \n";
				cout << "0  - End     \n";
				cin >> p;

			}// end while

		} // End Supplier 

		else if (data_File == 2){ // chef 

		cout << "1  - Write   \n";
		cout << "11 - Write in Avail List    \n";
		cout << "2  - Read    \n";
		cout << "3  - Search  \n";
		cout << "4  - Update  \n";
		cout << "5  - Delete  \n";
		cout << "0  - End     \n";

			int p = -1;
			cin >> p;
			

			while (p != 0)
			{
				if (p == 1) {

					// Write 
					fstream TestOut; 
					fstream NewIndex;

					char n = 'r';

					while (n != 'n') {
						TestOut.open("Files//chefData.txt", ios::out | ios::binary | ios::app);
						TestOut.seekp(0, ios::end);

						cout << "Enter Name : " << endl;
						cin >> chef1.name;
						cout << "Enter Salary : " << endl;
						cin >> chef1.salary;
						cout << "Enter Phone : " << endl;
						cin >> chef1.phone;
						cout << "Enter Working Hours :  " << endl;
						cin >> chef1.workingHours;
						cout << "Etner ID : " << endl;
						cin >> chef1.ID;

						chef1.InitRecord(outRecord);
						outRecord.WriteHeader(TestOut);  // Only Once . 
						cout << "pack chef " << chef1.Pack(outRecord) << endl;
						outRecord.Write(TestOut , index.ofSet, p);
						TestOut.close();

						// Index File
						index.id = chef1.ID; 
						NewIndex.open("Files//chefIndex.txt", ios::binary | ios::out | ios::app);
						NewIndex.write((char*)&index, sizeof(index));
						NewIndex.close();

						cout << "Are you want Continue !?" << endl;
						cin >> n;

						cout << "************************************ \n";
					}

				}

				else if (p == 11) {

					// Write 
					fstream TestOut;
					fstream NewIndex;

					char n = 'r';

					while (n != 'n') {
						cout << "\tWrite in Avail List : " << endl;

						cout << "Enter Name : " << endl;
						cin >> chef1.name;
						cout << "Enter Salary : " << endl;
						cin >> chef1.salary;
						cout << "Enter Phone : " << endl;
						cin >> chef1.phone;
						cout << "Enter Working Hours :  " << endl;
						cin >> chef1.workingHours;
						cout << "Etner ID : " << endl;
						cin >> chef1.ID;

						TestOut.open("Files//chefData.txt", ios::binary | ios::out | ios::in);

						chef1.InitRecord(outRecord);
						outRecord.WriteHeader(TestOut);  // Only Once . 
						cout << "pack chef " << chef1.Pack(outRecord) << endl;
						outRecord.WriteinAvailableList(TestOut, index.ofSet);
						TestOut.close();

						// Index File
						index.id = chef1.ID;
						NewIndex.open("Files//chefIndex.txt", ios::binary | ios::out | ios::app);
						NewIndex.write((char*)&index, sizeof(index));
						NewIndex.close();

						cout << "Are you want Continue !?" << endl;
						cin >> n;

						cout << "************************************ \n";
					}

				}

				else if (p == 2) {
					// Read 

					cout << "\t\tRead From File " << endl;

					fstream TestIn("Files//chefData.txt", ios::in | ios::binary);
					TestIn.seekg(0, ios::beg);
					inRecord.ReadHeader(TestIn);

					fstream indexFile;
					indexFile.open("Files//chefIndex.txt", ios::binary | ios::in);

					int num = inRecord.RecordeNumbers(indexFile);
					cout << "The Number of Recodes : " << num << endl;

					int idOffSet;
					indexFile.seekg(0, ios::beg);
					for (int i = 0; i < num; i++) {
						indexFile.seekg(4, ios::cur); // skip id 
						indexFile.read((char*)&idOffSet, sizeof(int));
						cout << "OFSet : " << idOffSet << endl;
						TestIn.seekg(idOffSet, ios::beg);
						cout << "read " << inRecord.Read(TestIn) << endl;
						int t = chef1.Unpack(inRecord);
						cout << "unpack " << t << endl;
						chef1.Print(chef1);

					}

					TestIn.close();

					cout << "************************************ \n";

				}// end read 

				else  if (p == 3) {
					// Search 
					cout << "\t\tIN Search " << endl;

					fstream SearchStream;
					fstream readIndex;

					int Num = 0;
					int ofset = 0;
					char t = 't';
					while (t != 'n')
					{
						SearchStream.open("Files//chefData.txt", ios::in | ios::binary);

						cout << "Etner the ID of Recorde " << endl; cin >> Num;

						fstream NewIndex;
						readIndex.open("Files//chefIndex.txt", ios::binary | ios::in);
						int searchid = inRecord.SearchID(readIndex, SearchStream, Num, ofset);
						readIndex.close();
						if (searchid) {
							cout << "ID : " << Num << endl;
							cout << "OF Set : " << ofset << endl;

							cout << "read " << inRecord.Read(SearchStream) << endl;

							cout << "unpack " << chef1.Unpack(inRecord) << endl;

							chef1.Print(chef1);
						}

						SearchStream.close();

						cout << "Are you want Sreach to another recorde !?" << endl;
						cin >> t;

						cout << "************************************ \n";
					}
				}// end Write  

				else if (p == 4) {
					char t = 't';
					while (t != 'n') {
						//chef chef2;
						fstream DataFile;
						fstream indexFile;

						int id;
						cout << "Enter ID to update : " << endl;
						cin >> id;
						int offSet = 0;

						indexFile.open("Files//chefIndex.txt", ios::binary | ios::in | ios::out);
						DataFile.open("Files//chefData.txt", ios::binary | ios::in | ios::out);

						int newSize = 0;
						int oldSize = 0;
						DataFile.seekg(0, ios::beg);
						int test = inRecord.SearchID(indexFile, DataFile, id, offSet);

						if (test) {
							cout << "read " << inRecord.Read(DataFile, oldSize) << endl;
							cout << "unpack " << chef1.Unpack(inRecord) << endl;
							chef1.Print(chef1);
							DataFile.close();

							chef newRecorde;
							cout << "Enter the updated Data of Recorde " << endl;
							cout << "Enter Name : " << endl;
							cin >> newRecorde.name;
							cout << "Enter Salary : " << endl;
							cin >> newRecorde.salary;
							cout << "Enter Phone : " << endl;
							cin >> newRecorde.phone;
							cout << "Enter Working Hours :  " << endl;
							cin >> newRecorde.workingHours;
							cout << "Etner ID : " << endl;
							cin >> newRecorde.ID;

							DataFile.open("Files//chefData.txt", ios::binary | ios::in | ios::out);

							//DataFile.seekg(0, ios::beg);
							//inRecord.ReadHeader(DataFile);  

							newSize = 4 + 4 + 2 + strlen(newRecorde.name) + 1 + strlen(newRecorde.phone) + 1;

							if (oldSize >= newSize) {

								DataFile.seekp(offSet, ios::beg);

								cout << "To APP : " << DataFile.tellp() << endl;
								newRecorde.InitRecord(outRecord);
								newRecorde.Pack(outRecord);
								outRecord.Write(DataFile, newSize);

								outRecord.UpdateRecorde(indexFile, DataFile, newSize, oldSize, chef1.ID, newRecorde.ID);
							} 

							else {

								indexFile.close();
								//   Add the end of Flie 
								cout << "In Second Case " << endl;
								DataFile.seekp(0, ios::end);
								cout << "To APP : " << DataFile.tellp() << endl;
								newRecorde.InitRecord(outRecord);
								cout << "pack chef " << newRecorde.Pack(outRecord) << endl;
								outRecord.Write(DataFile, index.ofSet, p);
								DataFile.close();
								index.id = newRecorde.ID;

								// Index File
								fstream NewIndex;
								NewIndex.open("Files//chefIndex.txt", ios::binary | ios::out | ios::app);
								NewIndex.write((char*)&index, sizeof(index));
								NewIndex.close();

								// -----------------------------------------------------------------
								indexFile.open("Files//chefIndex.txt", ios::binary | ios::in | ios::out);
								DataFile.open("Files//chefData.txt", ios::binary | ios::in | ios::out);

								outRecord.Delete(DataFile, indexFile, offSet);
								int renum = inRecord.RecordeNumbers(indexFile);
								--id;
								//cout << "postion of deleted record = " << id << endl;

								indexFile.seekg(0, ios::beg);

								for (int i = 0; i < renum; i++) { // read all index 
									indexFile.read((char*)&n[i].id, sizeof(int));
									indexFile.read((char*)&n[i].ofSet, sizeof(int));
									
								}
								indexFile.close();
								remove("Files//chefIndex.txt");

								indexFile.open("Files//chefIndex.txt", ios::out | ios::in | ios::binary | ios::trunc);

								indexFile.seekp(0, ios::beg);
								for (int i = 0; i < renum; i++) {
									if (i == id) {
										continue;
									}
									else {
										indexFile.write((char*)&n[i], sizeof(n[i]));
										cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
									}
								}
								indexFile.close();
							}
							

							DataFile.close();
							

						}



						cout << "Are you want Update another recorde !?" << endl;
						cin >> t;

						cout << "************************************ \n";
					}

					
				} 

				else if (p == 5) {

				cout << "\t\tIN DELET " << endl;
				Index n[50];
				fstream deletStream;
				fstream deleteIndex;
				int Num = 0;
				int ofset = 0;
				char t = 't';

				int oldSize;
				while (t != 'n')
				{
					deletStream.open("Files//chefData.txt", ios::out | ios::in | ios::binary);

					cout << "Etner the ID of Recorde " << endl; cin >> Num;

					deleteIndex.open("Files//chefIndex.txt", ios::binary | ios::in | ios::out); 

					deletStream.seekg(0, ios::beg);
					int test = inRecord.SearchID(deleteIndex, deletStream, Num, ofset);

					if (test) {
						cout << "read " << inRecord.Read(deletStream, oldSize) << endl;
						cout << "unpack " << chef1.Unpack(inRecord) << endl;
						chef1.Print(chef1);
						deletStream.close();// error at the end of file 

						deletStream.open("Files//chefData.txt", ios::out | ios::in | ios::binary);

						outRecord.Delete(deletStream, deleteIndex , ofset);

						deletStream.close();

						/************************delete in index***********************************/
						int renum = inRecord.RecordeNumbers(deleteIndex);
						--Num;
						//cout << "postion of deleted record = " << Num << endl;

						deleteIndex.seekg(0, ios::beg);

						for (int i = 0; i < renum; i++) { // read all index 
							deleteIndex.read((char*)&n[i].id, sizeof(int));
							deleteIndex.read((char*)&n[i].ofSet, sizeof(int));
						}
						deleteIndex.close();
						remove("Files//chefIndex.txt");

						deleteIndex.open("Files//chefIndex.txt", ios::out | ios::in | ios::binary | ios::trunc);

						deleteIndex.seekp(0, ios::beg);
						for (int i = 0; i < renum; i++) {
							if (i == Num) {
								continue;
							}
							else {
								deleteIndex.write((char*)&n[i], sizeof(n[i]));
								cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
							}
						}
						deleteIndex.close();
					}


					cout << "Are you want Delete to another recorde !?" << endl;
					cin >> t;

				}
				}

				cout << "1  - Write   \n";
				cout << "11 - Write in Avail List    \n";
				cout << "2  - Read    \n";
				cout << "3  - Search  \n";
				cout << "4  - Update  \n";
				cout << "5  - Delete  \n";
				cout << "0  - End     \n";
				cin >> p;

			}// end while


		} // End chef 

		else if (data_File == 3) { // customer 
		cout << "1  - Write   \n";
		cout << "11 - Write in Avail List    \n";
		cout << "2  - Read    \n";
		cout << "3  - Search  \n";
		cout << "4  - Update  \n";
		cout << "5  - Delete  \n";
		cout << "0  - End     \n";
			int p = -1;
			cin >> p;

			while (p != 0)
			{
				if (p == 1) {

					// Write 

					fstream TestOut;  
					fstream NewIndex;
					char n = 'r';

					while (n != 'n') {

						TestOut.open("Files//customerData.txt", ios::out | ios::binary | ios::app);
						TestOut.seekp(0, ios::end);

						cout << "Enter Name : " << endl;
						cin >> customer.name;
						cout << "Enter Address" << endl;
						cin >> customer.address;
						cout << "Enter Phone : " << endl;
						cin >> customer.phone;
						cout << "Enter recipe : " << endl;
						cin >> customer.recipe;
						cout << "Etner ID : " << endl;
						cin >> customer.id;

						customer.InitRecord(outRecord);
						outRecord.WriteHeader(TestOut);  // Only Once.    
						cout << "pack customer " << customer.Pack(outRecord) << endl;
						outRecord.Write(TestOut, index.ofSet, p);
						TestOut.close();

						// Index File
						index.id = customer.id;
						NewIndex.open("Files//customerIndex.txt", ios::binary | ios::out | ios::app);
						NewIndex.write((char*)&index, sizeof(index));
						NewIndex.close();

						cout << "Are you want Continue !?" << endl;
						cin >> n;

						cout << "********************************************* \n";
					}

				}

				else if (p == 11) {

					// Write 

					fstream TestOut;
					fstream NewIndex;
					char n = 'r';

					while (n != 'n') {

	
						cout << "Enter Name : " << endl;
						cin >> customer.name;
						cout << "Enter Address" << endl;
						cin >> customer.address;
						cout << "Enter Phone : " << endl;
						cin >> customer.phone;
						cout << "Enter recipe : " << endl;
						cin >> customer.recipe;
						cout << "Etner ID : " << endl;
						cin >> customer.id;

						TestOut.open("Files//customerData.txt", ios::binary | ios::out | ios::in);

						customer.InitRecord(outRecord);
						outRecord.WriteHeader(TestOut);  // Only Once.    
						cout << "pack customer " << customer.Pack(outRecord) << endl;
						outRecord.WriteinAvailableList(TestOut, index.ofSet);
						TestOut.close();

						// Index File
						index.id = customer.id;
						NewIndex.open("Files//customerIndex.txt", ios::binary | ios::out | ios::app);
						NewIndex.write((char*)&index, sizeof(index));
						NewIndex.close();

						cout << "Are you want Continue !?" << endl;
						cin >> n;

						cout << "********************************************* \n";
					}

				}

				else if (p == 2) {
					// Read 

					cout << "\t\tRead From File " << endl;

					fstream TestIn("Files//customerData.txt", ios::in | ios::binary);
					TestIn.seekg(0, ios::beg);
					inRecord.ReadHeader(TestIn);

					fstream indexFile;
					indexFile.open("Files//customerIndex.txt", ios::binary | ios::in);

					int num = inRecord.RecordeNumbers(indexFile);

					cout << "The Number of Recodes : " << num << endl;
					int idOffSet;
					indexFile.seekg(0, ios::beg);
					for (int i = 0; i < num; i++) {
						indexFile.seekg(4, ios::cur); // skip id 
						indexFile.read((char*)&idOffSet, sizeof(int));
						cout << "OFSet : " << idOffSet << endl;
						TestIn.seekg(idOffSet, ios::beg);
						cout << "read " << inRecord.Read(TestIn) << endl;
						int t = customer.UnPack(inRecord);
						cout << "unpack " << t << endl;
						customer.Print(customer);

					}

					TestIn.close();

					cout << "********************************************* \n";

				}// end read 

				else  if (p == 3) {
					// Search 
					cout << "\t\tIN Search " << endl;

					fstream SearchStream;
					fstream readIndex;

					int Num = 0;
					int ofset = 0;
					char t = 't';
					while (t != 'n')
					{
						SearchStream.open("Files//customerData.txt", ios::in | ios::binary);

						cout << "Etner the ID of Recorde " << endl; cin >> Num;

						fstream NewIndex;
						readIndex.open("Files//customerIndex.txt", ios::binary | ios::in);
						int searchid = inRecord.SearchID(readIndex, SearchStream, Num, ofset);
						readIndex.close();
						if (searchid) {
							cout << "ID : " << Num << endl;
							cout << "OF Set : " << ofset << endl;

							cout << "read " << inRecord.Read(SearchStream) << endl;

							cout << "unpack " << customer.UnPack(inRecord) << endl;

							customer.Print(customer);
						}

						SearchStream.close();

						cout << "Are you want Sreach to another recorde !?" << endl;
						cin >> t;

						cout << "******************************************************** \n";
					}
				}// end Write  

				else if (p == 4) {
				char t = 't';
				while (t != 'n') {
					Customer  customer;
					fstream DataFile;
					fstream indexFile;

					int id;
					cout << "Enter ID to update : " << endl;
					cin >> id;
					int offSet = 0;

					indexFile.open("Files//customerIndex.txt", ios::binary | ios::in | ios::out);
					DataFile.open("Files//customerData.txt", ios::binary | ios::in | ios::out);

					int newSize = 0;
					int oldSize = 0;
					DataFile.seekg(0, ios::beg);
					int test = inRecord.SearchID(indexFile, DataFile, id, offSet);

					if (test) {
						cout << "read " << inRecord.Read(DataFile, oldSize) << endl;
						cout << "unpack " << customer.UnPack(inRecord) << endl;
						customer.Print(customer);
						DataFile.close();

						Customer newRecorde;
						cout << "Enter the updated Data of Recorde " << endl;
						cout << "Enter Name : " << endl; 
						cin >> newRecorde.name;
						cout << "Enter Address" << endl;
						cin >> newRecorde.address;
						cout << "Enter Phone : " << endl;
						cin >> newRecorde.phone;
						cout << "Enter recipe : " << endl;
						cin >> newRecorde.recipe;
						cout << "Etner ID : " << endl;
						cin >> newRecorde.id;

						DataFile.open("Files//customerData.txt", ios::binary | ios::in | ios::out);

						//DataFile.seekg(0, ios::beg);
						//inRecord.ReadHeader(DataFile);  
						newSize = 4 + strlen(newRecorde.name) + 1 + 4 + strlen(newRecorde.address) + 1 + strlen(newRecorde.phone) + 1;
						if (oldSize >= newSize) {
							cout << "In First Case " << endl;

							DataFile.seekp(offSet, ios::beg);
							cout << "To APP : " << DataFile.tellp() << endl;
							newRecorde.InitRecord(outRecord);
							newRecorde.Pack(outRecord);
							outRecord.Write(DataFile, newSize);

							outRecord.UpdateRecorde(indexFile, DataFile, newSize, oldSize, customer.id, newRecorde.id);
						}

						else {

							indexFile.close();
							//   Add the end of Flie 
							cout << "In Second Case " << endl;
							DataFile.seekp(0, ios::end);
							cout << "To APP : " << DataFile.tellp() << endl;
							newRecorde.InitRecord(outRecord);
							cout << "pack chef " << newRecorde.Pack(outRecord) << endl;
							outRecord.Write(DataFile, index.ofSet, p);
							DataFile.close();
							index.id = newRecorde.id;

							// Index File
							fstream NewIndex;
							NewIndex.open("Files//customerIndex.txt", ios::binary | ios::out | ios::app);
							NewIndex.write((char*)&index, sizeof(index));
							NewIndex.close();

							// -----------------------------------------------------------------
							indexFile.open("Files//customerIndex.txt", ios::binary | ios::in | ios::out);
							DataFile.open("Files//customerData.txt", ios::binary | ios::in | ios::out);

							outRecord.Delete(DataFile, indexFile, offSet);
							int renum = inRecord.RecordeNumbers(indexFile);
							--id;
							//cout << "postion of deleted record = " << id << endl;

							indexFile.seekg(0, ios::beg);

							for (int i = 0; i < renum; i++) { // read all index 
								indexFile.read((char*)&n[i].id, sizeof(int));
								indexFile.read((char*)&n[i].ofSet, sizeof(int));
							}
							indexFile.close();
							remove("Files//customerIndex.txt");

							indexFile.open("Files//customerIndex.txt", ios::out | ios::in | ios::binary | ios::trunc);

							indexFile.seekp(0, ios::beg);
							for (int i = 0; i < renum; i++) {
								if (i == id) {
									continue;
								}
								else {
									indexFile.write((char*)&n[i], sizeof(n[i]));
									cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
								}
							}
							indexFile.close();
						}

						DataFile.close();
					

					}



					cout << "Are you want Update another recorde !?" << endl;
					cin >> t;

					cout << "************************************ \n";
				}


				}
				
				else if (p == 5) {

				cout << "\t\tIN DELET " << endl;
				Index n[50];
				fstream deletStream;
				fstream deleteIndex;
				int Num = 0;
				int ofset = 0;
				char t = 't';

				int oldSize;
				while (t != 'n')
				{
					deletStream.open("Files//customerData.txt", ios::out | ios::in | ios::binary);

					cout << "Etner the ID of Recorde " << endl; cin >> Num;

					deleteIndex.open("Files//customerIndex.txt", ios::binary | ios::in | ios::out);

					deletStream.seekg(0, ios::beg);
					int test = inRecord.SearchID(deleteIndex, deletStream, Num, ofset);

					if (test) {
						cout << "read " << inRecord.Read(deletStream, oldSize) << endl;
						cout << "unpack " << customer.UnPack(inRecord) << endl;
						customer.Print(customer);
						deletStream.close();// error at the end of file 

						deletStream.open("Files//customerData.txt", ios::out | ios::in | ios::binary);

						outRecord.Delete(deletStream, deleteIndex , ofset);

						deletStream.close();

						/************************delete in index***********************************/
						int renum = inRecord.RecordeNumbers(deleteIndex);
						--Num;
						//cout << "postion of deleted record = " << Num << endl;

						deleteIndex.seekg(0, ios::beg);

						for (int i = 0; i < renum; i++) { // read all index 
							deleteIndex.read((char*)&n[i].id, sizeof(int));
							deleteIndex.read((char*)&n[i].ofSet, sizeof(int));
							
						}
						deleteIndex.close();
						remove("Files//customerIndex.txt");

						deleteIndex.open("Files//customerIndex.txt", ios::out | ios::in | ios::binary | ios::trunc);

						deleteIndex.seekp(0, ios::beg);
						for (int i = 0; i < renum; i++) {
							if (i == Num) {
								continue;
							}
							else {
								deleteIndex.write((char*)&n[i], sizeof(n[i]));
								cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
							}
						}
						deleteIndex.close();
					}


					cout << "Are you want Delete to another recorde !?" << endl;
					cin >> t;

				}
				}
				
				

				cout << "1  - Write   \n";
				cout << "11 - Write in Avail List    \n";
				cout << "2  - Read    \n";
				cout << "3  - Search  \n";
				cout << "4  - Update  \n";
				cout << "5  - Delete  \n";
				cout << "0  - End     \n";
				cin >> p;

			}// end while


		} // End customer  

		else if (data_File == 4) { // waiter 

		cout << "1  - Write   \n";
		cout << "11 - Write in Avail List    \n";
		cout << "2  - Read    \n";
		cout << "3  - Search  \n";
		cout << "4  - Update  \n";
		cout << "5  - Delete  \n";
		cout << "0  - End     \n";

			int p = -1 ;
			cin >> p;


			while (p != 0)
			{
				if (p == 1) {

					// Write 
					fstream TestOut;
					fstream NewIndex;
					char n = 'r';

					while (n != 'n') {
						TestOut.open("Files//waiterData.txt", ios::out | ios::binary | ios::app);
						TestOut.seekp(0, ios::end);

						cout << "Enter Name : " << endl;
						cin >> waiter1.name;
						cout << "Etner Phone : " << endl;
						cin >> waiter1.phone;
						cout << "Etner salary : " << endl;
						cin >> waiter1.salary;
						cout << "Etner ID : " << endl;
						cin >> waiter1.id;

						waiter1.InitRecord(outRecord);
						outRecord.WriteHeader(TestOut);  // Only Once. 
						cout << "pack chef " << waiter1.Pack(outRecord) << endl;
						outRecord.Write(TestOut, index.ofSet, p);
						TestOut.close();

						// Index File
						index.id = waiter1.id;
						NewIndex.open("Files//waiterIndex.txt", ios::binary | ios::out | ios::app);
						NewIndex.write((char*)&index, sizeof(index));
						NewIndex.close();

						cout << "Are you want Continue !?" << endl;
						cin >> n;

						cout << "************************************ \n";
					}



				}

				else if (p == 11) {

						// Write 
						fstream TestOut;
						fstream NewIndex;
						char n = 'r';

						while (n != 'n') {
							cout << "\tWrite in Available List: " << endl;
							cout << "Enter Name : " << endl;
							cin >> waiter1.name;
							cout << "Etner Phone : " << endl;
							cin >> waiter1.phone;
							cout << "Etner salary : " << endl;
							cin >> waiter1.salary;
							cout << "Etner ID : " << endl;
							cin >> waiter1.id;

							TestOut.open("Files//waiterData.txt", ios::binary | ios::out | ios::in);

							waiter1.InitRecord(outRecord);
							outRecord.WriteHeader(TestOut);  // Only Once. 
							cout << "pack chef " << waiter1.Pack(outRecord) << endl;
							outRecord.WriteinAvailableList(TestOut, index.ofSet);
							TestOut.close();

							// Index File
							index.id = waiter1.id;
							NewIndex.open("Files//waiterIndex.txt", ios::binary | ios::out | ios::app);
							NewIndex.write((char*)&index, sizeof(index));
							NewIndex.close();

							cout << "Are you want Continue !?" << endl;
							cin >> n;

							cout << "************************************ \n";
						}



				}

				else if (p == 2) {
					// Read 

					cout << "\t\tRead From File " << endl;

					fstream TestIn("Files//waiterData.txt", ios::in | ios::binary);
					TestIn.seekg(0, ios::beg);
					inRecord.ReadHeader(TestIn);

					fstream indexFile;
					indexFile.open("Files//waiterIndex.txt", ios::binary | ios::in);

					int num = inRecord.RecordeNumbers(indexFile);

					int idOffSet;
					indexFile.seekg(0, ios::beg);
					for (int i = 0; i < num; i++) {
						indexFile.seekg(4, ios::cur); // skip id 
						indexFile.read((char*)&idOffSet, sizeof(int));
						cout << "OFSet : " << idOffSet << endl;
						TestIn.seekg(idOffSet, ios::beg);
						cout << "read " << inRecord.Read(TestIn) << endl;
						int t = waiter1.UnPack(inRecord);
						cout << "unpack " << t << endl;
						waiter1.Print(waiter1);

					}

					TestIn.close();

					cout << "************************************ \n";

				}// end read  
				 
				else  if (p == 3) {
					// Search 
					cout << "\t\tIN Search " << endl;

					fstream SearchStream;
					fstream readIndex;

					int Num = 0;
					int ofset = 0;
					char t = 't';
					while (t != 'n')
					{
						SearchStream.open("Files//waiterData.txt", ios::in | ios::binary);

						cout << "Etner the ID of Recorde " << endl; cin >> Num;

						fstream NewIndex;
						readIndex.open("Files//waiterIndex.txt", ios::binary | ios::in);
						int searchid = inRecord.SearchID(readIndex, SearchStream, Num, ofset);
						readIndex.close();
						if (searchid) {
							cout << "ID : " << Num << endl;
							cout << "OF Set : " << ofset << endl;

							cout << "read " << inRecord.Read(SearchStream) << endl;

							cout << "unpack " << waiter1.UnPack(inRecord) << endl;

							waiter1.Print(waiter1);
						}

						SearchStream.close();

						cout << "Are you want Sreach to another recorde !?" << endl;
						cin >> t;

						cout << "************************************ \n";
					}
				}// end Write   
				 
				else if (p == 4) {
				char t = 't';
				while (t != 'n') {
					waiter  waiter2;
					fstream DataFile;
					fstream indexFile;

					int id;
					cout << "Enter ID to update : " << endl;
					cin >> id;
					int offSet = 0;

					indexFile.open("Files//waiterIndex.txt", ios::binary | ios::in | ios::out);
					DataFile.open("Files//waiterData.txt", ios::binary | ios::in | ios::out);

					int newSize = 0;
					int oldSize = 0;
					DataFile.seekg(0, ios::beg);
					int test = inRecord.SearchID(indexFile, DataFile, id, offSet);

					if (test) {
						cout << "read " << inRecord.Read(DataFile, oldSize) << endl;
						cout << "unpack " << waiter2.UnPack(inRecord) << endl;
						waiter2.Print(waiter2);
						DataFile.close();

						waiter newRecorde;
						cout << "Enter the updated Data of Recorde " << endl;
						cout << "Enter Name : " << endl;
						cin >> newRecorde.name;
						cout << "Etner Phone : " << endl;
						cin >> newRecorde.phone;
						cout << "Etner salary : " << endl;
						cin >> newRecorde.salary;
						cout << "Etner ID : " << endl;
						cin >> newRecorde.id;

						DataFile.open("Files//waiterData.txt", ios::binary | ios::in | ios::out);

						//DataFile.seekg(0, ios::beg);
						//inRecord.ReadHeader(DataFile);  
						newSize = 4 + strlen(newRecorde.name) + 1 + 4 + strlen(newRecorde.phone) + 1;

						if (oldSize >= newSize) {
							cout << "In First Case " << endl;
							DataFile.seekp(offSet, ios::beg);

							cout << "To APP : " << DataFile.tellp() << endl;
							newRecorde.InitRecord(outRecord);
							newRecorde.Pack(outRecord);
							outRecord.Write(DataFile, newSize);

							outRecord.UpdateRecorde(indexFile, DataFile, newSize, oldSize, waiter2.id, newRecorde.id);
						}

						else {

							indexFile.close();
							//   Add the end of Flie 
							cout << "In Second Case " << endl;
							DataFile.seekp(0, ios::end);
							cout << "To APP : " << DataFile.tellp() << endl;
							newRecorde.InitRecord(outRecord);
							cout << "pack chef " << newRecorde.Pack(outRecord) << endl;
							outRecord.Write(DataFile, index.ofSet, p);
							DataFile.close();
							index.id = newRecorde.id;

							// Index File
							fstream NewIndex;
							NewIndex.open("Files//waiterIndex.txt", ios::binary | ios::out | ios::app);
							NewIndex.write((char*)&index, sizeof(index));
							NewIndex.close();

							// -----------------------------------------------------------------
							indexFile.open("Files//waiterIndex.txt", ios::binary | ios::in | ios::out);
							DataFile.open("Files//waiterData.txt", ios::binary | ios::in | ios::out);

							outRecord.Delete(DataFile, indexFile, offSet);
							int renum = inRecord.RecordeNumbers(indexFile);
							--id;
							//cout << "postion of deleted record = " << id << endl;

							indexFile.seekg(0, ios::beg);

							for (int i = 0; i < renum; i++) { // read all index 
								indexFile.read((char*)&n[i].id, sizeof(int));
								indexFile.read((char*)&n[i].ofSet, sizeof(int));
							
							}
							indexFile.close();
							remove("Files//waiterIndex.txt");

							indexFile.open("Files//waiterIndex.txt", ios::out | ios::in | ios::binary | ios::trunc);

							indexFile.seekp(0, ios::beg);
							for (int i = 0; i < renum; i++) {
								if (i == id) {
									continue;
								}
								else {
									indexFile.write((char*)&n[i], sizeof(n[i]));
									cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
								}
							}
							indexFile.close();
						}
						
						DataFile.close();

					}



					cout << "Are you want Update another recorde !?" << endl;
					cin >> t;

					cout << "************************************ \n";
				}


				}  

				else if (p == 5) {

				cout << "\t\tIN DELET " << endl;
				Index n[50];
				fstream deletStream;
				fstream deleteIndex;
				int Num = 0;
				int ofset = 0;
				char t = 't';

				int oldSize;
				while (t != 'n')
				{
					deletStream.open("Files//waiterData.txt", ios::out | ios::in | ios::binary);

					cout << "Etner the ID of Recorde " << endl; cin >> Num;

					deleteIndex.open("Files//waiterIndex.txt", ios::binary | ios::in | ios::out);

					deletStream.seekg(0, ios::beg);
					int test = inRecord.SearchID(deleteIndex, deletStream, Num, ofset);

					if (test) {
						cout << "read " << inRecord.Read(deletStream, oldSize) << endl;
						cout << "unpack " << waiter1.UnPack(inRecord) << endl;
						waiter1.Print(waiter1);
						deletStream.close();// error at the end of file 

						deletStream.open("Files//waiterData.txt", ios::out | ios::in | ios::binary);

						outRecord.Delete(deletStream, deleteIndex , ofset);

						deletStream.close();

						/************************delete in index***********************************/
						int renum = inRecord.RecordeNumbers(deleteIndex);
						--Num;
						//cout << "postion of deleted record = " << Num << endl;

						deleteIndex.seekg(0, ios::beg);

						for (int i = 0; i < renum; i++) { // read all index 
							deleteIndex.read((char*)&n[i].id, sizeof(int));
							deleteIndex.read((char*)&n[i].ofSet, sizeof(int));
							
						}
						deleteIndex.close();
						remove("Files//waiterIndex.txt");

						deleteIndex.open("Files//waiterIndex.txt", ios::out | ios::in | ios::binary | ios::trunc);

						deleteIndex.seekp(0, ios::beg);
						for (int i = 0; i < renum; i++) {
							if (i == Num) {
								continue;
							}
							else {
								deleteIndex.write((char*)&n[i], sizeof(n[i]));
								cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
							}
						}
						deleteIndex.close();
					}


					cout << "Are you want Delete to another recorde !?" << endl;
					cin >> t;

				}
				} 

				cout << "1  - Write   \n";
				cout << "11 - Write in Avail List    \n";
				cout << "2  - Read    \n";
				cout << "3  - Search  \n";
				cout << "4  - Update  \n";
				cout << "5  - Delete  \n";
				cout << "0  - End     \n";
				cin >> p;

			}// end while

		} // End waiter  

		else if (data_File == 5) { // meal 

		cout << "1  - Write   \n";
		cout << "11 - Write in Avail List    \n";
		cout << "2  - Read    \n";
		cout << "3  - Search  \n";
		cout << "4  - Update  \n";
		cout << "5  - Delete  \n";
		cout << "0  - End     \n";


			int p = -1;
			cin >> p;

			while (p != 0)
			{
				if (p == 1) {

					// Write 
					fstream TestOut; 
					fstream NewIndex;
					char n = 'r';

					while (n != 'n') {
						TestOut.open("Files//mealData.txt", ios::out | ios::binary | ios::app);
						TestOut.seekp(0, ios::end);

						cout << "Enter Name : " << endl;
						cin >> meal1.name;
						cout << "Enter Type : " << endl;
						cin >> meal1.type;
						cout << "Etner Price : " << endl;
						cin >> meal1.price;
						cout << "Etner ID : " << endl;
						cin >> meal1.id;

						meal1.InitRecord(outRecord);
						outRecord.WriteHeader(TestOut);  // Only Once. 
						cout << "pack chef " << meal1.Pack(outRecord) << endl;
						outRecord.Write(TestOut, index.ofSet, p);
						TestOut.close();

						// Index File
						index.id = meal1.id;
						NewIndex.open("Files//mealIndex.txt", ios::binary | ios::out | ios::app);
						NewIndex.write((char*)&index, sizeof(index));
						NewIndex.close();

						cout << "Are you want Continue !?" << endl;
						cin >> n;

						cout << "************************************ \n";
					}

				} 

				else if (p == 11) {

					// Write 
					fstream TestOut;
					fstream NewIndex;
					char n = 'r';

					while (n != 'n') {

						cout << "Enter Name : " << endl;
						cin >> meal1.name;
						cout << "Enter Type : " << endl;
						cin >> meal1.type;
						cout << "Etner Price : " << endl;
						cin >> meal1.price;
						cout << "Etner ID : " << endl;
						cin >> meal1.id;

						TestOut.open("Files//mealData.txt", ios::binary | ios::out | ios::in);

						meal1.InitRecord(outRecord);
						outRecord.WriteHeader(TestOut);  // Only Once. 
						cout << "pack chef " << meal1.Pack(outRecord) << endl;
						outRecord.WriteinAvailableList(TestOut, index.ofSet);
						TestOut.close();

						// Index File
						index.id = meal1.id;
						NewIndex.open("Files//mealIndex.txt", ios::binary | ios::out | ios::app);
						NewIndex.write((char*)&index, sizeof(index));
						NewIndex.close();

						cout << "Are you want Continue !?" << endl;
						cin >> n;

						cout << "************************************ \n";
					}

				}

				else if (p == 2) {
					// Read 

					cout << "\t\tRead From File " << endl;

					fstream TestIn("Files//mealData.txt", ios::in | ios::binary);
					TestIn.seekg(0, ios::beg);
					inRecord.ReadHeader(TestIn);

					fstream indexFile;
					indexFile.open("Files//mealIndex.txt", ios::binary | ios::in);

					int num = inRecord.RecordeNumbers(indexFile); 

					int idOffSet;
					indexFile.seekg(0, ios::beg);
					for (int i = 0; i < num; i++) {
						indexFile.seekg(4, ios::cur); // skip id 
						indexFile.read((char*)&idOffSet, sizeof(int));
						cout << "OFSet : " << idOffSet << endl;
						TestIn.seekg(idOffSet, ios::beg);
						cout << "read " << inRecord.Read(TestIn) << endl;
						int t = meal1.UnPack(inRecord);
						cout << "unpack " << t << endl;
						meal1.Print(meal1);

					}

					TestIn.close();

					cout << "************************************ \n";

				}// end read 

				else  if (p == 3) {
					// Search 
					cout << "\t\tIN Search " << endl;

					fstream SearchStream;
					fstream readIndex;

					int Num = 0;
					int ofset = 0;
					char t = 't';
					while (t != 'n')
					{
						SearchStream.open("Files//mealData.txt", ios::in | ios::binary);

						cout << "Etner the ID of Recorde " << endl; cin >> Num;

						fstream NewIndex;
						readIndex.open("Files//mealIndex.txt", ios::binary | ios::in);
						int searchid = inRecord.SearchID(readIndex, SearchStream, Num, ofset);
						readIndex.close();
						if (searchid) {
							cout << "ID : " << Num << endl;
							cout << "OF Set : " << ofset << endl;

							cout << "read " << inRecord.Read(SearchStream) << endl;

							cout << "unpack " << meal1.UnPack(inRecord) << endl;

							meal1.Print(meal1);
						}

						SearchStream.close();

						cout << "Are you want Sreach to another recorde !?" << endl;
						cin >> t;

						cout << "************************************ \n";
					}
				}// end Write  


				else if (p == 4) {
				char t = 't';
				while (t != 'n') {
					meal  meal2;
					fstream DataFile;
					fstream indexFile;

					int id;
					cout << "Enter ID to update : " << endl;
					cin >> id;
					int offSet = 0;

					indexFile.open("Files//mealIndex.txt", ios::binary | ios::in | ios::out);
					DataFile.open("Files//mealData.txt", ios::binary | ios::in | ios::out);

					int newSize = 0;
					int oldSize = 0;
					DataFile.seekg(0, ios::beg);
					int test = inRecord.SearchID(indexFile, DataFile, id, offSet);

					if (test) {
						cout << "read " << inRecord.Read(DataFile, oldSize) << endl;
						cout << "unpack " << meal2.UnPack(inRecord) << endl;
						meal2.Print(meal2);
						DataFile.close();

						meal newRecorde;
						cout << "Enter the updated Data of Recorde " << endl;
						cout << "Enter Name : " << endl;
						cin >> newRecorde.name;
						cout << "Enter Type : " << endl;
						cin >> newRecorde.type;
						cout << "Etner Price : " << endl;
						cin >> newRecorde.price;
						cout << "Etner ID : " << endl;
						cin >> newRecorde.id;

						DataFile.open("Files//mealData.txt", ios::binary | ios::in | ios::out);

						//DataFile.seekg(0, ios::beg);
						//inRecord.ReadHeader(DataFile);  

						newSize = 4 + strlen(newRecorde.name) + 1 + 4 + strlen(newRecorde.type) + 1;

						if (oldSize >= newSize) {

							DataFile.seekp(offSet, ios::beg);

							cout << "To APP : " << DataFile.tellp() << endl;
							newRecorde.InitRecord(outRecord);
							newRecorde.Pack(outRecord);
							outRecord.Write(DataFile, newSize);

							outRecord.UpdateRecorde(indexFile, DataFile, newSize, oldSize, meal2.id, newRecorde.id);
						}

						else {

							indexFile.close();
							//   Add the end of Flie 
							cout << "In Second Case " << endl;
							DataFile.seekp(0, ios::end);
							cout << "To APP : " << DataFile.tellp() << endl;
							newRecorde.InitRecord(outRecord);
							cout << "pack chef " << newRecorde.Pack(outRecord) << endl;
							outRecord.Write(DataFile, index.ofSet, p);
							DataFile.close();
							index.id = newRecorde.id;

							// Index File
							fstream NewIndex;
							NewIndex.open("Files//mealIndex.txt", ios::binary | ios::out | ios::app);
							NewIndex.write((char*)&index, sizeof(index));
							NewIndex.close();

							// -----------------------------------------------------------------
							indexFile.open("Files//mealIndex.txt", ios::binary | ios::in | ios::out);
							DataFile.open("Files//mealData.txt", ios::binary | ios::in | ios::out);

							outRecord.Delete(DataFile, indexFile, offSet);
							int renum = inRecord.RecordeNumbers(indexFile);
							--id;
							//cout << "postion of deleted record = " << id << endl;

							indexFile.seekg(0, ios::beg);

							for (int i = 0; i < renum; i++) { // read all index 
								indexFile.read((char*)&n[i].id, sizeof(int));
								indexFile.read((char*)&n[i].ofSet, sizeof(int));
								
							}
							indexFile.close();
							remove("Files//mealIndex.txt");

							indexFile.open("Files//mealIndex.txt", ios::out | ios::in | ios::binary | ios::trunc);

							indexFile.seekp(0, ios::beg);
							for (int i = 0; i < renum; i++) {
								if (i == id) {
									continue;
								}
								else {
									indexFile.write((char*)&n[i], sizeof(n[i]));
									cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
								}
							}
							indexFile.close();
						}
						

						DataFile.close();
						

					}



					cout << "Are you want Update another recorde !?" << endl;
					cin >> t;

					cout << "************************************ \n";
				}


				}

				else if (p == 5) {

				cout << "\t\tIN DELET " << endl;
				Index n[50];
				fstream deletStream;
				fstream deleteIndex;
				int Num = 0;
				int ofset = 0;
				char t = 't';

				int oldSize;
				while (t != 'n')
				{
					deletStream.open("Files//mealData.txt", ios::out | ios::in | ios::binary);

					cout << "Etner the ID of Recorde " << endl; cin >> Num;

					deleteIndex.open("Files//mealIndex.txt", ios::binary | ios::in | ios::out);

					deletStream.seekg(0, ios::beg);
					int test = inRecord.SearchID(deleteIndex, deletStream, Num, ofset);

					if (test) {
						cout << "read " << inRecord.Read(deletStream, oldSize) << endl;
						cout << "unpack " << meal1.UnPack(inRecord) << endl;
						meal1.Print(meal1);
						deletStream.close();// error at the end of file 

						deletStream.open("Files//mealData.txt", ios::out | ios::in | ios::binary);

						outRecord.Delete(deletStream, deleteIndex , ofset);

						deletStream.close();

						/************************delete in index***********************************/
						int renum = inRecord.RecordeNumbers(deleteIndex);
						--Num;
						//cout << "postion of deleted record = " << Num << endl;

						deleteIndex.seekg(0, ios::beg);

						for (int i = 0; i < renum; i++) { // read all index 
							deleteIndex.read((char*)&n[i].id, sizeof(int));
							deleteIndex.read((char*)&n[i].ofSet, sizeof(int));
							
						}
						deleteIndex.close();
						remove("Files//mealIndex.txt");

						deleteIndex.open("Files//mealIndex.txt", ios::out | ios::in | ios::binary | ios::trunc);

						deleteIndex.seekp(0, ios::beg);
						for (int i = 0; i < renum; i++) {
							if (i == Num) {
								continue;
							}
							else {
								deleteIndex.write((char*)&n[i], sizeof(n[i]));
								cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
							}
						}
						deleteIndex.close();
					}


					cout << "Are you want Delete to another recorde !?" << endl;
					cin >> t;

				}
				}


				cout << "1  - Write   \n";
				cout << "11 - Write in Avail List    \n";
				cout << "2  - Read    \n";
				cout << "3  - Search  \n";
				cout << "4  - Update  \n";
				cout << "5  - Delete  \n";
				cout << "0  - End     \n";
				cin >> p;

			}// end while

		} // End Meal 

		else if (data_File == 6) { // Order
		cout << "1  - Write   \n";
		cout << "11 - Write in Avail List    \n";
		cout << "2  - Read    \n";
		cout << "3  - Search  \n";
		cout << "4  - Update  \n";
		cout << "5  - Delete  \n";
		cout << "0  - End     \n";


			int p = -1;
			cin >> p;


			while (p != 0)
			{
				if (p == 1) {

					// Write 
					fstream TestOut; 
					fstream NewIndex;
					char n = 'r';

					while (n != 'n') {
						TestOut.open("Files//orderData.txt", ios::out | ios::binary | ios::app);
						TestOut.seekp(0, ios::end);

						cout << "Enter Order Name : " << endl;
						cin >> order1.ordername;
						cout << "Enter Price : " << endl;
						cin >> order1.price;
						cout << "Etner Count : " << endl;
						cin >> order1.count;
						cout << "Etner ID : " << endl;
						cin >> order1.id;

						order1.InitRecord(outRecord);
						outRecord.WriteHeader(TestOut);  // Only Once.    
						cout << "pack chef " << order1.Pack(outRecord) << endl;
						outRecord.Write(TestOut, index.ofSet, p);
						TestOut.close();

						// Index File
						index.id = order1.id;
						NewIndex.open("Files//orderIndex.txt", ios::binary | ios::out | ios::app);
						NewIndex.write((char*)&index, sizeof(index));
						NewIndex.close();

						cout << "Are you want Continue !?" << endl;
						cin >> n;

						cout << "************************************ \n";
					}
				}

				else if (p == 11) {

					// Write 
					fstream TestOut;
					fstream NewIndex;
					char n = 'r';

					while (n != 'n') {

						cout << "Enter Order Name : " << endl;
						cin >> order1.ordername;
						cout << "Enter Price : " << endl;
						cin >> order1.price;
						cout << "Etner Count : " << endl;
						cin >> order1.count;
						cout << "Etner ID : " << endl;
						cin >> order1.id;

						TestOut.open("Files//orderData.txt", ios::binary | ios::out | ios::in);

						order1.InitRecord(outRecord);
						outRecord.WriteHeader(TestOut);  // Only Once.    
						cout << "pack chef " << order1.Pack(outRecord) << endl;
						outRecord.WriteinAvailableList(TestOut, index.ofSet);
						TestOut.close();

						// Index File
						index.id = order1.id;
						NewIndex.open("Files//orderIndex.txt", ios::binary | ios::out | ios::app);
						NewIndex.write((char*)&index, sizeof(index));
						NewIndex.close();

						cout << "Are you want Continue !?" << endl;
						cin >> n;

						cout << "************************************ \n";
					}
				}

				else if (p == 2) {
					// Read 

					cout << "\t\tRead From File " << endl;

					fstream TestIn("Files//orderData.txt", ios::in | ios::binary);
					TestIn.seekg(0, ios::beg);
					inRecord.ReadHeader(TestIn);

					fstream indexFile;
					indexFile.open("Files//orderIndex.txt", ios::binary | ios::in);

					int num = inRecord.RecordeNumbers(indexFile);

					cout << "The Number of Recodes : " << num << endl;
					int idOffSet;
					indexFile.seekg(0, ios::beg);
					for (int i = 0; i < num; i++) {
						indexFile.seekg(4, ios::cur); // skip id 
						indexFile.read((char*)&idOffSet, sizeof(int));
						cout << "OFSet : " << idOffSet << endl;
						TestIn.seekg(idOffSet, ios::beg);
						cout << "read " << inRecord.Read(TestIn) << endl;
						int t = order1.Unpack(inRecord);
						cout << "unpack " << t << endl;
						order1.Print(order1);

					}

					TestIn.close();

					cout << "************************************ \n";

				}// end read 

				else  if (p == 3) {
					// Search 
					cout << "\t\tIN Search " << endl;

					fstream SearchStream;
					fstream readIndex;

					int Num = 0;
					int ofset = 0;
					char t = 't';
					while (t != 'n')
					{
						SearchStream.open("Files//orderData.txt", ios::in | ios::binary);

						cout << "Etner the ID of Recorde " << endl; cin >> Num;

						fstream NewIndex;
						readIndex.open("Files//orderIndex.txt", ios::binary | ios::in);
						int searchid = inRecord.SearchID(readIndex, SearchStream, Num, ofset);
						readIndex.close();
						if (searchid) {
							cout << "ID : " << Num << endl;
							cout << "OF Set : " << ofset << endl;

							cout << "read " << inRecord.Read(SearchStream) << endl;

							cout << "unpack " << order1.Unpack(inRecord) << endl;

							order1.Print(order1);
						}

						SearchStream.close();

						cout << "Are you want Sreach to another recorde !?" << endl;
						cin >> t;

						cout << "************************************ \n";
					}
				}// end Write   

				else if (p == 4) {
				char t = 't';
				while (t != 'n') {
					Order  order2;
					fstream DataFile;
					fstream indexFile;
					Index n[50];
					int id;
					cout << "Enter ID to update : " << endl;
					cin >> id;
					int offSet = 0;

					indexFile.open("Files//orderIndex.txt", ios::binary | ios::in | ios::out);
					DataFile.open("Files//orderData.txt", ios::binary | ios::in | ios::out);

					int newSize = 0;
					int oldSize = 0;
					DataFile.seekg(0, ios::beg);
					int test = inRecord.SearchID(indexFile, DataFile, id, offSet);

					if (test) {
						cout << "read " << inRecord.Read(DataFile, oldSize) << endl;
						cout << "unpack " << order2.Unpack(inRecord) << endl;
						order2.Print(order2);
						DataFile.close();

						Order newRecorde;
						cout << "Enter the updated Data of Recorde " << endl;
						cout << "Enter Order Name : " << endl;
						cin >> newRecorde.ordername;
						cout << "Enter Price : " << endl;
						cin >> newRecorde.price;
						cout << "Etner Count : " << endl;
						cin >> newRecorde.count;
						cout << "Etner ID : " << endl;
						cin >> newRecorde.id;

						DataFile.open("Files//orderData.txt", ios::binary | ios::in | ios::out);

						//DataFile.seekg(0, ios::beg);
						//inRecord.ReadHeader(DataFile);  
						newSize = 4 + 4 + 2 + strlen(newRecorde.ordername) + 1;

						 if (oldSize >= newSize) {
							 cout << "In First Case " << endl;
							DataFile.seekp(offSet, ios::beg);

							cout << "To APP : " << DataFile.tellp() << endl;
							newRecorde.InitRecord(outRecord);
							newRecorde.Pack(outRecord);
							outRecord.Write(DataFile, newSize);

							outRecord.UpdateRecorde(indexFile, DataFile, newSize, oldSize, order2.id, newRecorde.id);
						 }
						 else {

							 indexFile.close();
							 //   Add the end of Flie 
							 cout << "In Second Case " << endl;
							 DataFile.seekp(0, ios::end);
							 cout << "To APP : " << DataFile.tellp() << endl;
							 newRecorde.InitRecord(outRecord);
							 cout << "pack chef " << newRecorde.Pack(outRecord) << endl;
							 outRecord.Write(DataFile, index.ofSet, p);
							 DataFile.close();
							 index.id = newRecorde.id;

							 // Index File
							 fstream NewIndex;
							 NewIndex.open("Files//orderIndex.txt", ios::binary | ios::out | ios::app);
							 NewIndex.write((char*)&index, sizeof(index));
							 NewIndex.close();

							 // -----------------------------------------------------------------
							 indexFile.open("Files//orderIndex.txt", ios::binary | ios::in | ios::out);
							 DataFile.open("Files//orderData.txt", ios::binary | ios::in | ios::out);

							 outRecord.Delete(DataFile, indexFile, offSet);
							 int renum = inRecord.RecordeNumbers(indexFile);
							 --id;
							// cout << "postion of deleted record = " << id << endl;

							 indexFile.seekg(0, ios::beg);

							 for (int i = 0; i < renum; i++) { // read all index 
								 indexFile.read((char*)&n[i].id, sizeof(int));
								 indexFile.read((char*)&n[i].ofSet, sizeof(int));
								 
							 }
							 indexFile.close();
							 remove("Files//orderIndex.txt");

							 indexFile.open("Files//orderIndex.txt", ios::out | ios::in | ios::binary | ios::trunc);

							 indexFile.seekp(0, ios::beg);
							 for (int i = 0; i < renum; i++) {
								 if (i == id) {
									 continue;
								 }
								 else {
									 indexFile.write((char*)&n[i], sizeof(n[i]));
									 cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
								 }
							 }
							 indexFile.close();
						 }


						DataFile.close();
						

					}



					cout << "Are you want Update another recorde !?" << endl;
					cin >> t;

					cout << "************************************ \n";
				}


				}

				else if (p == 5) {

				cout << "\t\tIN DELETE " << endl;
				Index n[50];
				fstream deletStream;
				fstream deleteIndex;
				int Num = 0;
				int ofset = 0;
				char t = 't';

				int oldSize;
				while (t != 'n')
				{
					deletStream.open("Files//orderData.txt", ios::out | ios::in | ios::binary);

					cout << "Etner the ID of Recorde " << endl; cin >> Num;

					deleteIndex.open("Files//orderIndex.txt", ios::binary | ios::in | ios::out);

					deletStream.seekg(0, ios::beg);
					int test = inRecord.SearchID(deleteIndex, deletStream, Num, ofset);

					if (test) {
						cout << "read " << inRecord.Read(deletStream, oldSize) << endl;
						cout << "unpack " << order1.Unpack(inRecord) << endl;
						order1.Print(order1);
						deletStream.close();// error at the end of file 

						deletStream.open("Files//orderData.txt", ios::out | ios::in | ios::binary);

						outRecord.Delete(deletStream, deleteIndex, ofset);

						deletStream.close();

						/************************delete in index***********************************/
						int renum = inRecord.RecordeNumbers(deleteIndex);
						--Num;
						//cout << "postion of deleted record = " << Num << endl;

						deleteIndex.seekg(0, ios::beg);

						for (int i = 0; i < renum; i++) { // read all index 
							deleteIndex.read((char*)&n[i].id, sizeof(int));
							deleteIndex.read((char*)&n[i].ofSet, sizeof(int));
							
						}
						deleteIndex.close();
						remove("Files//orderIndex.txt");

						deleteIndex.open("Files//orderIndex.txt", ios::out | ios::in | ios::binary | ios::trunc);

						deleteIndex.seekp(0, ios::beg);
						for (int i = 0; i < renum; i++) {
							if (i == Num) {
								continue;
							}
							else {
								deleteIndex.write((char*)&n[i], sizeof(n[i]));
								cout << "ID : " << n[i].id << " OFSet : " << n[i].ofSet << endl;
							}
						}
						deleteIndex.close();
					}


					cout << "Are you want Delete to another recorde !?" << endl;
					cin >> t;

				}
				}




				cout << "1  - Write   \n";
				cout << "11 - Write in Avail List    \n";
				cout << "2  - Read    \n";
				cout << "3  - Search  \n";
				cout << "4  - Update  \n";
				cout << "5  - Delete  \n";
				cout << "0  - End     \n";
				cin >> p;

			}// end while


		}// End Order 


		cout << " 1 - Supplier " << endl;
		cout << " 2 - Chef " << endl;
		cout << " 3 - Customer " << endl;
		cout << " 4 - Waiter " << endl;
		cout << " 5 - Meal " << endl;
		cout << " 6 - Order " << endl;
		cout << " 10- Exit " << endl;



		data_File = -1;
		cout << "Choose The File ( from 1 to 6 ) " << endl;
		cin >> data_File;

	}// End Large While  


	return 0; 
}