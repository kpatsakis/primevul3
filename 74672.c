void FrameLoader::write(const String& str)
{
    if (str.isNull())
        return;

    if (!m_receivedData) {
        m_receivedData = true;
        m_frame->document()->setParseMode(Document::Strict);
    }

    if (Tokenizer* tokenizer = m_frame->document()->tokenizer())
        tokenizer->write(str, true);
}
