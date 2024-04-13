KURL FrameLoader::iconURL()
{
    if (m_frame->tree() && m_frame->tree()->parent())
        return KURL();

    if (!m_frame->document()->iconURL().isEmpty())
        return KURL(ParsedURLString, m_frame->document()->iconURL());

    if (!m_URL.protocolInHTTPFamily())
        return KURL();

    KURL url;
    url.setProtocol(m_URL.protocol());
    url.setHost(m_URL.host());
    if (int port = m_URL.port())
        url.setPort(port);
    url.setPath("/favicon.ico");
    return url;
}
