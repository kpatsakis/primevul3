NameValueParserEndElt(void * d, const char * name, int l)
{
    struct NameValueParserData * data = (struct NameValueParserData *)d;
    struct NameValue * nv;
	(void)name;
	(void)l;
	if(!data->topelt)
		return;
	if(strcmp(data->curelt, "NewPortListing") != 0)
	{
		int l;
		/* standard case. Limited to n chars strings */
		l = data->cdatalen;
	    nv = malloc(sizeof(struct NameValue));
		if(nv == NULL)
		{
			/* malloc error */
#ifdef DEBUG
			fprintf(stderr, "%s: error allocating memory",
			        "NameValueParserEndElt");
#endif /* DEBUG */
			return;
		}
	    if(l>=(int)sizeof(nv->value))
	        l = sizeof(nv->value) - 1;
	    strncpy(nv->name, data->curelt, 64);
		nv->name[63] = '\0';
		if(data->cdata != NULL)
		{
			memcpy(nv->value, data->cdata, l);
			nv->value[l] = '\0';
		}
		else
		{
			nv->value[0] = '\0';
		}
		nv->l_next = data->l_head;	/* insert in list */
		data->l_head = nv;
	}
	data->cdata = NULL;
	data->cdatalen = 0;
	data->topelt = 0;
}
