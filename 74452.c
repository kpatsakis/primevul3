void XMLTokenizer::doWrite(const String& parseString)
{
    if (!m_context)
        initializeParserContext();
    
    if (parseString.length()) {
        const UChar BOM = 0xFEFF;
        const unsigned char BOMHighByte = *reinterpret_cast<const unsigned char*>(&BOM);
        xmlSwitchEncoding(m_context, BOMHighByte == 0xFF ? XML_CHAR_ENCODING_UTF16LE : XML_CHAR_ENCODING_UTF16BE);

        XMLTokenizerScope scope(m_doc->docLoader());
        xmlParseChunk(m_context, reinterpret_cast<const char*>(parseString.characters()), sizeof(UChar) * parseString.length(), 0);
    }
    
    if (m_doc->decoder() && m_doc->decoder()->sawError()) {
        handleError(fatal, "Encoding error", lineNumber(), columnNumber());
    }

    return;
}
