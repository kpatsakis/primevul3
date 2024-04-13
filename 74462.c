static inline XMLTokenizer* getTokenizer(void* closure)
{
    xmlParserCtxtPtr ctxt = static_cast<xmlParserCtxtPtr>(closure);
    return static_cast<XMLTokenizer*>(ctxt->_private);
}
