#pragma once
//#include <stdlib>
#include <iostream>
using namespace std;


struct Field // difine the kind of Structure 
{
	char szFieldRepresentation;
	char length;
	char delimiter;
};

// Abstract class designed to support variablelength records
// Fields may be of a variety of types
class VariableLengthRecord
{
	// props

private:
	Field* m_recordFields;  // array of  fields (dynamic)

	int		m_iRecordSize, m_iNextByte, m_iFieldsCount; // m_iNextByte Index Tracker

	char* pRecord; // Record itself. 

public:

	VariableLengthRecord();

	void init(int iFieldsCount);

	void AddField(int index, char szType, char delimiter);  //Delimited 0 D |
	void AddField(int index, char szType, int length);  // Fixed , LI   1 [F or L] (Length)

	bool WriteHeader(fstream&); // out , put , << , write ( add char in file ) 
	bool ReadHeader(istream&);        // in , get , getline , read , ( read data form file to this var )  

	void Clear(int recordSize = -1); // clear fields from Record

	// this 3   Pack =>  Write
	bool PackFixLen(void* lpData, int dataLength, int fieldLength); // Value,Valuelength,FieldLength
	bool PackDelimeted(void* lpData, int dataLength, char delimiter);
	bool PackLength(void* lpData, short dataLength, char lengthIndicatorSize);


	bool Pack(int, void*, int);

	// this 3   UnPack =>  read
	bool UnpackFixLen(char* lpData, int fieldLength);
	bool UnpackDelimeted(char* lpData, char delimiter, bool bIsText = false);
	bool UnpackLength(char*, char lengthIndicatorSize, bool bIsText = false);


	bool Unpack(int, char*, bool bIsText = false);

	bool Read(fstream&);
	bool Read(fstream&, int&);
	bool Write(fstream&, int&, int&);
	bool WriteinAvailableList(fstream&, int&);
	bool Write(fstream&, int&);

	// for search 
	bool SearchID(fstream&, fstream&, int&, int&);
	int RecordeSize(fstream&, fstream&);
	int RecordeNumbers(fstream&);

	// Update 

	void UpdateRecorde(fstream&, fstream&, int&, int&, int&, int&);
	bool  Delete(fstream&, fstream&, int&);
};