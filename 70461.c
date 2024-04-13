static int printportlist(char *buf, int bufsize, struct portlist* pl, char * delim){
	int printed = 0;

	for(; pl; pl = pl->next){
		if(printed > (bufsize - 64)) break;
		if(pl->startport != pl->endport)
			printed += sprintf(buf+printed, "%hu-%hu%s", pl->startport, pl->endport, pl->next?delim:"");
		else {
/*
			struct servent  *se=NULL;
			if(pl->startport)se = getservbyport((int)ntohs(pl->startport), NULL);
			printed += sprintf(buf+printed, "%hu(%s)%s", pl->startport, se?se->s_name:"unknown", pl->next?delim:"");
*/
			printed += sprintf(buf+printed, "%hu%s", pl->startport, pl->next?delim:"");
		}
		if(printed > (bufsize - 64)) {
			printed += sprintf(buf+printed, "...");
			break;
		}
	}
	return printed;
}
