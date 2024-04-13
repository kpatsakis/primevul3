static void commentHandler(void* closure, const xmlChar* comment)
{
    if (hackAroundLibXMLEntityBug(closure))
        return;
    
    getTokenizer(closure)->comment(comment);
}
