HashMap<String, String> parseAttributes(const String& string, bool& attrsOK)
{
    AttributeParseState state;
    state.gotAttributes = false;

    xmlSAXHandler sax;
    memset(&sax, 0, sizeof(sax));
    sax.startElementNs = attributesStartElementNsHandler;
    sax.initialized = XML_SAX2_MAGIC;
    xmlParserCtxtPtr parser = createStringParser(&sax, &state);
    String parseString = "<?xml version=\"1.0\"?><attrs " + string + " />";
    xmlParseChunk(parser, reinterpret_cast<const char*>(parseString.characters()), parseString.length() * sizeof(UChar), 1);
    if (parser->myDoc)
        xmlFreeDoc(parser->myDoc);
    xmlFreeParserCtxt(parser);
    attrsOK = state.gotAttributes;
    return state.attributes;
}
