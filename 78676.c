unsigned long FrameLoader::loadResourceSynchronously(const ResourceRequest& request, StoredCredentials storedCredentials, ResourceError& error, ResourceResponse& response, Vector<char>& data)
{
    ASSERT(m_frame->document());
    String referrer = SecurityPolicy::generateReferrerHeader(m_frame->document()->referrerPolicy(), request.url(), outgoingReferrer());

    ResourceRequest initialRequest = request;
    initialRequest.setTimeoutInterval(10);

    if (!referrer.isEmpty())
        initialRequest.setHTTPReferrer(referrer);
    addHTTPOriginIfNeeded(initialRequest, outgoingOrigin());

    addExtraFieldsToRequest(initialRequest);

    unsigned long identifier = 0;
    ResourceRequest newRequest(initialRequest);
    requestFromDelegate(newRequest, identifier, error);

    if (error.isNull()) {
        ASSERT(!newRequest.isNull());
        documentLoader()->applicationCacheHost()->willStartLoadingSynchronously(newRequest);
        ResourceLoader::loadResourceSynchronously(newRequest, storedCredentials, error, response, data);
    }
    int encodedDataLength = response.resourceLoadInfo() ? static_cast<int>(response.resourceLoadInfo()->encodedDataLength) : -1;
    notifier()->sendRemainingDelegateMessages(m_documentLoader.get(), identifier, response, data.data(), data.size(), encodedDataLength, error);
    return identifier;
}
