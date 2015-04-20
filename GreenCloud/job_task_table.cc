#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <map>

using namespace std;

set<long> jobs;
map<long, set<long> > tasks;
long tot_jobs = 0;
long tot_tasks = 0;

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

		long jid = atol(line[2].c_str());
		long tid = atol(line[3].c_str());

		// insert job id into set
		//jobs.insert(jid);

		// process task index
		if(tasks.find(jid) == tasks.end())
		{
			// if there is no job id in the map
			set<long> s;
			tasks.insert(make_pair(jid, s));
		}
		tasks[jid].insert(tid);
	}

	return ret_stat;
}

int main(int argc, char **argv)
{
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
		sprintf(logbuf, "echo \"%d: %s is processed! (%ld-%ld)\" >> /var/services/homes/dozhang/data/analysis/mem.log", i, path, jobs.size(), tasks.size());
		system(logbuf);
	}

	set<long>::iterator sit;
	for(sit=jobs.begin(); sit!=jobs.end(); sit++)
		tot_jobs++;
	
	map<long, set<long> >::iterator mit;
	for(mit=tasks.begin(); mit!=tasks.end(); mit++)
	{
		tot_tasks += mit->second.size();
	}

	cout<<"Total Jobs : "<<tot_jobs<<endl;
	cout<<"Total Tasks: "<<tot_tasks<<endl;
	
	// print all tasks in each job
	/*long count=0;
	map<long, set<long> >::iterator it1;
	for(it1=tasks.begin(); it1!=tasks.end(); it1++)
	{
		cout<<count<<" "<<it1->first<<" ";
		set<long>::iterator it2;
		for(it2=it1->second.begin(); it2!=it1->second.end(); it2++)
			cout<<*it2<<" ";
		cout<<endl;
		count++;
	}*/

	return 0;	
}
