void DocumentWriter::begin(const KURL& urlReference, bool dispatch, SecurityOrigin* origin)
{
    RefPtr<SecurityOrigin> forcedSecurityOrigin = origin;

    KURL url = urlReference;

    RefPtr<Document> document = createDocument(url);
    
    if (document->isPluginDocument() && m_frame->loader()->isSandboxed(SandboxPlugins))
        document = SinkDocument::create(m_frame, url);


    bool resetScripting = !(m_frame->loader()->stateMachine()->isDisplayingInitialEmptyDocument() && m_frame->document()->securityOrigin()->isSecureTransitionTo(url));
    m_frame->loader()->clear(resetScripting, resetScripting);
    clear();
    if (resetScripting)
        m_frame->script()->updatePlatformScriptObjects();

    m_frame->loader()->setOutgoingReferrer(url);
    m_frame->setDocument(document);

    if (m_decoder)
        document->setDecoder(m_decoder.get());
    if (forcedSecurityOrigin)
        document->setSecurityOrigin(forcedSecurityOrigin.get());

    m_frame->domWindow()->setURL(document->url());
    m_frame->domWindow()->setSecurityOrigin(document->securityOrigin());

    m_frame->loader()->didBeginDocument(dispatch);

    document->implicitOpen();

    m_parser = document->parser();

    if (m_frame->view() && m_frame->loader()->client()->hasHTMLView())
        m_frame->view()->setContentsSize(IntSize());
}
