void FrameLoader::loadedResourceFromMemoryCache(Resource* resource)
{
    Page* page = m_frame->page();
    if (!page)
        return;

    if (!resource->shouldSendResourceLoadCallbacks())
        return;

    if (resource->type() == Resource::MainResource)
        return;

    ResourceRequest request(resource->url());
    m_client->dispatchDidLoadResourceFromMemoryCache(m_documentLoader.get(), request, resource->response(), resource->encodedSize());

    unsigned long identifier;
    ResourceError error;
    requestFromDelegate(request, identifier, error);
    InspectorInstrumentation::markResourceAsCached(page, identifier);
    notifier()->sendRemainingDelegateMessages(m_documentLoader.get(), identifier, resource->response(), 0, resource->encodedSize(), 0, error);
}
