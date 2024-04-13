void FrameLoader::checkLoadCompleteForThisFrame()
{
    ASSERT(m_client->hasWebView());

    switch (m_state) {
        case FrameStateProvisional: {
            if (m_delegateIsHandlingProvisionalLoadError)
                return;

            RefPtr<DocumentLoader> pdl = m_provisionalDocumentLoader;
            if (!pdl)
                return;
                
            const ResourceError& error = pdl->mainDocumentError();
            if (error.isNull())
                return;

            RefPtr<HistoryItem> item;
            if (Page* page = m_frame->page())
                if (isBackForwardLoadType(loadType()) && m_frame == page->mainFrame())
                    item = history()->currentItem();
                
            bool shouldReset = true;
            if (!(pdl->isLoadingInAPISense() && !pdl->isStopping())) {
                m_delegateIsHandlingProvisionalLoadError = true;
                m_client->dispatchDidFailProvisionalLoad(error);
                m_delegateIsHandlingProvisionalLoadError = false;

                stopLoadingSubframes();
                pdl->stopLoading();

                if (pdl == m_provisionalDocumentLoader)
                    clearProvisionalLoad();
                else if (m_provisionalDocumentLoader) {
                    KURL unreachableURL = m_provisionalDocumentLoader->unreachableURL();
                    if (!unreachableURL.isEmpty() && unreachableURL == pdl->request().url())
                        shouldReset = false;
                }
            }
            if (shouldReset && item)
                if (Page* page = m_frame->page()) {
                    page->backForwardList()->goToItem(item.get());
                    Settings* settings = m_frame->settings();
                    page->setGlobalHistoryItem((!settings || settings->privateBrowsingEnabled()) ? 0 : item.get());
                }
            return;
        }
        
        case FrameStateCommittedPage: {
            DocumentLoader* dl = m_documentLoader.get();            
            if (!dl || (dl->isLoadingInAPISense() && !dl->isStopping()))
                return;

            markLoadComplete();


            m_client->forceLayoutForNonHTML();
             
            if (Page* page = m_frame->page())
                if ((isBackForwardLoadType(m_loadType) || m_loadType == FrameLoadTypeReload || m_loadType == FrameLoadTypeReloadFromOrigin) && page->backForwardList())
                    history()->restoreScrollPositionAndViewState();

            if (m_creatingInitialEmptyDocument || !m_committedFirstRealDocumentLoad)
                return;

            const ResourceError& error = dl->mainDocumentError();
#ifndef NDEBUG
            m_didDispatchDidCommitLoad = false;
#endif
            if (!error.isNull())
                m_client->dispatchDidFailLoad(error);
            else
                m_client->dispatchDidFinishLoad();

            if (Page* page = m_frame->page())
                page->progress()->progressCompleted(m_frame);
            return;
        }
        
        case FrameStateComplete:
            frameLoadCompleted();
            return;
    }

    ASSERT_NOT_REACHED();
}
