void FrameLoader::addExtraFieldsToRequest(ResourceRequest& request, FrameLoadType loadType, bool mainResource, bool cookiePolicyURLFromRequest)
{
    if (request.firstPartyForCookies().isEmpty()) {
        if (mainResource && (isLoadingMainFrame() || cookiePolicyURLFromRequest))
            request.setFirstPartyForCookies(request.url());
        else if (Document* document = m_frame->document())
            request.setFirstPartyForCookies(document->firstPartyForCookies());
    }

    if (!request.url().isEmpty() && !request.url().protocolInHTTPFamily())
        return;

    applyUserAgent(request);
    
    if (loadType == FrameLoadTypeReload) {
        request.setCachePolicy(ReloadIgnoringCacheData);
        request.setHTTPHeaderField("Cache-Control", "max-age=0");
    } else if (loadType == FrameLoadTypeReloadFromOrigin) {
        request.setCachePolicy(ReloadIgnoringCacheData);
        request.setHTTPHeaderField("Cache-Control", "no-cache");
        request.setHTTPHeaderField("Pragma", "no-cache");
    }
    
    if (mainResource)
        request.setHTTPAccept(defaultAcceptHeader);

    addHTTPOriginIfNeeded(request, String());

    Settings* settings = m_frame->settings();
    request.setResponseContentDispositionEncodingFallbackArray("UTF-8", m_URL.isEmpty() ? m_encoding : encoding(), settings ? settings->defaultTextEncodingName() : String());
}
