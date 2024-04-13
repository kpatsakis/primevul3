static xmlParserCtxtPtr createMemoryParser(xmlSAXHandlerPtr handlers, void* userData, const char* chunk)
{
    if (!didInit) {
        xmlInitParser();
        xmlRegisterInputCallbacks(matchFunc, openFunc, readFunc, closeFunc);
        xmlRegisterOutputCallbacks(matchFunc, openFunc, writeFunc, closeFunc);
        libxmlLoaderThread = currentThread();
        didInit = true;
    }

    xmlParserCtxtPtr parser = xmlCreateMemoryParserCtxt(chunk, xmlStrlen((const xmlChar*)chunk));

    if (!parser)
        return 0;

    memcpy(parser->sax, handlers, sizeof(xmlSAXHandler));

    xmlCtxtUseOptions(parser, XML_PARSE_NODICT | XML_PARSE_NOENT);

    parser->sax2 = 1;
    parser->instate = XML_PARSER_CONTENT; // We are parsing a CONTENT
    parser->depth = 0;
    parser->str_xml = xmlDictLookup(parser->dict, BAD_CAST "xml", 3);
    parser->str_xmlns = xmlDictLookup(parser->dict, BAD_CAST "xmlns", 5);
    parser->str_xml_ns = xmlDictLookup(parser->dict, XML_XML_NAMESPACE, 36);
    parser->_private = userData;

    return parser;
}
