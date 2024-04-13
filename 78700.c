void FrameLoader::stopAllLoaders(ClearProvisionalItemPolicy clearProvisionalItemPolicy)
{
    if (m_frame->document()->pageDismissalEventBeingDispatched() != Document::NoDismissal)
        return;

    if (m_inStopAllLoaders)
        return;

    RefPtr<Frame> protect(m_frame);

    m_inStopAllLoaders = true;

    if (clearProvisionalItemPolicy == ShouldClearProvisionalItem)
        history()->setProvisionalItem(0);

    for (RefPtr<Frame> child = m_frame->tree()->firstChild(); child; child = child->tree()->nextSibling())
        child->loader()->stopAllLoaders(clearProvisionalItemPolicy);
    if (m_provisionalDocumentLoader)
        m_provisionalDocumentLoader->stopLoading();
    if (m_documentLoader)
        m_documentLoader->stopLoading();

    if (m_provisionalDocumentLoader)
        m_provisionalDocumentLoader->detachFromFrame();
    m_provisionalDocumentLoader = 0;

    m_checkTimer.stop();

    m_inStopAllLoaders = false;
}
