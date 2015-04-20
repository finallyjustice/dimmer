BEGIN {
}

{
	cpu = $2 / ccap;
	mem = $3 / mcap;
	ucache = $4 / mcap;
	tcache = $5 / mcap;
	print $1 " " cpu " " mem " " ucache " " tcache; 
}

END {
}
