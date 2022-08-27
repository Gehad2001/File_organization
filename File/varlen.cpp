// varlen.cc
#include<fstream> 
#include"varlen.h" 
#include <string.h>
#include<iostream> 
using namespace std;
//class VariableLengthRecord

// public members
VariableLengthRecord::VariableLengthRecord()
{
	m_recordFields = 0; // array of  fields(dynamic) => Struct 
	m_iFieldsCount = 0; // int 
	m_iNextByte = 0;  // int 
	m_iRecordSize = 0;  // int 

	pRecord = 0;  // char* pRecord; // Record itself.
}

void VariableLengthRecord::init(int iFieldsCount)
{
	m_iFieldsCount = iFieldsCount;
	m_recordFields = new Field[m_iFieldsCount]; // id , name , address 
}

void VariableLengthRecord::AddField(int index, char szType, char delimiter)//D  // index , szType=> ( D ) there , L , F 
{
	m_recordFields[index].szFieldRepresentation = szType; // D
	m_recordFields[index].delimiter = delimiter;
}

void VariableLengthRecord::AddField(int index, char szType, int length)  // L , F 
{
	m_recordFields[index].szFieldRepresentation = szType;
	m_recordFields[index].length = length;
}

// Write Header
bool VariableLengthRecord::WriteHeader(fstream& stream)  // write 
{
	int h = -1;
	if (stream.is_open()) {
		stream.seekg(0, ios::end);
		h = stream.tellg();
		cout << "File is Open " << endl;
	}
	cout << "Header : " << h << endl;
	if (h == 0) {
		short t = -1;
		cout << "Aail list is writen " << endl;
		stream.write((char*)&t, sizeof(short)); // to avail list 

		stream.write((char*)&m_iFieldsCount, 1);

		if (stream.fail())
			return false;

		// m_recordFields IS member in class 

		for (int i = 0; i < m_iFieldsCount; i++) // id name address 
		{
			stream.write(&m_recordFields[i].szFieldRepresentation, 1); // = D , F , L 

			if (stream.fail())
				return false;

			if (m_recordFields[i].szFieldRepresentation == 'F' || m_recordFields[i].szFieldRepresentation == 'L')
			{
				//cout <<"*************************** : "<< & m_recordFields[i].length << endl ; 
				stream.write(&m_recordFields[i].length, 1);
			}

			else if (m_recordFields[i].szFieldRepresentation == 'D')
				stream.write(&m_recordFields[i].delimiter, 1);

			if (stream.fail())
				return false;
		}

		return true;
	}
	else {
		//cout << "Header is Written" << endl;
		return false;
	}


}
// read the header and check for consistency
bool VariableLengthRecord::ReadHeader(istream& stream)
{
	short t;
	stream.read((char*)&t, sizeof(t)); // avail list 

	stream.read((char*)&m_iFieldsCount, 1);
	if (stream.fail())
		return false;

	// m_recordFields -> Only in this function 

	m_recordFields = new Field[m_iFieldsCount]; // Put the number of field 

	for (int i = 0; i < m_iFieldsCount; i++)
	{
		stream.read(&m_recordFields[i].szFieldRepresentation, 1);  // = D , F , L 

		if (stream.fail())
			return false;

		if (m_recordFields[i].szFieldRepresentation == 'F' || m_recordFields[i].szFieldRepresentation == 'L')
			stream.read(&m_recordFields[i].length, 1);

		else if (m_recordFields[i].szFieldRepresentation == 'D')
			stream.read(&m_recordFields[i].delimiter, 1);

		if (stream.fail())
			return false;
	}
	return true;
}


void VariableLengthRecord::Clear(int recordSize /*=-1*/)
{
	m_iNextByte = 0;
	m_iRecordSize = 0;

	if (pRecord != 0)
		delete pRecord;

	if (recordSize != -1)
		pRecord = new char[recordSize];
	else
		pRecord = 0;
}


bool VariableLengthRecord::PackFixLen(void* lpData, int dataLength, int fieldLength) // 4 
{
	// Fill , insrt 
	//   char *pRecode => array 

	memset(pRecord + m_iNextByte, 0, fieldLength);

	if (dataLength > fieldLength)
		dataLength = fieldLength;

	memcpy(&pRecord[m_iNextByte], lpData, dataLength); // 80 , 4 byte

	m_iNextByte += fieldLength;
	m_iRecordSize = m_iNextByte;

	return true;
}

bool VariableLengthRecord::PackDelimeted(void* lpData, int length, char delimiter) // void* lpData => point the address of value => must send the length to know the number of char
{
	memcpy(&pRecord[m_iNextByte], lpData, length); // add Data into Recode based on this length  Ali|Fayoum|
	// memory copy => 
	pRecord[m_iNextByte + length] = delimiter; // add delimeter


	m_iNextByte += length + 1;

	m_iRecordSize = m_iNextByte;

	return true;
}

bool VariableLengthRecord::PackLength(void* lpData, short length, char lengthIndicatorSize)
{
	memcpy(&pRecord[m_iNextByte], &length, lengthIndicatorSize); /*&Length => lenght of data  |  lengthIndicatorSize  */

	memcpy(&pRecord[m_iNextByte + lengthIndicatorSize], lpData, length);

	m_iNextByte += lengthIndicatorSize + length;

	m_iRecordSize = m_iNextByte;

	return true;
}

bool VariableLengthRecord::Pack(int index, void* lpData, int dataLength) // Write 
{
	if (m_recordFields[index].szFieldRepresentation == 'F')
		return PackFixLen(lpData, dataLength, m_recordFields[index].length);

	if (m_recordFields[index].szFieldRepresentation == 'D')
		return PackDelimeted(lpData, dataLength, m_recordFields[index].delimiter);

	return PackLength(lpData, dataLength, m_recordFields[index].length);
}


bool VariableLengthRecord::UnpackFixLen(char* lpData, int fieldLength)
{
	// 
	memcpy(lpData, pRecord + m_iNextByte, fieldLength); // save in data 
	m_iNextByte += fieldLength; // in the second step we read after this data ( id ) 
	return true;
}

bool VariableLengthRecord::UnpackDelimeted(char* lpData, char delimiter, bool bIsText /*= false*/)
{
	int len = -1; // length of unpacked string
	//cout << "UnpackDelimeted : " << m_iNextByte << endl;
	for (int i = m_iNextByte; i < m_iRecordSize; i++)  // pRecord =>               436|Ali|Fayoum|
	{
		if (pRecord[i] == delimiter)
		{
			len = i - m_iNextByte;
			break;
		}
	}

	if (len == -1) {
		cout << "delimeter not found " << endl;
		return false; // delimeter not found
	}


	memcpy(lpData, pRecord + m_iNextByte, len);

	if (bIsText)
		lpData[len] = 0; // zero termination for string 

	m_iNextByte += len + 1;

	return true;
}

bool VariableLengthRecord::UnpackLength(char* lpData, char lengthIndicatorSize, bool bIsText /*= false*/)
{
	short length;

	memcpy(&length, pRecord + m_iNextByte, lengthIndicatorSize);

	memcpy(lpData, pRecord + m_iNextByte + lengthIndicatorSize, length);

	if (bIsText)
		lpData[length] = 0;

	m_iNextByte += length + lengthIndicatorSize;

	return true;
}

bool VariableLengthRecord::Unpack(int index, char* lpData, bool bIsText /*= false*/)  // Read
{
	if (m_recordFields[index].szFieldRepresentation == 'F')
		return UnpackFixLen(lpData, m_recordFields[index].length);

	if (m_recordFields[index].szFieldRepresentation == 'D')
		return UnpackDelimeted(lpData, m_recordFields[index].delimiter, bIsText);

	return UnpackLength(lpData, m_recordFields[index].length, bIsText);
}

bool VariableLengthRecord::Write(fstream& stream, int& offSet, int& temp)
{

	stream.seekp(0, ios::end);
	cout << "Inside Write => ofSet : " << stream.tellp() << endl;
	offSet = stream.tellp();
	stream.write((char*)&m_iRecordSize, sizeof(m_iRecordSize)); // 4 Byte 
	cout << "m_iRecordSize : " << m_iRecordSize << endl;

	if (stream.fail())
		return false;

	cout << "NOW recorde is writen" << endl;
	stream.write(pRecord, m_iRecordSize);

}

bool VariableLengthRecord::WriteinAvailableList(fstream& stream, int& offSet)
{
	short header;
	stream.seekg(0, ios::beg);
	stream.read((char*)&header, sizeof(header));
	cout << "Header of Avail : " << header << endl;

	if (header == -1) { // is Empty 

		stream.seekp(0, ios::end);
		cout << "Inside Write => ofSet : " << stream.tellp() << endl;
		offSet = stream.tellp();
		stream.write((char*)&m_iRecordSize, sizeof(m_iRecordSize)); // 4 Byte 
		cout << "m_iRecordSize : " << m_iRecordSize << endl;

		if (stream.fail())
			return false;

		cout << "NOW recorde is writen" << endl;
		stream.write(pRecord, m_iRecordSize);
	}

	else {
		short next = 0;
		int oldSize;
		short temp;
		stream.seekg(header, ios::beg); // last recorde 
		stream.read((char*)&oldSize, sizeof(int)); // read old size 

		if (oldSize >= m_iRecordSize) {
			int position = stream.tellp();
			stream.seekg(1, ios::cur); // skip * 
			stream.read((char*)&next, sizeof(short)); // to next 

			stream.seekg(0, ios::beg);
			stream.write((char*)&next, sizeof(next)); // update header 


			stream.seekg(position - 4, ios::beg);
			//stream.seekg(-4, ios::cur);
			cout << "Inside Write => ofSet : " << stream.tellp() << endl;
			offSet = stream.tellp();
			stream.write((char*)&m_iRecordSize, sizeof(m_iRecordSize)); // 4 Byte 
			cout << "m_iRecordSize : " << m_iRecordSize << endl;

			if (stream.fail())
				return false;

			cout << "NOW recorde is writen" << endl;
			stream.write(pRecord, m_iRecordSize);


			if (oldSize > m_iRecordSize) {
				int seeknum = 0;
				seeknum = oldSize - m_iRecordSize;
				cout << "Differnt : " << stream.tellp() << endl;
				cout << "seeknum : " << seeknum << endl;
				for (int i = 0; i < seeknum; i++) {
					char t = '!';
					stream.write((char*)&t, 1);
				}

			}


		}

		else {
			while (oldSize < m_iRecordSize)
			{
				temp = stream.tellg();
				//short next;
				stream.seekg(1, ios::cur); // skip * 
				stream.read((char*)&next, sizeof(short)); // to next 
				if (next == -1)
					break;
				stream.seekg(next, ios::beg);
				stream.read((char*)&oldSize, sizeof(int)); // read old size    

			}
			if (next == -1) { // no avail space 
				stream.seekp(0, ios::end);
				cout << "Inside Write => ofSet : " << stream.tellp() << endl;
				offSet = stream.tellp();
				stream.write((char*)&m_iRecordSize, sizeof(m_iRecordSize)); // 4 Byte 
				cout << "m_iRecordSize : " << m_iRecordSize << endl;

				if (stream.fail())
					return false;

				cout << "NOW recorde is writen" << endl;
				stream.write(pRecord, m_iRecordSize);

			}

			else if (oldSize > m_iRecordSize) {
				int position = stream.tellp();
				stream.seekg(1, ios::cur); // skip * 
				stream.read((char*)&next, sizeof(short)); // to next 

				stream.seekg(temp, ios::beg);
				stream.seekg(1, ios::cur); // skip * 
				stream.write((char*)&next, sizeof(next)); // update Next 



				stream.seekg(position - 4, ios::beg);

				//stream.seekg(-4, ios::cur);
				cout << "Inside Write => ofSet : " << stream.tellp() << endl;
				offSet = stream.tellp();
				stream.write((char*)&m_iRecordSize, sizeof(m_iRecordSize)); // 4 Byte 
				cout << "m_iRecordSize : " << m_iRecordSize << endl;

				if (stream.fail())
					return false;

				cout << "NOW recorde is writen" << endl;
				stream.write(pRecord, m_iRecordSize);

				/// for Update space between 
				if (oldSize > m_iRecordSize) {
					int seeknum = 0;
					seeknum = oldSize - m_iRecordSize;
					cout << "Differnt : " << stream.tellp() << endl;
					cout << "seeknum : " << seeknum << endl;
					for (int i = 0; i < seeknum; i++) {
						char t = '!';
						stream.write((char*)&t, 1);
					}

				}


			}
		}




	}


}
bool VariableLengthRecord::Write(fstream& stream, int& newsize)
{
	cout << "Inside Write => ofSet : " << stream.tellp() << endl;

	stream.write((char*)&m_iRecordSize, sizeof(m_iRecordSize)); // 4 Byte 

	cout << "m_iRecordSize : " << m_iRecordSize << endl;

	newsize = m_iRecordSize;

	if (stream.fail())
		return false;

	//cout << "NOW recorde is writen" << endl;
	stream.write(pRecord, m_iRecordSize);

}



int VariableLengthRecord::RecordeSize(fstream& NewIndex, fstream& DataFile) {
	// fstream readOnlyOfSet;
	int index = 0;

	int fileSize = 0;
	if (NewIndex.is_open()) { // in first case False  => fileSize = 0 

		NewIndex.seekp(0, ios::end);
		fileSize = NewIndex.tellg();
	}

	//cout << "File Size : " << fileSize << endl ; 

	if (fileSize == 0) { // File is empty 
		index = 0;
		//cout << "Index in the first case  : " << index << endl; 
		DataFile.seekg(0, ios::beg);
		ReadHeader(DataFile);
		index = DataFile.tellg();
	}

	else {
		NewIndex.seekp(-4, ios::end);
		int m2;
		NewIndex.read((char*)&m2, sizeof(m2)); // read offset of last 
		NewIndex.close();

		//cout << "Last Value From Read  : " << m2 << endl;

		//DataFile.seekg(0, ios::beg); 
		//ReadHeader(DataFile); 

		DataFile.seekg(m2, ios::beg);
		int LastRecode;
		DataFile.read((char*)&LastRecode, sizeof(LastRecode)); // read size of last reacorde 
		int s = 0; /// for ! in update 
		DataFile.seekg(LastRecode, ios::cur);
		char x = '-';

		// for last recorde 
		DataFile.read((char*)&x, 1);
		if (x == '!') {


			while (x == '!')
			{
				++s;
				DataFile.read((char*)&x, 1);
				cout << "to put new index after update  : " << x << endl;
				cout << s << endl;
				if (DataFile.eof())
					break;

			}

			//--s;
		}
		DataFile.seekg(-1, ios::cur);

		//cout << "Last Recode From Mian File" << LastRecode << endl;

		cout << "Last S " << s << endl;
		//index = 4 + LastRecode + m2 + s; 

		index = DataFile.tellg();
		//cout << "Index of the Second Reacode : " << index << endl;
	}



	return index;
}

bool VariableLengthRecord::Read(fstream& stream)
{
	Clear();
	stream.seekg(0, ios::cur);
	int t = stream.tellg();
	//cout << "T in read " << t << endl;
	stream.read((char*)&m_iRecordSize, sizeof(m_iRecordSize)); // sizeof(m_iRecordSize) => int ( 4 Byte) 

	cout << "In Read Method  ( m_iRecordSize ) " << m_iRecordSize << endl;

	if (stream.fail())
		return false;

	pRecord = new char[m_iRecordSize];

	stream.read(pRecord, m_iRecordSize);

	return !stream.fail();
}

bool VariableLengthRecord::Read(fstream& stream, int& reocrdeSize)
{
	Clear();
	stream.seekg(0, ios::cur);
	int t = stream.tellg();
	//cout << "T in read " << t << endl;
	stream.read((char*)&m_iRecordSize, sizeof(m_iRecordSize)); // sizeof(m_iRecordSize) => int ( 4 Byte) 

	reocrdeSize = m_iRecordSize;
	cout << "In Read Method  ( m_iRecordSize ) " << m_iRecordSize << endl;

	if (stream.fail())
		return false;

	pRecord = new char[m_iRecordSize];

	stream.read(pRecord, m_iRecordSize);

	/*
	char x = '-';
	stream.read((char*)&x, 1);
	if (x == '!') {

		while (x == '!')
		{
			if (stream.eof())
				return !stream.fail();
			stream.read((char*)&x, 1);
			cout << "x : " << x << endl;

		}

	}
	stream.seekg(-1, ios::cur);

	*/

	//	cout << "\nIn Read Method : " << pRecord << endl;

	return !stream.fail();
}


bool VariableLengthRecord::SearchID(fstream& NewIndex, fstream& FileStream, int& id, int& ofSet) {

	NewIndex.seekg(0, ios::beg);
	int t = -1;
	int pos = 0;
	int recodeNum = RecordeNumbers(NewIndex);
	NewIndex.seekg(0, ios::beg);

	for (int i = 0; i < recodeNum; i++) {
		pos++;

		NewIndex.read((char*)&t, sizeof(int));
		if (t == id) {
			NewIndex.read((char*)&ofSet, sizeof(int));

			FileStream.seekg(ofSet, ios::beg);

			id = pos;
			//cout << "In File Stream : " << FileStream.tellg() << endl;
			return true;
		}
		NewIndex.seekg(4, ios::cur); // skip ofSet 
	}

	cout << "\t This Recode does not exist !!! " << endl;
	return false;

	//while (!NewIndex.eof()) {
	//	NewIndex.read((char*)&t, sizeof(int));
	//	if (t == id) {
	//		NewIndex.read((char*)&ofSet, sizeof(int));
	//		cout << " \t\t OFSeT : " << ofSet << endl;
	//		break;
	//	}
	//	else
	//		NewIndex.seekg(4, ios::cur); // skip ofSet 

	//}







}


int VariableLengthRecord::RecordeNumbers(fstream& indexFile) {
	int fileSize = 0;
	if (indexFile.is_open()) { // in first case False  => fileSize = 0 

		indexFile.seekp(0, ios::end);
		fileSize = indexFile.tellg();
	}

	int RecordeNum = fileSize / 8;
	return RecordeNum;
}



void  VariableLengthRecord::UpdateRecorde(fstream& indexFile, fstream& dataFile, int& newSize, int& oldSize, int& oldID, int& newID) {
	//cout << "Old : " << oldSize << endl;
	//cout << "New : " << newSize << endl;

	if (oldSize > newSize) {
		int seeknum = 0;
		seeknum = oldSize - newSize;
		//cout << "Differnt : " << dataFile.tellp() << endl;
		//cout << "seeknum : " << seeknum << endl;
		for (int i = 0; i < seeknum; i++) {
			char t = '!';
			dataFile.write((char*)&t, 1);
		}

	}

	VariableLengthRecord  inRecord;
	// update Index File 

	int num = inRecord.RecordeNumbers(indexFile);
	int curID = 0;
	indexFile.seekg(0, ios::beg);
	for (int i = 0; i < num; i++) {
		indexFile.read((char*)&curID, sizeof(int));
		if (curID == oldID) {
			//cout << "-----------Old id : " << oldID << endl;
			indexFile.seekp(-4, ios::cur);
			indexFile.write((char*)&newID, sizeof(int));
		}
		indexFile.seekg(4, ios::cur); // skip ofSet
	}

}


bool VariableLengthRecord::Delete(fstream& FileStream, fstream& NewIndex, int& ofset) {
	short header;

	FileStream.seekg(0, ios::beg);
	FileStream.read((char*)&header, sizeof(header)); // if list is empty 

	if (header == -1) { // this mian it's the first deleted reacorde 
		FileStream.seekg(0, ios::beg);
		FileStream.write((char*)&ofset, sizeof(short)); // update header 

		FileStream.seekg(ofset, ios::beg);
		FileStream.seekg(4, ios::cur);
		FileStream.put('*');  //delete in data file 

		short next = -1;
		FileStream.write((char*)&next, sizeof(next));

		return true;
	}

	else {
		short  temp;
		short next = 0;

		FileStream.seekg(0, ios::beg); // last recorde 
		FileStream.write((char*)&ofset, sizeof(short)); // update header 
		FileStream.seekg(ofset, ios::beg); // 
		FileStream.seekg(4, ios::cur);
		FileStream.put('*');  //delete in data file 

		FileStream.write((char*)&header, sizeof(header));
		return true;
	}

	return false;

}

