unsigned long FrameLoader::loadResourceSynchronously(const ResourceRequest& request, StoredCredentials storedCredentials, ResourceError& error, ResourceResponse& response, Vector<char>& data)
{
    String referrer = m_outgoingReferrer;
    if (SecurityOrigin::shouldHideReferrer(request.url(), referrer))
        referrer = String();
    
    ResourceRequest initialRequest = request;
    initialRequest.setTimeoutInterval(10);
    
    if (initialRequest.isConditional())
        initialRequest.setCachePolicy(ReloadIgnoringCacheData);
    else
        initialRequest.setCachePolicy(documentLoader()->request().cachePolicy());
    
    if (!referrer.isEmpty())
        initialRequest.setHTTPReferrer(referrer);
    addHTTPOriginIfNeeded(initialRequest, outgoingOrigin());

    if (Page* page = m_frame->page())
        initialRequest.setFirstPartyForCookies(page->mainFrame()->loader()->documentLoader()->request().url());
    initialRequest.setHTTPUserAgent(client()->userAgent(request.url()));

    unsigned long identifier = 0;    
    ResourceRequest newRequest(initialRequest);
    requestFromDelegate(newRequest, identifier, error);

    if (error.isNull()) {
        ASSERT(!newRequest.isNull());
        
#if ENABLE(OFFLINE_WEB_APPLICATIONS)
        if (!documentLoader()->applicationCacheHost()->maybeLoadSynchronously(newRequest, error, response, data)) {
#endif
            ResourceHandle::loadResourceSynchronously(newRequest, storedCredentials, error, response, data, m_frame);
#if ENABLE(OFFLINE_WEB_APPLICATIONS)
            documentLoader()->applicationCacheHost()->maybeLoadFallbackSynchronously(newRequest, error, response, data);
        }
#endif
    }

    notifier()->sendRemainingDelegateMessages(m_documentLoader.get(), identifier, response, data.size(), error);
    return identifier;
}
