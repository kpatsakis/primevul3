static char *fgets_extending(char **buf, int *buflen, FILE *stream)
{
	char *rc;
	char endchar;
	int offset = 0;
	char *newbuf;

	do{
		rc = fgets(&((*buf)[offset]), *buflen-offset, stream);
		if(feof(stream)){
			return rc;
		}

		endchar = (*buf)[strlen(*buf)-1];
		if(endchar == '\n'){
			return rc;
		}
		/* No EOL char found, so extend buffer */
		offset = *buflen-1;
		*buflen += 1000;
		newbuf = realloc(*buf, *buflen);
		if(!newbuf){
			return NULL;
		}
		*buf = newbuf;
	}while(1);
}
