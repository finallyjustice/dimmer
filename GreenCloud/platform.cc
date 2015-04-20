#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv)
{
	map<long, string> m;
	ifstream file("platform.time");
	string mid;
	string name;

	while(file >> mid)
	{
		file >> name;
		m.insert(map<long, string>::value_type(atol(mid.c_str()), name));
	}

	map<long, string>::iterator it;
	for(it=m.begin(); it!=m.end(); it++)
		cout<<it->first<<" "<<it->second<<endl;

	file.close();
	return 0;
}
