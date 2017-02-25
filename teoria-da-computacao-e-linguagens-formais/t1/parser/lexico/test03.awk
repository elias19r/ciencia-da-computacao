BEGIN {
	print "Hello";
}

{
	if (NF == 6)
		print $0;
}

{ 
	if (NF > 8) {
		print "Linha pequena:", $0;
	} else { 
		print "Linha grande:", $0;
	} 
}
