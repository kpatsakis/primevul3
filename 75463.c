DocumentWriter::DocumentWriter(Frame* frame)
    : m_frame(frame)
    , m_hasReceivedSomeData(false)
    , m_encodingWasChosenByUser(false)
{
}
