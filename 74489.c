void* xmlDocPtrForString(DocLoader* docLoader, const String& source, const String& url)
{
    if (source.isEmpty())
        return 0;

    const UChar BOM = 0xFEFF;
    const unsigned char BOMHighByte = *reinterpret_cast<const unsigned char*>(&BOM);

    XMLTokenizerScope scope(docLoader, errorFunc, 0);
    xmlDocPtr sourceDoc = xmlReadMemory(reinterpret_cast<const char*>(source.characters()),
                                        source.length() * sizeof(UChar),
                                        url.latin1().data(),
                                        BOMHighByte == 0xFF ? "UTF-16LE" : "UTF-16BE", 
                                        XSLT_PARSE_OPTIONS);
    return sourceDoc;
}
