void XMLTokenizer::doEnd()
{
#if ENABLE(XSLT)
    if (m_sawXSLTransform) {
        m_doc->setTransformSource(xmlDocPtrForString(m_doc->docLoader(), m_originalSourceForTransform, m_doc->url().string()));
        
        m_doc->setParsing(false); // Make the doc think it's done, so it will apply xsl sheets.
        m_doc->updateStyleSelector();
        m_doc->setParsing(true);
        m_parserStopped = true;
    }
#endif

    if (m_context) {
        {
            XMLTokenizerScope scope(m_doc->docLoader());
            xmlParseChunk(m_context, 0, 0, 1);
        }

        if (m_context->myDoc)
            xmlFreeDoc(m_context->myDoc);
        xmlFreeParserCtxt(m_context);
        m_context = 0;
    }
}
