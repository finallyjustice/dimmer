#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>

using namespace std;

#define NUM_MACH 12583
#define NUM_HOUR 8400

int cc = 0;
int cm = 0;
int cu = 0;
int ct = 0;

int main(int argc, char **argv)
{
	for(int i=0; i<NUM_MACH; i++)
	{
		stringstream path;
		path<<"nusage/nusage-"<<i<<".data";

		FILE *fp = fopen(path.str().c_str(), "r");
		if(fp == NULL)
		{
			cout<<"Faild to open file"<<endl;
			return 1;
		}

		int time;
		float cpu;
		float mem;
		float ucache;
		float tcache;

		while(fscanf(fp, "%d %f %f %f %f", &time, &cpu, &mem, &ucache, &tcache) != EOF)
		{
			//cout<<time<<" "<<cpu<<" "<<mem<<" "<<ucache<<" "<<tcache<<endl;

			if(cpu > 1.0f)
				cout<<"CPU: "<<path.str().c_str()<<" "<<time<<" "<<cpu<<endl;
			//if(mem > 1.0f)
			//	cout<<"MEM: "<<path.str().c_str()<<" "<<time<<" "<<mem<<endl;
		}

		fclose(fp);
	}

	return 0;
}
