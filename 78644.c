void FrameLoader::checkLoadCompleteForThisFrame()
{
    ASSERT(m_client->hasWebView());
    if (m_state != FrameStateCommittedPage)
        return;

    if (!m_documentLoader || (m_documentLoader->isLoadingInAPISense() && !m_inStopAllLoaders))
        return;

    m_state = FrameStateComplete;


    if (m_frame->page()) {
        if (isBackForwardLoadType(m_loadType) || m_loadType == FrameLoadTypeReload || m_loadType == FrameLoadTypeReloadFromOrigin)
            history()->restoreScrollPositionAndViewState();
    }

    if (!m_stateMachine.committedFirstRealDocumentLoad())
        return;

    m_progressTracker->progressCompleted();
    if (Page* page = m_frame->page()) {
        if (m_frame == page->mainFrame())
            page->resetRelevantPaintedObjectCounter();
    }

    const ResourceError& error = m_documentLoader->mainDocumentError();
    if (!error.isNull())
        m_client->dispatchDidFailLoad(error);
    else
        m_client->dispatchDidFinishLoad();
    m_loadType = FrameLoadTypeStandard;
}
