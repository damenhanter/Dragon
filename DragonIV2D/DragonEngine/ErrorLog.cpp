#include "StdAfx.h"
#include "ErrorLog.h"
#include "datatype.h"



extern HWND hWnd;

CErrorLog::CErrorLog(void)
{
	hFile=NULL;
}

CErrorLog::~CErrorLog(void)
{
}

void CErrorLog::CreateErrorFile()
{
	char filename[]={"errorlog.txt"};
	if (fopen_s(&hFile,filename,"w"))
	{
		MessageBox(hWnd,"打开错误日志文件失败！","",MB_OK);
	}
	_timeb timebuffer;
	char* timeline;
	char timestring[280];
	_ftime_s(&timebuffer);            //获得当前本地时间
	timeline=ctime(&(timebuffer.time));  //把时间值转换成字符串
	wsprintf(timestring,"%.19s.%hu, %s",timeline,timebuffer.millitm,&timeline[20]);
	WriteError("\nOpening Error Output File (%s) on %s\n",filename,timestring);
	fclose(hFile);  //以增加模式重新打开文件
	if (!fopen(filename,"a+"))
	{
		MessageBox(hWnd,"打开错误日志文件失败！","",MB_OK);
	}
}

void CErrorLog::WriteError(char* string,...)
{
	char buf[80];
	va_list arglist;
	if (!string||!hFile)
	{
		MessageBox(hWnd,"写日志失败！","",MB_OK);
	}

	va_start(arglist,string);
	vsprintf_s(buf,string,arglist);
	va_end(arglist);
	
	fprintf(hFile,buf);

	fflush(hFile);
}

void CErrorLog::CloseFile()
{

	if (hFile)
	{
		WriteError("\nClose the errorlog file output.");
		fclose(hFile);
		hFile=NULL;
	}
}