void DocumentWriter::reportDataReceived()
{
    ASSERT(m_decoder);
    if (m_hasReceivedSomeData)
        return;
    m_hasReceivedSomeData = true;
    if (m_decoder->encoding().usesVisualOrdering())
        m_frame->document()->setVisuallyOrdered();
    m_frame->document()->recalcStyle(Node::Force);
}
