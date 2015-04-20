#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;

#define NUM_MACH 12583
#define NUM_HOUR 8352 

long base = 600000000;
long step = 300000000;

map<long, int> machines;
float mem_cap[NUM_MACH][NUM_HOUR];
float cpu_cap[NUM_MACH][NUM_HOUR];
int mach_cur[NUM_MACH];

void get_machine_list()
{
	FILE *mfp;
	int index;
	long machine;

	mfp = fopen("machines", "r");
	if(mfp == NULL)
	{
		cout<<"Failed to get the list of machines"<<endl;
		return;
	}
	while(fscanf(mfp, "%d %ld", &index, &machine) != EOF)
		machines.insert(map<long, int>::value_type(machine, index));
	fclose(mfp);
}

void init_data()
{
	for(int i=0; i<NUM_MACH; i++)
	{
		for(int j=0; j<NUM_HOUR; j++)
		{
			mem_cap[i][j] = 0.0f;
			cpu_cap[i][j] = 0.0f;
		}
		mach_cur[i] = 0;
	}
}

void save_to_file()
{
	char sfile[1024];
	for(int i=10327; i<=10328; i++)
	{
		memset(sfile, 0, 1024);
		sprintf(sfile, "/var/services/homes/dozhang/data/analysis/output/capacity-%d.data"    , i);
		
		FILE *sfp = fopen(sfile, "w+");
		if(sfp == NULL)
		{
			cout<<"Failed to open write file!"<<endl;
			return;
		}
		for(int j=0; j<NUM_HOUR; j++)
		{
			//int w = weight[i][j]; 
			//if(w == 0.0f)
			//  cout<<i<<", "<<j<<" is 0"<<endl;
			fprintf(sfp, "%d %f %f\n", j, cpu_cap[i][j], mem_cap[i][j]);
		}
		fclose(sfp);
	}
}

void process_last()
{
	cout<<"INIT "<<mach_cur[10328]<<" "<<mem_cap[10328][0]<<endl;
	for(int i=0; i<10329; i++)
	{
		while(mach_cur[i] < NUM_HOUR)
		{
			if(i == 10328)
			{
				cout<<"CUR "<<mach_cur[i]<<" "<<mem_cap[10328][0]<<endl;
				return;
			}
			mach_cur[i]++;
			cpu_cap[i][mach_cur[i]] = cpu_cap[i][mach_cur[i]-1];
			mem_cap[i][mach_cur[i]] = mem_cap[i][mach_cur[i]-1];
		}
	}
}

int main(int argc, char **argv)
{
	get_machine_list();
	init_data();

	FILE *fp = fopen("MachineCapTime.data", "r");
	if(fp == NULL)
	{
		cout<<"Failed to open file"<<endl;
		return 1;
	}

	long time;
	long mid;
	int event;
	float cpu;
	float mem;
	while(fscanf(fp, "%ld %ld %d %f %f", &time, &mid, &event, &cpu, &mem) != EOF)
	{
		//cout<<time<<" "<<mid<<" "<<event<<" "<<cpu<<" "<<mem<<endl;
		if(cpu==0.0f || mem==0.0f)
			continue;
		int index = machines[mid];
		//main stream
		if(time-base < 0)
		{
			cpu_cap[index][0] = cpu;
			mem_cap[index][0] = mem;
			continue;
		}

		int ti = (int)((time-base)/step);

		/*while(mach_cur[index] < ti)
		{
			mach_cur[index]++;
			cpu_cap[index][mach_cur[index]] = cpu_cap[index][mach_cur[index]-1];
			mem_cap[index][mach_cur[index]] = mem_cap[index][mach_cur[index]-1];
		}

		if(event==0 || event==2)
		{
			cpu_cap[index][ti] = cpu;
			mem_cap[index][ti] = mem;
		}
		if(event == 1)
		{
			cpu_cap[index][ti] = 0.0f;
			mem_cap[index][ti] = 0.0f;
		}
		mach_cur[index] = ti;*/
	}

	fclose(fp);

	process_last();
	save_to_file();
	
	return 0;
}

