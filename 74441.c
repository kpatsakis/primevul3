void XMLTokenizer::cdataBlock(const xmlChar* s, int len)
{
    if (m_parserStopped)
        return;

    if (m_parserPaused) {
        m_pendingCallbacks->appendCDATABlockCallback(s, len);
        return;
    }
    
    exitText();

    RefPtr<Node> newNode = new CDATASection(m_doc, toString(s, len));
    if (!m_currentNode->addChild(newNode.get()))
        return;
    if (m_view && !newNode->attached())
        newNode->attach();
}
