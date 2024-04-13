void FrameLoader::requestFromDelegate(ResourceRequest& request, unsigned long& identifier, ResourceError& error)
{
    ASSERT(!request.isNull());

    identifier = 0;
    if (Page* page = m_frame->page()) {
        identifier = page->progress()->createUniqueIdentifier();
        notifier()->assignIdentifierToInitialRequest(identifier, m_documentLoader.get(), request);
    }

    ResourceRequest newRequest(request);
    notifier()->dispatchWillSendRequest(m_documentLoader.get(), identifier, newRequest, ResourceResponse());

    if (newRequest.isNull())
        error = cancelledError(request);
    else
        error = ResourceError();

    request = newRequest;
}
