static void startDocumentHandler(void* closure)
{
    xmlParserCtxt* ctxt = static_cast<xmlParserCtxt*>(closure);
    getTokenizer(closure)->startDocument(ctxt->version, ctxt->encoding, ctxt->standalone);
    xmlSAX2StartDocument(closure);
}
