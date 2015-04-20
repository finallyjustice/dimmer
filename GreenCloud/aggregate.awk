# compute the aggregated resource usage of all machines
# output to aggreate.data

BEGIN {
	time_slot = 8400;
	mem[time_slot];		# memory usage
	cpu[time_slot];		# cpu usage
	ucache[time_slot];	# cache not mapped to any process
	tcache[time_slot];  # total file mapped cache

	for(i=0; i<time_slot; i++)
	{
		mem[i] = 0.0;
		cpu[i] = 0.0;
		ucache[i] = 0.0;
		tcache[i] = 0.0;
	}
}

{
	if($1 < time_slot)
	{
		cpu[$1] += $2;
		mem[$1] +=  $3;
		ucache[$1] += $4;
		tcache[$1] += $5;
	}
}

END {
for(i=0; i<time_slot; i++)
	printf i " " cpu[i] " " mem[i] " " ucache[i] " " tcache[i] "\n"; 
}
