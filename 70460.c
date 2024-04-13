static int printiplist(char *buf, int bufsize, struct iplist* ipl, char * delim){
	int printed = 0;
	for(; ipl; ipl = ipl->next){
		if(printed > (bufsize - 128)) break;
		printed += printiple(buf+printed, ipl);
		if(printed > (bufsize - 128)) {
			printed += sprintf(buf+printed, "...");
			break;
		}
	}
	return printed;
}
