void FrameLoader::transitionToCommitted(PassRefPtr<CachedPage> cachedPage)
{
    ASSERT(m_client->hasWebView());
    ASSERT(m_state == FrameStateProvisional);

    if (m_state != FrameStateProvisional)
        return;

    m_client->setCopiesOnScroll();
    history()->updateForCommit();

    DocumentLoader* pdl = m_provisionalDocumentLoader.get();
    if (m_documentLoader)
        closeURL();
    if (pdl != m_provisionalDocumentLoader)
        return;

    if (m_documentLoader)
        m_documentLoader->stopLoadingSubresources();
    if (m_documentLoader)
        m_documentLoader->stopLoadingPlugIns();

    setDocumentLoader(m_provisionalDocumentLoader.get());
    setProvisionalDocumentLoader(0);
    setState(FrameStateCommittedPage);

    DocumentLoader* dl = m_documentLoader.get();
    String ptitle = dl->title(); 

    switch (m_loadType) {
        case FrameLoadTypeForward:
        case FrameLoadTypeBack:
        case FrameLoadTypeBackWMLDeckNotAccessible:
        case FrameLoadTypeIndexedBackForward:
            if (Page* page = m_frame->page())
                if (page->backForwardList()) {
                    history()->updateForBackForwardNavigation();

                    if (cachedPage) {
                        DocumentLoader* cachedDocumentLoader = cachedPage->documentLoader();
                        ASSERT(cachedDocumentLoader);
                        cachedDocumentLoader->setFrame(m_frame);
                        m_client->transitionToCommittedFromCachedFrame(cachedPage->cachedMainFrame());
                        
                    } else
                        m_client->transitionToCommittedForNewPage();
                }
            break;

        case FrameLoadTypeReload:
        case FrameLoadTypeReloadFromOrigin:
        case FrameLoadTypeSame:
        case FrameLoadTypeReplace:
            history()->updateForReload();
            m_client->transitionToCommittedForNewPage();
            break;

        case FrameLoadTypeStandard:
            history()->updateForStandardLoad();
#ifndef BUILDING_ON_TIGER
            if (m_frame->view())
                m_frame->view()->setScrollbarsSuppressed(true);
#endif
            m_client->transitionToCommittedForNewPage();
            break;

        case FrameLoadTypeRedirectWithLockedBackForwardList:
            history()->updateForRedirectWithLockedBackForwardList();
            m_client->transitionToCommittedForNewPage();
            break;

        default:
            ASSERT_NOT_REACHED();
    }

    m_responseMIMEType = dl->responseMIMEType();

    ASSERT(m_frame->view());

    if (m_creatingInitialEmptyDocument)
        return;
    
    m_committedFirstRealDocumentLoad = true;

    if (!m_client->hasHTMLView())
        receivedFirstData();
    else if (cachedPage) {
        dispatchDidCommitLoad();

        if (!ptitle.isNull()) 
            m_client->dispatchDidReceiveTitle(ptitle);         
    }
}
