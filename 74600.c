void FrameLoader::loadedResourceFromMemoryCache(const CachedResource* resource)
{
    Page* page = m_frame->page();
    if (!page)
        return;

#if ENABLE(INSPECTOR)
    page->inspectorController()->didLoadResourceFromMemoryCache(m_documentLoader.get(), resource);
#endif

    if (!resource->sendResourceLoadCallbacks() || m_documentLoader->haveToldClientAboutLoad(resource->url()))
        return;

    if (!page->areMemoryCacheClientCallsEnabled()) {
        m_documentLoader->recordMemoryCacheLoadForFutureClientNotification(resource->url());
        m_documentLoader->didTellClientAboutLoad(resource->url());
        return;
    }

    ResourceRequest request(resource->url());
    if (m_client->dispatchDidLoadResourceFromMemoryCache(m_documentLoader.get(), request, resource->response(), resource->encodedSize())) {
        m_documentLoader->didTellClientAboutLoad(resource->url());
        return;
    }

    unsigned long identifier;
    ResourceError error;
    requestFromDelegate(request, identifier, error);
    notifier()->sendRemainingDelegateMessages(m_documentLoader.get(), identifier, resource->response(), resource->encodedSize(), error);
}
