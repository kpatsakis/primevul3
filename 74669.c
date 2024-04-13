void FrameLoader::willChangeTitle(DocumentLoader* loader)
{
    m_client->willChangeTitle(loader);
}
