static xmlParserCtxtPtr createStringParser(xmlSAXHandlerPtr handlers, void* userData)
{
    if (!didInit) {
        xmlInitParser();
        xmlRegisterInputCallbacks(matchFunc, openFunc, readFunc, closeFunc);
        xmlRegisterOutputCallbacks(matchFunc, openFunc, writeFunc, closeFunc);
        libxmlLoaderThread = currentThread();
        didInit = true;
    }

    xmlParserCtxtPtr parser = xmlCreatePushParserCtxt(handlers, 0, 0, 0, 0);
    parser->_private = userData;
    parser->replaceEntities = true;
    const UChar BOM = 0xFEFF;
    const unsigned char BOMHighByte = *reinterpret_cast<const unsigned char*>(&BOM);
    xmlSwitchEncoding(parser, BOMHighByte == 0xFF ? XML_CHAR_ENCODING_UTF16LE : XML_CHAR_ENCODING_UTF16BE);

    return parser;
}
