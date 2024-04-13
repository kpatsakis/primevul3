static void endElementNsHandler(void* closure, const xmlChar*, const xmlChar*, const xmlChar*)
{
    if (hackAroundLibXMLEntityBug(closure))
        return;
    
    getTokenizer(closure)->endElementNs();
}
