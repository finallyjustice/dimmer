CC=g++

all: compusage compweight freemem check capchange platform count_platform \
		mach_mem_cpu_diff count_mach_attr count_job count_task job_task_table \
		task_usage check_table

compusage: compusage.cc
	${CC} compusage.cc -o compusage

compweight: compweight.cc
	${CC} compweight.cc -o compweight

freemem: freemem.cc
	${CC} freemem.cc -o freemem

check: check.cc
	${CC} check.cc -o check

capchange: capchange.cc
	${CC} capchange.cc -o capchange

platform: platform.cc
	${CC} platform.cc -o platform

count_platform: count_platform.cc
	${CC} count_platform.cc -o count_platform

mach_mem_cpu_diff: mach_mem_cpu_diff.cc
	${CC} mach_mem_cpu_diff.cc -o mach_mem_cpu_diff

count_mach_attr: count_mach_attr.cc
	${CC} count_mach_attr.cc -o count_mach_attr

count_job: count_job.cc
	${CC} count_job.cc -o count_job

count_task: count_task.cc
	${CC} count_task.cc -o count_task

job_task_table:
	${CC} job_task_table.cc -o job_task_table

task_usage: task_usage.cc
	${CC} task_usage.cc -o task_usage

check_table: check_table.cc
	${CC} check_table.cc -o check_table

clean:
	rm compusage compweight freemem check capchange platform count_platform \
		mach_mem_cpu_diff count_mach_attr count_job count_task job_task_table \
		task_usage check_table
