void FrameLoader::checkNavigationPolicyAndContinueFragmentScroll(const NavigationAction& action, bool isNewNavigation)
{
    m_documentLoader->setTriggeringAction(action);

    const ResourceRequest& request = action.resourceRequest();
    if (!m_documentLoader->shouldContinueForNavigationPolicy(request, DocumentLoader::PolicyCheckFragment))
        return;

    if (m_provisionalDocumentLoader && !equalIgnoringFragmentIdentifier(m_provisionalDocumentLoader->request().url(), request.url())) {
        m_provisionalDocumentLoader->stopLoading();
        if (m_provisionalDocumentLoader)
            m_provisionalDocumentLoader->detachFromFrame();
        m_provisionalDocumentLoader = 0;
    }
    loadInSameDocument(request.url(), 0, isNewNavigation);
}
