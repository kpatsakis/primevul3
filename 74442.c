static void cdataBlockHandler(void* closure, const xmlChar* s, int len)
{
    if (hackAroundLibXMLEntityBug(closure))
        return;
    
    getTokenizer(closure)->cdataBlock(s, len);
}
