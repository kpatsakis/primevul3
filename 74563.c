void FrameLoader::finishedParsing()
{
    if (m_creatingInitialEmptyDocument)
        return;

    m_frame->injectUserScripts(InjectAtDocumentEnd);

    RefPtr<Frame> protector = m_frame->view() ? m_frame : 0;

    m_client->dispatchDidFinishDocumentLoad();

    checkCompleted();

    if (!m_frame->view())
        return; // We are being destroyed by something checkCompleted called.

    m_frame->view()->restoreScrollbar();
    m_frame->view()->scrollToFragment(m_URL);
}
