#ifndef _MY_LOG_
#define _MY_LOG_
#include<string>
using std::string;

class Log
{
private:
	const string PATH;
	ofstream fout;
public:
	Log(string path = PATH);
	void writeLog(const string& msg);
	~Log();
};
#endif
