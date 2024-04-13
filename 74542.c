void FrameLoader::continueFragmentScrollAfterNavigationPolicy(const ResourceRequest& request, bool shouldContinue)
{
    bool isRedirect = m_quickRedirectComing || policyChecker()->loadType() == FrameLoadTypeRedirectWithLockedBackForwardList;
    m_quickRedirectComing = false;

    if (!shouldContinue)
        return;

    KURL url = request.url();
    
    m_documentLoader->replaceRequestURLForAnchorScroll(url);
    if (!isRedirect && !shouldTreatURLAsSameAsCurrent(url)) {
        

        history()->updateBackForwardListForFragmentScroll();
    }
    
    scrollToAnchor(url);
    
    if (!isRedirect)
        checkLoadComplete();
 
    m_client->dispatchDidChangeLocationWithinPage();
    m_client->didFinishLoad();
}
