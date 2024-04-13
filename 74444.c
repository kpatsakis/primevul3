static void charactersHandler(void* closure, const xmlChar* s, int len)
{
    if (hackAroundLibXMLEntityBug(closure))
        return;
    
    getTokenizer(closure)->characters(s, len);
}
