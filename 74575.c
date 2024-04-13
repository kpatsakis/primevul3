const ResourceRequest& FrameLoader::initialRequest() const
{
    return activeDocumentLoader()->originalRequest();
}
