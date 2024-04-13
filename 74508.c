void FrameLoader::begin(const KURL& url, bool dispatch, SecurityOrigin* origin)
{
    RefPtr<SecurityOrigin> forcedSecurityOrigin = origin;

    RefPtr<Document> document;

    if (!m_isDisplayingInitialEmptyDocument && m_client->shouldUsePluginDocument(m_responseMIMEType))
        document = PluginDocument::create(m_frame);
    else if (!m_client->hasHTMLView())
        document = PlaceholderDocument::create(m_frame);
    else
        document = DOMImplementation::createDocument(m_responseMIMEType, m_frame, m_frame->inViewSourceMode());

    bool resetScripting = !(m_isDisplayingInitialEmptyDocument && m_frame->document()->securityOrigin()->isSecureTransitionTo(url));
    clear(resetScripting, resetScripting);
    if (resetScripting)
        m_frame->script()->updatePlatformScriptObjects();

    m_needsClear = true;
    m_isComplete = false;
    m_didCallImplicitClose = false;
    m_isLoadingMainResource = true;
    m_isDisplayingInitialEmptyDocument = m_creatingInitialEmptyDocument;

    KURL ref(url);
    ref.setUser(String());
    ref.setPass(String());
    ref.removeFragmentIdentifier();
    m_outgoingReferrer = ref.string();
    m_URL = url;

    document->setURL(m_URL);
    m_frame->setDocument(document);

    if (m_decoder)
        document->setDecoder(m_decoder.get());
    if (forcedSecurityOrigin)
        document->setSecurityOrigin(forcedSecurityOrigin.get());

    m_frame->domWindow()->setURL(document->url());
    m_frame->domWindow()->setSecurityOrigin(document->securityOrigin());

    if (dispatch)
        dispatchWindowObjectAvailable();
    
    updateFirstPartyForCookies();

    Settings* settings = document->settings();
    document->docLoader()->setAutoLoadImages(settings && settings->loadsImagesAutomatically());

    if (m_documentLoader) {
        String dnsPrefetchControl = m_documentLoader->response().httpHeaderField("X-DNS-Prefetch-Control");
        if (!dnsPrefetchControl.isEmpty())
            document->parseDNSPrefetchControlHeader(dnsPrefetchControl);
    }

    history()->restoreDocumentState();

    document->implicitOpen();
    
    if (m_frame->view() && m_client->hasHTMLView())
        m_frame->view()->setContentsSize(IntSize());
}
