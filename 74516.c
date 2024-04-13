bool FrameLoader::canCachePageContainingThisFrame()
{
    for (Frame* child = m_frame->tree()->firstChild(); child; child = child->tree()->nextSibling()) {
        if (!child->loader()->canCachePageContainingThisFrame())
            return false;
    }
            
    return m_documentLoader
        && m_documentLoader->mainDocumentError().isNull()
        && !m_containsPlugIns
        && !m_URL.protocolIs("https")
#ifndef PAGE_CACHE_ACCEPTS_UNLOAD_HANDLERS
        && (!m_frame->domWindow() || !m_frame->domWindow()->hasEventListeners(eventNames().unloadEvent))
#endif
#if ENABLE(DATABASE)
        && !m_frame->document()->hasOpenDatabases()
#endif
#if ENABLE(SHARED_WORKERS)
        && !SharedWorkerRepository::hasSharedWorkers(m_frame->document())
#endif
        && !m_frame->document()->usingGeolocation()
        && history()->currentItem()
        && !m_quickRedirectComing
        && !m_documentLoader->isLoadingInAPISense()
        && !m_documentLoader->isStopping()
        && m_frame->document()->canSuspendActiveDOMObjects()
#if ENABLE(OFFLINE_WEB_APPLICATIONS)
        && m_documentLoader->applicationCacheHost()->canCacheInPageCache()
#endif
#if ENABLE(WML)
        && !frameContainsWMLContent(m_frame)
#endif
        && m_client->canCachePage()
        ;
}
