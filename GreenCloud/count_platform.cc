#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <set>

using namespace std;

int main(int argc, char **argv)
{
	map<string, int> m;
	set<string> s;

	ifstream file("platform.data");
	string mid;
	string name;
	
	while(file >> mid)
	{
		file >> name;
		if(s.find(name) == s.end())
		{
			m[name] = 1;
			s.insert(name);
		}
		else
		{
			m[name] = m[name]+1;
		}
	}

	file.close();

	map<string, int>::iterator it;
	for(it=m.begin(); it!=m.end(); it++)
		cout<<it->first<<" "<<it->second<<endl;

	return 0;
}
