void FrameLoader::setOutgoingReferrer(const KURL& url)
{
    m_outgoingReferrer = url.strippedForUseAsReferrer();
}
