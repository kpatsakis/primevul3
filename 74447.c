void XMLTokenizer::comment(const xmlChar* s)
{
    if (m_parserStopped)
        return;

    if (m_parserPaused) {
        m_pendingCallbacks->appendCommentCallback(s);
        return;
    }
    
    exitText();

    RefPtr<Node> newNode = new Comment(m_doc, toString(s));
    m_currentNode->addChild(newNode.get());
    if (m_view && !newNode->attached())
        newNode->attach();
}
