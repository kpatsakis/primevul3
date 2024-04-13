 String DocumentWriter::deprecatedFrameEncoding() const
 {
     return m_frame->document()->url().isEmpty() ? m_encoding : encoding();
}
