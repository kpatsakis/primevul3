void FrameLoader::setFirstPartyForCookies(const KURL& url)
{
    m_frame->document()->setFirstPartyForCookies(url);
    for (Frame* child = m_frame->tree()->firstChild(); child; child = child->tree()->nextSibling())
        child->loader()->setFirstPartyForCookies(url);
}
