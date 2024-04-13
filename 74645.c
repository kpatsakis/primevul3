bool FrameLoader::shouldInterruptLoadForXFrameOptions(const String& content, const KURL& url)
{
    Frame* topFrame = m_frame->tree()->top();
    if (m_frame == topFrame)
        return false;

    if (equalIgnoringCase(content, "deny"))
        return true;

    if (equalIgnoringCase(content, "sameorigin")) {
        RefPtr<SecurityOrigin> origin = SecurityOrigin::create(url);
        if (!origin->isSameSchemeHostPort(topFrame->document()->securityOrigin()))
            return true;
    }

    return false;
}
