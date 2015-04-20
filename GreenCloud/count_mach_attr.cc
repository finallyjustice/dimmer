#include <iostream>
#include <string>
#include <fstream>
#include <set>

using namespace std;

int main(int argc, char **argv)
{
	ifstream file("/var/services/homes/dozhang/data/TraceVersion2/machine_attributes/attrlist.data");
	set<string> s;
	string name;
	while(file >> name)
		s.insert(name);
	file.close();

	set<string>::iterator it;
	for(it=s.begin(); it!=s.end(); it++)
		cout<<*it<<endl;

	return 0;
}
