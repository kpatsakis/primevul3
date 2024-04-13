void FrameLoader::addExtraFieldsToRequest(ResourceRequest& request)
{
    bool isMainResource = (request.targetType() == ResourceRequest::TargetIsMainFrame) || (request.targetType() == ResourceRequest::TargetIsSubframe);

    if (isMainResource && isLoadingMainFrame())
        request.setFirstPartyForCookies(request.url());
    else
        request.setFirstPartyForCookies(m_frame->document()->firstPartyForCookies());

    if (!request.url().isEmpty() && !request.url().protocolIsInHTTPFamily())
        return;

    applyUserAgent(request);

    if (request.cachePolicy() == ReloadIgnoringCacheData) {
        if (m_loadType == FrameLoadTypeReload)
            request.setHTTPHeaderField("Cache-Control", "max-age=0");
        else if (m_loadType == FrameLoadTypeReloadFromOrigin) {
            request.setHTTPHeaderField("Cache-Control", "no-cache");
            request.setHTTPHeaderField("Pragma", "no-cache");
        }
    }

    if (isMainResource)
        request.setHTTPAccept(defaultAcceptHeader);

    addHTTPOriginIfNeeded(request, String());
}
