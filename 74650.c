bool FrameLoader::shouldUseCredentialStorage(ResourceLoader* loader)
{
    return m_client->shouldUseCredentialStorage(loader->documentLoader(), loader->identifier());
}
