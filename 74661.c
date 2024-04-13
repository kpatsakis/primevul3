void FrameLoader::tellClientAboutPastMemoryCacheLoads()
{
    ASSERT(m_frame->page());
    ASSERT(m_frame->page()->areMemoryCacheClientCallsEnabled());

    if (!m_documentLoader)
        return;

    Vector<String> pastLoads;
    m_documentLoader->takeMemoryCacheLoadsForClientNotification(pastLoads);

    size_t size = pastLoads.size();
    for (size_t i = 0; i < size; ++i) {
        CachedResource* resource = cache()->resourceForURL(pastLoads[i]);

        if (!resource)
            continue;

        ResourceRequest request(resource->url());
        m_client->dispatchDidLoadResourceFromMemoryCache(m_documentLoader.get(), request, resource->response(), resource->encodedSize());
    }
}
