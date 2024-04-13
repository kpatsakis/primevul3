void FrameLoader::continueLoadAfterNavigationPolicy(const ResourceRequest&, PassRefPtr<FormState> formState, bool shouldContinue)
{
    ASSERT(m_policyDocumentLoader || !m_provisionalDocumentLoader->unreachableURL().isEmpty());

    bool isTargetItem = history()->provisionalItem() ? history()->provisionalItem()->isTargetItem() : false;

    bool canContinue = shouldContinue && (!isLoadingMainFrame() || m_frame->shouldClose());

    if (!canContinue) {
        if (m_quickRedirectComing)
            clientRedirectCancelledOrFinished(false);

        setPolicyDocumentLoader(0);

        if ((isTargetItem || isLoadingMainFrame()) && isBackForwardLoadType(policyChecker()->loadType()))
            if (Page* page = m_frame->page()) {
                Frame* mainFrame = page->mainFrame();
                if (HistoryItem* resetItem = mainFrame->loader()->history()->currentItem()) {
                    page->backForwardList()->goToItem(resetItem);
                    Settings* settings = m_frame->settings();
                    page->setGlobalHistoryItem((!settings || settings->privateBrowsingEnabled()) ? 0 : resetItem);
                }
            }
        return;
    }

    FrameLoadType type = policyChecker()->loadType();
    stopAllLoaders();
    
    if (!m_frame->page())
        return;

#if ENABLE(JAVASCRIPT_DEBUGGER) && ENABLE(INSPECTOR)
    if (Page* page = m_frame->page()) {
        if (page->mainFrame() == m_frame)
            page->inspectorController()->resumeDebugger();
    }
#endif

    setProvisionalDocumentLoader(m_policyDocumentLoader.get());
    m_loadType = type;
    setState(FrameStateProvisional);

    setPolicyDocumentLoader(0);

    if (isBackForwardLoadType(type) && loadProvisionalItemFromCachedPage())
        return;

    if (formState)
        m_client->dispatchWillSubmitForm(&PolicyChecker::continueLoadAfterWillSubmitForm, formState);
    else
        continueLoadAfterWillSubmitForm();
}
