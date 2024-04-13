void FrameLoader::committedLoad(DocumentLoader* loader, const char* data, int length)
{
    if (ArchiveFactory::isArchiveMimeType(loader->response().mimeType()))
        return;
    m_client->committedLoad(loader, data, length);
}
