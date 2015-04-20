#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <map>
#include <fstream>

using namespace std;

#define NUM_JOB 672074L 

map<long, long> jobs;		// map from index to job id
vector<long> vidx;
vector<set<long> > tasks;	// one task set for each job

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

		long ljid = atol(line[2].c_str());
		long ltid = atol(line[3].c_str());
		long lidx = jobs[ljid];
		tasks[lidx].insert(ltid);	
	}

	return ret_stat;
}

// create an entry for each job in the tmap
void init_jobs()
{
	ifstream path("data/jobs.data");
	string index;
	string jobid;
	long lidx;
	long ljid;

	while(path >> index)
	{
		path >> jobid;

		lidx = atol(index.c_str());
		ljid = atol(jobid.c_str());
		jobs.insert(make_pair(ljid, lidx));

		// init the task set for each job 
		set<long> s;
		tasks.push_back(s);
		vidx.push_back(ljid);
	}

	path.close();
}

int main(int argc, char **argv)
{
	init_jobs();
	
	char path[1024];
	char logbuf[1024];	
	for(int i=0; i<500; i++)
	{
		memset(path, 0, 1024);
		sprintf(path, "/var/services/homes/dozhang/data/TraceVersion2/task_events/part-%05d-of-00500.csv", i);
		
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

	long tot_task = 0;
	for(long i=0; i<NUM_JOB; i++)
	{
		tot_task += tasks[i].size();
		cout<<i<<" ";
		cout<<vidx[i]<<" ";
		cout<<tasks[i].size()<<" ";

		set<long>::iterator it;
		for(it=tasks[i].begin(); it!=tasks[i].end(); it++)
			cout<<*it<<" ";
		cout<<endl;
	}
	cout<<"Tasks: "<<tot_task<<endl;

	return 0;	
}
