BEGIN {
	FS = ",";
}

{
	print $1 " " $2 " " $3 " " $4;
}

END {
}
