void XMLTokenizer::resumeParsing()
{
    ASSERT(m_parserPaused);
    
    m_parserPaused = false;

    while (!m_pendingCallbacks->isEmpty()) {
        m_pendingCallbacks->callAndRemoveFirstCallback(this);
        
        if (m_parserPaused)
            return;
    }

    SegmentedString rest = m_pendingSrc;
    m_pendingSrc.clear();
    write(rest, false);

    if (m_finishCalled && m_pendingCallbacks->isEmpty())
        end();
}
