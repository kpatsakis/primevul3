void FrameLoader::didChangeTitle(DocumentLoader* loader)
{
    m_client->didChangeTitle(loader);

    if (loader == m_documentLoader) {
        history()->setCurrentItemTitle(loader->title());
        m_client->setTitle(loader->title(), loader->urlForHistory());
        m_client->setMainFrameDocumentReady(true); // update observers with new DOMDocument
        m_client->dispatchDidReceiveTitle(loader->title());
    }
}
