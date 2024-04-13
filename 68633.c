GetValueFromNameValueList(struct NameValueParserData * pdata,
                          const char * Name)
{
    struct NameValue * nv;
    char * p = NULL;
    for(nv = pdata->l_head;
        (nv != NULL) && (p == NULL);
        nv = nv->l_next)
    {
        if(strcmp(nv->name, Name) == 0)
            p = nv->value;
    }
    return p;
}
