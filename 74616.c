void FrameLoader::receivedMainResourceError(const ResourceError& error, bool isComplete)
{
    RefPtr<Frame> protect(m_frame);

    RefPtr<DocumentLoader> loader = activeDocumentLoader();

    if (isComplete) {
        stop();
        if (m_client->shouldFallBack(error))
            handleFallbackContent();
    }

    if (m_state == FrameStateProvisional && m_provisionalDocumentLoader) {
        if (m_submittedFormURL == m_provisionalDocumentLoader->originalRequestCopy().url())
            m_submittedFormURL = KURL();
            
        history()->invalidateCurrentItemCachedPage();
        
        if (m_sentRedirectNotification)
            clientRedirectCancelledOrFinished(false);
    }

    loader->mainReceivedError(error, isComplete);
}
