void FrameLoader::setMainDocumentError(DocumentLoader* loader, const ResourceError& error)
{
    m_client->setMainDocumentError(loader, error);
}
