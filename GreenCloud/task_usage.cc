#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <map>
#include <sstream>
#include <fstream>

using namespace std;

#define NUM_JOBS 672074L
#define NUM_FILE 500L
#define JOB_PER_FILE 1345L

class Trace
{
public:
	long start;
	long end;
	float cpu;
	float mem;
};

class Task
{
public:
	long idx;
	long mid;
	vector<Trace> traces;
};

class Job
{
public:
	long jid;
	map<long, Task> tasks;
};

vector<Job> jobs;
map<long, long> jobId2Idx;

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

		long start = atol(line[0].c_str());
		long end   = atol(line[1].c_str());
		long jid   = atol(line[2].c_str());
		long tid   = atol(line[3].c_str());
		long mid   = atol(line[4].c_str());
		float cpu   = atof(line[5].c_str());
		float mem   = atof(line[6].c_str());

		// insert the trace into the vector of each tasks.traces
		Trace t;
		t.start = start;
		t.end   = end;
		t.cpu   = cpu;
		t.mem   = mem;

		long jidx = jobId2Idx[jid];
		jobs[jidx].tasks[tid].mid = mid;
		jobs[jidx].tasks[tid].traces.push_back(t);
	}

	return ret_stat;
}

long idx2file(long idx)
{
	return idx/JOB_PER_FILE;
}

void save_to_file()
{
	char sfile[1024];
	long num_job = jobs.size();

	long fidx = 0;
	long jidx = 0;
	while(fidx < NUM_FILE)
	{
		memset(sfile, 0, 1024);
		sprintf(sfile, "/var/services/homes/dozhang/data/result/taskusage/taskusage-%ld.data", fidx);
		FILE *sfp = fopen(sfile, "w+");
		if(sfp == NULL)
		{
			cout<<"Failed to open write file!"<<endl;
			return;
		}

		while(idx2file(jidx) == fidx)
		{
			long num_task = jobs[jidx].tasks.size();
			fprintf(sfp, "J %ld %ld %ld\n", jidx, jobs[jidx].jid, num_task);
			map<long, Task>::iterator it;
			long j = 0;
			for(it=jobs[jidx].tasks.begin(); it!=jobs[jidx].tasks.end(); it++)
			{
				long num_trace = it->second.traces.size();
				fprintf(sfp, "T %ld %ld %ld %ld\n", j, it->first, it->second.mid, num_trace);
				j++;
				for(long k=0; k<num_trace; k++)
				{
					fprintf(sfp, "C %ld %ld %f %f\n",
						it->second.traces[k].start,
						it->second.traces[k].end,
						it->second.traces[k].cpu,
						it->second.traces[k].mem);
				}
			}

			jidx++;
			if(jidx >= num_job)
				break;
		}

		fclose(sfp);
		fidx++;
	}
}

void init_job_task()
{
	ifstream path("/var/services/homes/dozhang/data/result/job_task_table.data");
	
	string line;
	while(getline(path, line))
	{
		stringstream job(line);	
		string tmp;
		long idx, jid, num, tid;
		
		job >> tmp; idx = atol(tmp.c_str());
		job >> tmp; jid = atol(tmp.c_str());
		job >> tmp; num = atol(tmp.c_str());

		Job j;
		j.jid = jid;
		jobs.push_back(j);

		jobId2Idx.insert(make_pair(jid, idx));

		for(long i=0; i<num; i++)
		{
			job >> tmp;
			tid = atol(tmp.c_str());
			Task t;
			t.idx = tid;
			//jobs[idx].tasks.push_back(t);
			jobs[idx].tasks.insert(make_pair(tid, t));
		}
	}

	path.close();
}

int main(int argc, char **argv)
{
	init_job_task();
	cout<<"Init Job Task"<<endl;

	char path[1024];
	char logbuf[1024];	
	for(int i=0; i<500; i++)
	{
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

	cout<<"Start to save file"<<endl; 
	save_to_file();

	return 0;	
}
