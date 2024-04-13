const ResourceRequest& FrameLoader::originalRequest() const
{
    return activeDocumentLoader()->originalRequestCopy();
}
