static int printuserlist(char *buf, int bufsize, struct userlist* ul, char * delim){
	int printed = 0;

	for(; ul; ul = ul->next){
		if(printed > (bufsize - 64)) break;
		printed += sprintf(buf+printed, "%s%s", ul->user, ul->next?delim:"");
		if(printed > (bufsize - 64)) {
			printed += sprintf(buf+printed, "...");
			break;
		}
	}
	return printed;
}
