#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv)
{
	ifstream path("/var/services/homes/dozhang/data/analysis/taskinfo/task_table.data");

	string line;
	while(getline(path, line))	
	{
		stringstream job(line);
		string idx, jid, tid;

		job >> idx;
		job >> jid;
		
		int last = -1;
		while(job >> tid)
		{
			if(atoi(tid.c_str()) != last+1)
			{
				cout<<idx<<" "<<jid<<endl;
				return 1;
			}
			last++;
		}
	}

	path.close();
	return 0;
}
