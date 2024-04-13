void FrameLoader::requestFromDelegate(ResourceRequest& request, unsigned long& identifier, ResourceError& error)
{
    ASSERT(!request.isNull());

    identifier = 0;
    if (m_frame->page())
        identifier = createUniqueIdentifier();

    ResourceRequest newRequest(request);
    notifier()->dispatchWillSendRequest(m_documentLoader.get(), identifier, newRequest, ResourceResponse());

    if (newRequest.isNull())
        error = ResourceError::cancelledError(request.url());
    else
        error = ResourceError();

    request = newRequest;
}
