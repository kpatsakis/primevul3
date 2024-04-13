CachePolicy FrameLoader::subresourceCachePolicy() const
{
    if (m_frame->document()->loadEventFinished())
        return CachePolicyVerify;

    if (m_loadType == FrameLoadTypeReloadFromOrigin)
        return CachePolicyReload;

    if (Frame* parentFrame = m_frame->tree()->parent()) {
        CachePolicy parentCachePolicy = parentFrame->loader()->subresourceCachePolicy();
        if (parentCachePolicy != CachePolicyVerify)
            return parentCachePolicy;
    }

    if (m_loadType == FrameLoadTypeReload)
        return CachePolicyRevalidate;

    const ResourceRequest& request(documentLoader()->request());

    if (request.cachePolicy() == ReturnCacheDataElseLoad)
        return CachePolicyHistoryBuffer;

    return CachePolicyVerify;
}
