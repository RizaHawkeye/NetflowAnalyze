#include "Log.h"
#include<fstream>

using std::ofstream;

string Log::PATH = "../log.txt";

Log::Log(string path) :fout(path.c_str())
{
}

void Log::writeLog(const string& msg)
{
	fout<<msg<<std::endl;
	fout.flush();
}

Log::~Log()
{
	fout.close();
}
