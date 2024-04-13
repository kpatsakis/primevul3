void FrameLoader::receivedMainResourceError(const ResourceError& error)
{
    RefPtr<Frame> protect(m_frame);

    RefPtr<DocumentLoader> loader = activeDocumentLoader();
    if (m_frame->document()->parser())
        m_frame->document()->parser()->stopParsing();

    ResourceError c(ResourceError::cancelledError(KURL()));
    if (error.errorCode() != c.errorCode() || error.domain() != c.domain())
        handleFallbackContent();

    if (m_state == FrameStateProvisional && m_provisionalDocumentLoader) {
        m_client->dispatchDidFailProvisionalLoad(error);
        if (loader != m_provisionalDocumentLoader)
            return;
        m_provisionalDocumentLoader->detachFromFrame();
        m_provisionalDocumentLoader = 0;
        m_progressTracker->progressCompleted();
        m_state = FrameStateComplete;

        RefPtr<HistoryItem> item = m_frame->page()->mainFrame()->loader()->history()->currentItem();
        if (isBackForwardLoadType(loadType()) && !history()->provisionalItem() && item)
            m_frame->page()->backForward().setCurrentItem(item.get());
    }

    checkCompleted();
    if (m_frame->page())
        checkLoadComplete();
}
