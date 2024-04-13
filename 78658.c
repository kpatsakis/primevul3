void FrameLoader::didBeginDocument(bool dispatch)
{
    m_isComplete = false;
    m_frame->document()->setReadyState(Document::Loading);

    if (history()->currentItem() && m_loadType == FrameLoadTypeBackForward)
        m_frame->document()->statePopped(history()->currentItem()->stateObject());

    if (dispatch)
        dispatchDidClearWindowObjectsInAllWorlds();

    m_frame->document()->initContentSecurityPolicy(m_documentLoader ? ContentSecurityPolicyResponseHeaders(m_documentLoader->response()) : ContentSecurityPolicyResponseHeaders());

    Settings* settings = m_frame->document()->settings();
    if (settings) {
        m_frame->document()->fetcher()->setImagesEnabled(settings->areImagesEnabled());
        m_frame->document()->fetcher()->setAutoLoadImages(settings->loadsImagesAutomatically());
    }

    if (m_documentLoader) {
        String dnsPrefetchControl = m_documentLoader->response().httpHeaderField("X-DNS-Prefetch-Control");
        if (!dnsPrefetchControl.isEmpty())
            m_frame->document()->parseDNSPrefetchControlHeader(dnsPrefetchControl);

        String headerContentLanguage = m_documentLoader->response().httpHeaderField("Content-Language");
        if (!headerContentLanguage.isEmpty()) {
            size_t commaIndex = headerContentLanguage.find(',');
            headerContentLanguage.truncate(commaIndex); // notFound == -1 == don't truncate
            headerContentLanguage = headerContentLanguage.stripWhiteSpace(isHTMLSpace);
            if (!headerContentLanguage.isEmpty())
                m_frame->document()->setContentLanguage(headerContentLanguage);
        }
    }

    history()->restoreDocumentState();
}
