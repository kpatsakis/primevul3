static aClient *decode_puid(char *puid)
{
	aClient *cptr;
	char *it, *it2;
	int cookie = 0;

	if ((it = strrchr(puid, '!')) == NULL)
		return NULL;

	*it++ = '\0';

	if ((it2 = strrchr(it, '.')) != NULL)
	{
		*it2++ = '\0';
		cookie = atoi(it2);
	}

	if (stricmp(me.name, puid))
		return NULL;

	list_for_each_entry(cptr, &unknown_list, lclient_node)
		if (cptr->local->sasl_cookie == cookie)
			return cptr;

	return NULL;
}
