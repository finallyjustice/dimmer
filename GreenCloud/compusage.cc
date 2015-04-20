// compute the resource usage of each machine during each 5-min

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define NUM_MACH 12583 // Total number of machines in Google
#define NUM_HOUR 8400  // Total number of 5-min

map<long, int> machines; // mapping between machine id and index
float mem[NUM_MACH][NUM_HOUR];
float cpu[NUM_MACH][NUM_HOUR];
float ucache[NUM_MACH][NUM_HOUR];
float tcache[NUM_MACH][NUM_HOUR];

long base = 600000000;
long step = 300000000;

// return the number of substring in string
int strstr_cnt(const char *string, const char *substring)
{
	int i, j, k;
	int count = 0;

	for(i=0; string[i]; i++)
	{
		for(j=i,k=0; (string[j]==substring[k] && j<strlen(string)); j++,k++)
		{
			if(!substring[k+1])
				count++;
		}
	}
	return count;
}

// return the position of s2 in s1 staring from pos
int substring_index(const char *s1, const char *s2, int pos)
{
	int i, j, k;
	for(i=pos; s1[i]; i++)
	{
		for(j=i,k=0; s1[j]==s2[k]; j++,k++)
		{
			if(!s2[k+1])
				return i;
		}
	}
	return -1;
}

// process one line of csv
char* fgetcsvline(FILE *f)
{
	char *ret_stat;
	vector<string> line;
	char data_buf[1024];
	string strbuf;

	// data_buf is one line in each file
	ret_stat = fgets(data_buf, 1024, f);
	if(ret_stat != NULL)
	{
		int pos = substring_index(data_buf, ",", 0);
		int lc = 0;
		int sb = 0;
		int sl = pos;
		while(pos > 0)
		{
			strbuf = (string)data_buf;
			string csv_buf = strbuf.substr(sb, sl);
			line.push_back(csv_buf);

			sb = sb+sl+1;
			pos = substring_index(data_buf, ",", pos+1);
			sl = pos-sb;
		}

		long mid = atol(line[4].c_str());
		long start = atol(line[0].c_str());
		long end = atol(line[1].c_str());
		float musage = atof(line[6].c_str());
		float cusage = atof(line[5].c_str());
		float uusage = atof(line[8].c_str());
		float tusage = atof(line[9].c_str());
		int midx;

		for(long c=start; c<end; c+=step)
		{
			if(c-base < 0)
				continue;
			int idx = (int)((c-base)/step);
			midx = machines[mid];
			mem[midx][idx] += musage;
			cpu[midx][idx] += cusage;
			ucache[midx][idx] += uusage;
			tcache[midx][idx] += tusage;
		}
	}

	return ret_stat;
}

// get the list of machine and establish the mapping between machine id and index
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
			mem[i][j]    = 0.0f;
			cpu[i][j]    = 0.0f;
			ucache[i][j] = 0.0f;
			tcache[i][j] = 0.0f;
		}
	}
}

// save the result into each file of each machine
void save_to_file()
{
	char sfile[1024];
	for(int i=0; i<NUM_MACH; i++)
	{
		memset(sfile, 0, 1024);
		sprintf(sfile, "/var/services/homes/dozhang/data/analysis/output/usage-%ld.data", i);

		FILE *sfp = fopen(sfile, "w+");
		if(sfp == NULL)
		{
			cout<<"Failed to open write file!"<<endl;
			return;
		}
		for(int j=0; j<NUM_HOUR; j++)
			fprintf(sfp, "%d %f %f %f %f\n", j, cpu[i][j], mem[i][j], ucache[i][j], tcache[i][j]);
		fclose(sfp);
	}
}

int main(int argc, char **argv)
{
	get_machine_list();
	init_data();

	char path[1024];
	char logbuf[1024];
	for(int i=0; i<500; i++)
	{
		cout<<"File : "<<i<<endl;
		memset(path, 0, 1024);
		sprintf(path, "/var/services/homes/dozhang/data/TraceVersion2/task_usage/part-%05d-of-00500.csv", i);
		
		FILE *fp;
		char *ret_stat;

		fp = fopen(path, "rt");
		if(fp == NULL)
		{
			cout<<"Failed to open file"<<endl;
			return 0;
		}

		ret_stat = fgetcsvline(fp);
		while(ret_stat != NULL)
			ret_stat = fgetcsvline(fp);

		fclose(fp);
		memset(logbuf, 0, 1024);
		sprintf(logbuf, "echo \"%d: %s is processed!\" >> /var/services/homes/dozhang/data/analysis/mem.log", i, path);
		system(logbuf);
	}
	
	save_to_file();

	return 0;
}
