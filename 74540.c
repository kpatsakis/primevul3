KURL FrameLoader::completeURL(const String& url)
{
    ASSERT(m_frame->document());
    return m_frame->document()->completeURL(url);
}
