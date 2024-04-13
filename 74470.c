static void internalSubsetHandler(void* closure, const xmlChar* name, const xmlChar* externalID, const xmlChar* systemID)
{
    getTokenizer(closure)->internalSubset(name, externalID, systemID);
    xmlSAX2InternalSubset(closure, name, externalID, systemID);
}
