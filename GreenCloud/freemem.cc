// usage: change REFRESH_THD to set the threshold to turn off a DIMM

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdio>

using namespace std;

#define NUM_MACH 12583		//total number of machines
#define REFRESH_THD 24.0f	// threshold to turn off DIMM
#define DIMM_TYPE 8			//max number of 8G dimms in a machine

float dimm_size[DIMM_TYPE];	 // size of 1, 2 ... 7, 8 DIMMs
float last_time[DIMM_TYPE];	 // size of time window that the DIMM can be turned off
float energy_time[NUM_MACH]; // saved energy of each machine
float mem_cap[NUM_MACH];	 // memory capacity of each machine
float tot_energy = 0.0f;

void init_dimm()
{
	for(int i=0; i<DIMM_TYPE; i++)
		dimm_size[i] = (i+1)*0.125f;
}

void init_time()
{
	for(int i=0; i<DIMM_TYPE; i++)
		last_time[i] = 0.0f;
}

void init_energy()
{
	for(int i=0; i<NUM_MACH; i++)
		energy_time[i] = 0.0f;
}

// read from MemCPUCap.csv and get the memory capacity of each machine
void init_capacity()
{
	FILE *fp = fopen("MemCPUCap.csv", "r");
	int c = 0;

	long mid;
	float cpu, mem, memc;
	while(fscanf(fp, "%ld %f %f %f", &mid, &cpu, &mem, &memc) != EOF)
	{
		mem_cap[c] = mem;
		c++;
	}

	fclose(fp);
}

int main(int argc, char **argv)
{
	init_dimm();
	init_energy();
	init_capacity();

	// i is the index of machine
	for(int i=1134; i<1135; i++)
	{
		init_time();

		stringstream path;	// the path of resource usage file of each machine
		path<<"usage/usage-"<<i<<".data";

		FILE *fp = fopen(path.str().c_str(), "r");
		if(fp == NULL)
		{
			cout<<"Failed to open file"<<endl;
			return 1;
		}

		int time;
		float cpu, mem, ucache, tcache; // mem-average mem usage for every 5 mins

		// for mem usage during each 5-min, totally 8351 5-min for each machine
		while(fscanf(fp, "%d %f %f %f %f", &time, &cpu, &mem, &ucache, &tcache) != EOF)
		{
			if(time > 8351)
				break;
			
			for(int j=0; j<DIMM_TYPE; j++)
			{
				// mem==0.0f means the server is down
				// mem_cap[i]-mem>=dimm_size[j] means there are free DIMMs
				if(mem!=0.0f && mem_cap[i]-mem>=dimm_size[j])
				{
					last_time[j] += 1.0f;		
				}
				else
				{
					if(last_time[j] >= REFRESH_THD)
						energy_time[i] += last_time[j]/12.0f;
					last_time[j] = 0.0f;
				}
			}
		}

		for(int j=0; j<DIMM_TYPE; j++)
		{
			if(last_time[j] >= REFRESH_THD)
				energy_time[i] += last_time[j]/12.0f;
		}

		fclose(fp);
	}
	
	for(int i=0; i<NUM_MACH; i++)
	{
		tot_energy += energy_time[i];
		cout<<i<<" "<<energy_time[i]<<endl;
	}
	cout<<"Total Energy: "<<tot_energy<<endl;
	
	return 0;
}
