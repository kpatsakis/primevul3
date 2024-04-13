bool FrameLoader::shouldInterruptLoadForXFrameOptions(const String& content, const KURL& url, unsigned long requestIdentifier)
{
    UseCounter::count(m_frame->document(), UseCounter::XFrameOptions);

    Frame* topFrame = m_frame->tree()->top();
    if (m_frame == topFrame)
        return false;

    XFrameOptionsDisposition disposition = parseXFrameOptionsHeader(content);

    switch (disposition) {
    case XFrameOptionsSameOrigin: {
        UseCounter::count(m_frame->document(), UseCounter::XFrameOptionsSameOrigin);
        RefPtr<SecurityOrigin> origin = SecurityOrigin::create(url);
        if (!origin->isSameSchemeHostPort(topFrame->document()->securityOrigin()))
            return true;
        for (Frame* frame = m_frame->tree()->parent(); frame; frame = frame->tree()->parent()) {
            if (!origin->isSameSchemeHostPort(frame->document()->securityOrigin())) {
                UseCounter::count(m_frame->document(), UseCounter::XFrameOptionsSameOriginWithBadAncestorChain);
                break;
            }
        }
        return false;
    }
    case XFrameOptionsDeny:
        return true;
    case XFrameOptionsAllowAll:
        return false;
    case XFrameOptionsConflict:
        m_frame->document()->addConsoleMessageWithRequestIdentifier(JSMessageSource, ErrorMessageLevel, "Multiple 'X-Frame-Options' headers with conflicting values ('" + content + "') encountered when loading '" + url.elidedString() + "'. Falling back to 'DENY'.", requestIdentifier);
        return true;
    case XFrameOptionsInvalid:
        m_frame->document()->addConsoleMessageWithRequestIdentifier(JSMessageSource, ErrorMessageLevel, "Invalid 'X-Frame-Options' header encountered when loading '" + url.elidedString() + "': '" + content + "' is not a recognized directive. The header will be ignored.", requestIdentifier);
        return false;
    default:
        ASSERT_NOT_REACHED();
        return false;
    }
}
