void FrameLoader::endIfNotLoadingMainResource()
{
    if (m_isLoadingMainResource || !m_frame->page() || !m_frame->document())
        return;

    RefPtr<Frame> protector(m_frame);

    write(0, 0, true);
    m_frame->document()->finishParsing();
}
