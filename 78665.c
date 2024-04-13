void FrameLoader::finishedParsing()
{
    if (m_stateMachine.creatingInitialEmptyDocument())
        return;

    RefPtr<Frame> protector = m_frame->view() ? m_frame : 0;

    m_client->dispatchDidFinishDocumentLoad();

    checkCompleted();

    if (!m_frame->view())
        return; // We are being destroyed by something checkCompleted called.

    m_frame->view()->restoreScrollbar();
    scrollToFragmentWithParentBoundary(m_frame->document()->url());
}
