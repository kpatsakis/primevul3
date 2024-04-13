NameValueParserStartElt(void * d, const char * name, int l)
{
	struct NameValueParserData * data = (struct NameValueParserData *)d;
	data->topelt = 1;
    if(l>63)
        l = 63;
    memcpy(data->curelt, name, l);
    data->curelt[l] = '\0';
	data->cdata = NULL;
	data->cdatalen = 0;
}
