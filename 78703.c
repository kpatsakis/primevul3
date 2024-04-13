void FrameLoader::updateForSameDocumentNavigation(const KURL& newURL, SameDocumentNavigationSource sameDocumentNavigationSource, PassRefPtr<SerializedScriptValue> data, const String& title, UpdateBackForwardListPolicy updateBackForwardList)
{
    KURL oldURL = m_frame->document()->url();
    m_frame->document()->setURL(newURL);
    setOutgoingReferrer(newURL);
    documentLoader()->replaceRequestURLForSameDocumentNavigation(newURL);

    if (updateBackForwardList == UpdateBackForwardList)
        history()->updateBackForwardListForFragmentScroll();

    if (sameDocumentNavigationSource == SameDocumentNavigationDefault)
        history()->updateForSameDocumentNavigation();
    else if (sameDocumentNavigationSource == SameDocumentNavigationPushState)
        history()->pushState(data, title, newURL.string());
    else if (sameDocumentNavigationSource == SameDocumentNavigationReplaceState)
        history()->replaceState(data, title, newURL.string());
    else
        ASSERT_NOT_REACHED();

    if (m_frame->document()->loadEventFinished())
        m_client->postProgressStartedNotification();

    m_documentLoader->clearRedirectChain();
    if (m_documentLoader->isClientRedirect())
        m_documentLoader->appendRedirect(oldURL);
    m_documentLoader->appendRedirect(newURL);

    m_client->dispatchDidNavigateWithinPage();
    m_client->dispatchDidReceiveTitle(m_frame->document()->title());

    if (m_frame->document()->loadEventFinished())
        m_client->postProgressFinishedNotification();
}
