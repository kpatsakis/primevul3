void FrameLoader::open(CachedFrameBase& cachedFrame)
{
    m_isComplete = false;
    
    m_didCallImplicitClose = true;

    KURL url = cachedFrame.url();

    if (url.protocolInHTTPFamily() && !url.host().isEmpty() && url.path().isEmpty())
        url.setPath("/");
    
    m_URL = url;
    m_workingURL = url;

    started();
    clear(true, true, cachedFrame.isMainFrame());

    Document* document = cachedFrame.document();
    ASSERT(document);
    document->setInPageCache(false);

    m_needsClear = true;
    m_isComplete = false;
    m_didCallImplicitClose = false;
    m_outgoingReferrer = url.string();

    FrameView* view = cachedFrame.view();
    
    ASSERT(view);
    view->setWasScrolledByUser(false);

    if (m_frame->view())
        view->setFrameRect(m_frame->view()->frameRect());
    m_frame->setView(view);
    
    m_frame->setDocument(document);
    m_frame->setDOMWindow(cachedFrame.domWindow());
    m_frame->domWindow()->setURL(document->url());
    m_frame->domWindow()->setSecurityOrigin(document->securityOrigin());

    m_decoder = document->decoder();

    updateFirstPartyForCookies();

    cachedFrame.restore();
}
