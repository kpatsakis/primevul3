void FrameLoader::commitProvisionalLoad()
{
    ASSERT(m_client->hasWebView());
    ASSERT(m_state == FrameStateProvisional);
    RefPtr<DocumentLoader> pdl = m_provisionalDocumentLoader;
    RefPtr<Frame> protect(m_frame);

    closeOldDataSources();

    if (m_frame->document()) {
        RefPtr<SecurityOrigin> securityOrigin = SecurityOrigin::create(pdl->request().url());
        pdl->timing()->setHasSameOriginAsPreviousDocument(securityOrigin->canRequest(m_frame->document()->url()));
    }

    if (m_documentLoader)
        closeURL();
    if (pdl != m_provisionalDocumentLoader)
        return;

    RefPtr<DocumentLoader> loaderBeingCommitted = m_provisionalDocumentLoader.release();
    detachChildren();
    if (m_documentLoader)
        m_documentLoader->detachFromFrame();
    m_documentLoader = loaderBeingCommitted;
    m_state = FrameStateCommittedPage;

    if (isLoadingMainFrame())
        m_frame->page()->chrome().client().needTouchEvents(false);

    history()->updateForCommit();
    m_client->transitionToCommittedForNewPage();

    m_frame->navigationScheduler()->cancel();
    m_frame->editor().clearLastEditCommand();

    if (!m_stateMachine.creatingInitialEmptyDocument()) {
        DOMWindow* window = m_frame->domWindow();
        window->setStatus(String());
        window->setDefaultStatus(String());
    }
    started();
}
