void FrameLoader::stopLoading(UnloadEventPolicy unloadEventPolicy, DatabasePolicy databasePolicy)
{
    if (m_frame->document() && m_frame->document()->tokenizer())
        m_frame->document()->tokenizer()->stopParsing();

    if (unloadEventPolicy != UnloadEventPolicyNone) {
        if (m_frame->document()) {
            if (m_didCallImplicitClose && !m_wasUnloadEventEmitted) {
                Node* currentFocusedNode = m_frame->document()->focusedNode();
                if (currentFocusedNode)
                    currentFocusedNode->aboutToUnload();
                m_unloadEventBeingDispatched = true;
                if (m_frame->domWindow()) {
                    if (unloadEventPolicy == UnloadEventPolicyUnloadAndPageHide)
                        m_frame->domWindow()->dispatchEvent(PageTransitionEvent::create(EventNames().pagehideEvent, m_frame->document()->inPageCache()), m_frame->document());
                    if (!m_frame->document()->inPageCache())
                        m_frame->domWindow()->dispatchEvent(Event::create(eventNames().unloadEvent, false, false), m_frame->domWindow()->document());
                }
                m_unloadEventBeingDispatched = false;
                if (m_frame->document())
                    m_frame->document()->updateStyleIfNeeded();
                m_wasUnloadEventEmitted = true;
            }
        }

        if (m_frame->document() && !m_frame->document()->inPageCache()) {
            bool keepEventListeners = m_isDisplayingInitialEmptyDocument && m_provisionalDocumentLoader
                && m_frame->document()->securityOrigin()->isSecureTransitionTo(m_provisionalDocumentLoader->url());

            if (!keepEventListeners)
                m_frame->document()->removeAllEventListeners();
        }
    }

    m_isComplete = true; // to avoid calling completed() in finishedParsing()
    m_isLoadingMainResource = false;
    m_didCallImplicitClose = true; // don't want that one either

    if (m_frame->document() && m_frame->document()->parsing()) {
        finishedParsing();
        m_frame->document()->setParsing(false);
    }
  
    m_workingURL = KURL();

    if (Document* doc = m_frame->document()) {
        if (DocLoader* docLoader = doc->docLoader())
            cache()->loader()->cancelRequests(docLoader);

#if ENABLE(DATABASE)
        if (databasePolicy == DatabasePolicyStop)
            doc->stopDatabases();
#endif
    }

    for (Frame* child = m_frame->tree()->firstChild(); child; child = child->tree()->nextSibling())
        child->loader()->stopLoading(unloadEventPolicy);

    m_frame->redirectScheduler()->cancel();
}
