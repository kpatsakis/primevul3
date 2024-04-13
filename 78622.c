bool XSLStyleSheet::parseString(const String& source)
{
    if (!m_stylesheetDocTaken)
        xmlFreeDoc(m_stylesheetDoc);
    m_stylesheetDocTaken = false;

    PageConsole* console = 0;
    Frame* frame = ownerDocument()->frame();
    if (frame && frame->page())
        console = frame->page()->console();

    XMLDocumentParserScope scope(fetcher(), XSLTProcessor::genericErrorFunc, XSLTProcessor::parseErrorFunc, console);
    XMLParserInput input(source);

    xmlParserCtxtPtr ctxt = xmlCreateMemoryParserCtxt(input.data(), input.size());
    if (!ctxt)
        return 0;

    if (m_parentStyleSheet) {
        xmlDictFree(ctxt->dict);
        ctxt->dict = m_parentStyleSheet->m_stylesheetDoc->dict;
        xmlDictReference(ctxt->dict);
    }

    m_stylesheetDoc = xmlCtxtReadMemory(ctxt, input.data(), input.size(),
        finalURL().string().utf8().data(), input.encoding(),
        XML_PARSE_NOENT | XML_PARSE_DTDATTR | XML_PARSE_NOWARNING | XML_PARSE_NOCDATA);

    xmlFreeParserCtxt(ctxt);
    loadChildSheets();
    return m_stylesheetDoc;
}
