bool FrameLoader::logCanCacheFrameDecision(int indentLevel)
{
    if (m_creatingInitialEmptyDocument)
        return false;
    KURL currentURL = m_documentLoader ? m_documentLoader->url() : KURL();
    if (currentURL.isEmpty())
        return false;

    PCLOG("+---");
    KURL newURL = m_provisionalDocumentLoader ? m_provisionalDocumentLoader->url() : KURL();
    if (!newURL.isEmpty())
        PCLOG(" Determining if frame can be cached navigating from (%s) to (%s):", currentURL.string().utf8().data(), newURL.string().utf8().data());
    else
        PCLOG(" Determining if subframe with URL (%s) can be cached:", currentURL.string().utf8().data());
        
    bool cannotCache = false;

    do {
        if (!m_documentLoader) {
            PCLOG("   -There is no DocumentLoader object");
            cannotCache = true;
            break;
        }
        if (!m_documentLoader->mainDocumentError().isNull())
            { PCLOG("   -Main document has an error"); cannotCache = true; }
        if (m_containsPlugIns)
            { PCLOG("   -Frame contains plugins"); cannotCache = true; }
        if (m_URL.protocolIs("https"))
            { PCLOG("   -Frame is HTTPS"); cannotCache = true; }
#ifndef PAGE_CACHE_ACCEPTS_UNLOAD_HANDLERS
        if (m_frame->domWindow() && m_frame->domWindow()->hasEventListeners(eventNames().unloadEvent))
            { PCLOG("   -Frame has an unload event listener"); cannotCache = true; }
#endif
#if ENABLE(DATABASE)
        if (m_frame->document()->hasOpenDatabases())
            { PCLOG("   -Frame has open database handles"); cannotCache = true; }
#endif
#if ENABLE(SHARED_WORKERS)
        if (SharedWorkerRepository::hasSharedWorkers(m_frame->document()))
            { PCLOG("   -Frame has associated SharedWorkers"); cannotCache = true; }
#endif
        if (m_frame->document()->usingGeolocation())
            { PCLOG("   -Frame uses Geolocation"); cannotCache = true; }
        if (!history()->currentItem())
            { PCLOG("   -No current history item"); cannotCache = true; }
        if (m_quickRedirectComing)
            { PCLOG("   -Quick redirect is coming"); cannotCache = true; }
        if (m_documentLoader->isLoadingInAPISense())
            { PCLOG("   -DocumentLoader is still loading in API sense"); cannotCache = true; }
        if (m_documentLoader->isStopping())
            { PCLOG("   -DocumentLoader is in the middle of stopping"); cannotCache = true; }
        if (!m_frame->document()->canSuspendActiveDOMObjects())
            { PCLOG("   -The document cannot suspect its active DOM Objects"); cannotCache = true; }
#if ENABLE(OFFLINE_WEB_APPLICATIONS)
        if (!m_documentLoader->applicationCacheHost()->canCacheInPageCache())
            { PCLOG("   -The DocumentLoader uses an application cache"); cannotCache = true; }
#endif
        if (!m_client->canCachePage())
            { PCLOG("   -The client says this frame cannot be cached"); cannotCache = true; }
    } while (false);

    for (Frame* child = m_frame->tree()->firstChild(); child; child = child->tree()->nextSibling())
        if (!child->loader()->logCanCacheFrameDecision(indentLevel + 1))
            cannotCache = true;
    
    PCLOG(cannotCache ? " Frame CANNOT be cached" : " Frame CAN be cached");
    PCLOG("+---");

    return !cannotCache;
}
