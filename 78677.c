void FrameLoader::loadWithNavigationAction(const ResourceRequest& request, const NavigationAction& action, FrameLoadType type, PassRefPtr<FormState> formState, const SubstituteData& substituteData, const String& overrideEncoding)
{
    ASSERT(m_client->hasWebView());
    if (m_frame->document()->pageDismissalEventBeingDispatched() != Document::NoDismissal)
        return;

    if (!m_stateMachine.committedFirstRealDocumentLoad() && m_frame->ownerElement() && !m_frame->ownerElement()->dispatchBeforeLoadEvent(request.url().string()))
        return;

    if (!m_stateMachine.startedFirstRealLoad())
        m_stateMachine.advanceTo(FrameLoaderStateMachine::StartedFirstRealLoad);

    m_policyDocumentLoader = m_client->createDocumentLoader(request, substituteData.isValid() ? substituteData : defaultSubstituteDataForURL(request.url()));
    m_policyDocumentLoader->setFrame(m_frame);
    m_policyDocumentLoader->setTriggeringAction(action);
    m_policyDocumentLoader->setReplacesCurrentHistoryItem(type == FrameLoadTypeRedirectWithLockedBackForwardList);
    m_policyDocumentLoader->setIsClientRedirect(m_startingClientRedirect);

    if (Frame* parent = m_frame->tree()->parent())
        m_policyDocumentLoader->setOverrideEncoding(parent->loader()->documentLoader()->overrideEncoding());
    else if (!overrideEncoding.isEmpty())
        m_policyDocumentLoader->setOverrideEncoding(overrideEncoding);
    else if (m_documentLoader)
        m_policyDocumentLoader->setOverrideEncoding(m_documentLoader->overrideEncoding());

    RefPtr<Frame> protect(m_frame);
    if ((!m_policyDocumentLoader->shouldContinueForNavigationPolicy(request, DocumentLoader::PolicyCheckStandard) || !shouldClose()) && m_policyDocumentLoader) {
        m_policyDocumentLoader->detachFromFrame();
        m_policyDocumentLoader = 0;
        return;
    }

    stopAllLoaders(ShouldNotClearProvisionalItem);

    if (!m_frame->page() || !m_policyDocumentLoader)
        return;

    if (isLoadingMainFrame())
        m_frame->page()->inspectorController().resume();
    m_frame->navigationScheduler()->cancel();

    m_provisionalDocumentLoader = m_policyDocumentLoader.release();
    m_loadType = type;
    m_state = FrameStateProvisional;

    if (formState)
        m_client->dispatchWillSubmitForm(formState);

    m_progressTracker->progressStarted();
    if (m_provisionalDocumentLoader->isClientRedirect())
        m_provisionalDocumentLoader->appendRedirect(m_frame->document()->url());
    m_provisionalDocumentLoader->appendRedirect(m_provisionalDocumentLoader->request().url());
    m_client->dispatchDidStartProvisionalLoad();
    ASSERT(m_provisionalDocumentLoader);
    m_provisionalDocumentLoader->startLoadingMainResource();
}
