{ 
	if (NF < 8)
		print "Linha pequena:", $0;
	else
		print "Linha grande:", $0;
}
