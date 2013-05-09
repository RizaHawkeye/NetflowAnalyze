#include "Log.h"
#include<fstream>

using std::ofstream;
using std::endl;

const string PATH = "~/code/bishe/log.txt";

Log::Log(string path = PATH) :fout(path)
{ }

void writeLog(const string& msg)
{
	fout<<msg<<endl;
	fout.flush();
}

Log::~Log()
{
	fout.close();
}
