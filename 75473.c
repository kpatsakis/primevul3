void DocumentWriter::endIfNotLoadingMainResource()
{
    if (m_frame->loader()->isLoadingMainResource() || !m_frame->page() || !m_frame->document())
        return;

    RefPtr<Frame> protector(m_frame);

    if (!m_parser)
        return;
    m_parser->flush(this);
    if (!m_parser)
        return;
    m_parser->finish();
    m_parser = 0;
}
