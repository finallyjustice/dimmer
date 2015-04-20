#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <map>

using namespace std;

set<long> jset;

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

		long job_id = atol(line[2].c_str());	
		jset.insert(job_id);
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
		sprintf(path, "/var/services/homes/dozhang/data/TraceVersion2/job_events/part-%05d-of-00500.csv", i);
		
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

	set<long>::iterator it;
	long count = 0;
	for(it=jset.begin(); it!=jset.end(); it++)
	{
		cout<<count<<" "<<*it<<endl;
		count++;
	}

	return 0;	
}
