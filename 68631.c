ClearNameValueList(struct NameValueParserData * pdata)
{
    struct NameValue * nv;
	if(pdata->portListing)
	{
		free(pdata->portListing);
		pdata->portListing = NULL;
		pdata->portListingLength = 0;
	}
    while((nv = pdata->l_head) != NULL)
    {
		pdata->l_head = nv->l_next;
        free(nv);
    }
}
