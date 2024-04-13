TextResourceDecoder* DocumentWriter::createDecoderIfNeeded()
{
    if (!m_decoder) {
        if (Settings* settings = m_frame->settings()) {
            m_decoder = TextResourceDecoder::create(m_mimeType,
                settings->defaultTextEncodingName(),
                settings->usesEncodingDetector());
            Frame* parentFrame = m_frame->tree()->parent();
            if (canReferToParentFrameEncoding(m_frame, parentFrame))
                m_decoder->setHintEncoding(parentFrame->document()->decoder());
        } else
            m_decoder = TextResourceDecoder::create(m_mimeType, String());
        Frame* parentFrame = m_frame->tree()->parent();
        if (m_encoding.isEmpty()) {
            if (canReferToParentFrameEncoding(m_frame, parentFrame))
                m_decoder->setEncoding(parentFrame->document()->inputEncoding(), TextResourceDecoder::EncodingFromParentFrame);
        } else {
            m_decoder->setEncoding(m_encoding,
                m_encodingWasChosenByUser ? TextResourceDecoder::UserChosenEncoding : TextResourceDecoder::EncodingFromHTTPHeader);
        }
        m_frame->document()->setDecoder(m_decoder.get());
    }
    return m_decoder.get();
}
