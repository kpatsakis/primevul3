String FrameLoader::encoding() const
{
    if (m_encodingWasChosenByUser && !m_encoding.isEmpty())
        return m_encoding;
    if (m_decoder && m_decoder->encoding().isValid())
        return m_decoder->encoding().name();
    Settings* settings = m_frame->settings();
    return settings ? settings->defaultTextEncodingName() : String();
}
