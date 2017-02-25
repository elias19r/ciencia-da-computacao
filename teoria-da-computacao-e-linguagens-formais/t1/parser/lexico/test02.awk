BEGIN {
	print "Hello";
}

{
	if (NF == 6)
		print $0;
}
