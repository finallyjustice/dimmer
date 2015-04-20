#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace std;

#define NUM_MACH 12583
#define NUM_HOUR 8352

int main(int argc, char **argv)
{
	string stime;
	string scpu;
	string smem;
	string sccache;
	string stcache;

	float tot_cpu;
	float tot_mem;

	for(int i=0; i<NUM_MACH; i++)
	{
		tot_cpu = 0.0f;
		tot_mem = 0.0f;

		stringstream path;
		path<<"/var/services/homes/dozhang/data/analysis/nusage/nusage-"<<i<<".data";
		
		ifstream file(path.str().c_str());
	
		while(file >> stime)
		{
			file >> scpu;
			file >> smem;
			file >> sccache;
			file >> stcache;

			tot_cpu += atof(scpu.c_str());
			tot_mem += atof(smem.c_str());
		}

		file.close();

		if(tot_mem / tot_cpu >= 2.0f)
			cout<<path.str()<<endl;
	}

	return 0;
}
