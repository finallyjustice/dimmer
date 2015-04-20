BEGIN {
	max_cpu = 0.0;
	max_mem = 0.0;
	max_ucache = 0.0;
	max_tcache = 0.0;
}

{
	if($2 > max_cpu)
		max_cpu = $2;
	if($3 > max_mem)
		max_mem = $3;
	if($4 > max_ucache)
		max_ucache = $4;
	if($5 > max_tcache)
		max_tcache = $5;
}

END {
	print "Max CPU   : " max_cpu;
	print "Max Mem   : " max_mem;
	print "Max Ucache: " max_ucache;
	print "Max Tcache: " max_tcache;
}
