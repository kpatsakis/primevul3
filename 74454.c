static void endDocumentHandler(void* closure)
{
    getTokenizer(closure)->endDocument();
    xmlSAX2EndDocument(closure);
}
