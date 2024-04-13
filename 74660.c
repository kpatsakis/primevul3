CachePolicy FrameLoader::subresourceCachePolicy() const
{
    if (m_isComplete)
        return CachePolicyVerify;

    if (m_loadType == FrameLoadTypeReloadFromOrigin)
        return CachePolicyReload;

    if (Frame* parentFrame = m_frame->tree()->parent()) {
        CachePolicy parentCachePolicy = parentFrame->loader()->subresourceCachePolicy();
        if (parentCachePolicy != CachePolicyVerify)
            return parentCachePolicy;
    }

    const ResourceRequest& request(documentLoader()->request());
    if (request.cachePolicy() == ReloadIgnoringCacheData && !equalIgnoringCase(request.httpMethod(), "post"))
        return CachePolicyRevalidate;

    if (m_loadType == FrameLoadTypeReload)
        return CachePolicyRevalidate;

    return CachePolicyVerify;
}
