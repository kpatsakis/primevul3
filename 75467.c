void DocumentWriter::clear()
{
    m_decoder = 0;
    m_hasReceivedSomeData = false;
    if (!m_encodingWasChosenByUser)
        m_encoding = String();
}
