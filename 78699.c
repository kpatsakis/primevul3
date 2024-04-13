bool FrameLoader::shouldTreatURLAsSrcdocDocument(const KURL& url) const
{
    if (!equalIgnoringCase(url.string(), "about:srcdoc"))
        return false;
    HTMLFrameOwnerElement* ownerElement = m_frame->ownerElement();
    if (!ownerElement)
        return false;
    if (!ownerElement->hasTagName(iframeTag))
        return false;
    return ownerElement->fastHasAttribute(srcdocAttr);
}
