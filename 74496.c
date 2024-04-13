XMLTokenizer::~XMLTokenizer()
{
    setCurrentNode(0);
    if (m_parsingFragment && m_doc)
        m_doc->deref();
    if (m_pendingScript)
        m_pendingScript->removeClient(this);
    if (m_context)
        xmlFreeParserCtxt(m_context);
}
