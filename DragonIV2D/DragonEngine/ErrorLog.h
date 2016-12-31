#pragma once
#include "datatype.h"

class CErrorLog
{
public:
	CErrorLog(void);
	~CErrorLog(void);

	void CreateErrorFile();
	void WriteError(char* string,...);
	void CloseFile();
private:
	FILE* hFile;
};
