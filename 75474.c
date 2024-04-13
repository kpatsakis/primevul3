void DocumentWriter::replaceDocument(const String& source)
{
    m_frame->loader()->stopAllLoaders();
    begin(m_frame->document()->url(), true, m_frame->document()->securityOrigin());

    if (!source.isNull()) {
        if (!m_hasReceivedSomeData) {
            m_hasReceivedSomeData = true;
            m_frame->document()->setCompatibilityMode(Document::NoQuirksMode);
        }

        if (DocumentParser* parser = m_frame->document()->parser())
            parser->append(source);
    }

    end();
}
