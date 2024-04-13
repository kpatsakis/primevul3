void FrameLoader::updateFirstPartyForCookies()
{
    if (m_frame->tree()->parent())
        setFirstPartyForCookies(m_frame->tree()->parent()->document()->firstPartyForCookies());
    else
        setFirstPartyForCookies(m_URL);
}
