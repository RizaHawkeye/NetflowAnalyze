#ifndef _MY_LOG_
#define _MY_LOG_
#include<string>
#include<fstream>
using std::string;
using std::ofstream;

class Log
{
private:
	static string PATH;
	ofstream fout;
public:
	Log(string path = PATH);
	void writeLog(const string& msg);
	~Log();
};
#endif
