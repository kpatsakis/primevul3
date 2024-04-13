PassRefPtr<Document> DocumentWriter::createDocument(const KURL& url)
{
    if (!m_frame->loader()->stateMachine()->isDisplayingInitialEmptyDocument() && m_frame->loader()->client()->shouldUsePluginDocument(m_mimeType))
        return PluginDocument::create(m_frame, url);
    if (!m_frame->loader()->client()->hasHTMLView())
        return PlaceholderDocument::create(m_frame, url);
    return DOMImplementation::createDocument(m_mimeType, m_frame, url, m_frame->inViewSourceMode());
}
