void XMLTokenizer::characters(const xmlChar* s, int len)
{
    if (m_parserStopped)
        return;
    
    if (m_parserPaused) {
        m_pendingCallbacks->appendCharactersCallback(s, len);
        return;
    }
    
    if (m_currentNode->isTextNode() || enterText())
        m_bufferedText.append(s, len);
}
