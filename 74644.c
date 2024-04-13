bool FrameLoader::shouldAllowNavigation(Frame* targetFrame) const
{

    if (!targetFrame)
        return true;

    if (m_frame == targetFrame)
        return true;

    if (targetFrame == m_frame->tree()->top())
        return true;

    if (!targetFrame->tree()->parent() && m_frame->loader()->opener() == targetFrame)
        return true;

    Document* activeDocument = m_frame->document();
    ASSERT(activeDocument);
    const SecurityOrigin* activeSecurityOrigin = activeDocument->securityOrigin();

    if (!targetFrame->tree()->parent() && canAccessAncestor(activeSecurityOrigin, targetFrame->loader()->opener()))
        return true;

    if (canAccessAncestor(activeSecurityOrigin, targetFrame))
        return true;

    Settings* settings = targetFrame->settings();
    if (settings && !settings->privateBrowsingEnabled()) {
        Document* targetDocument = targetFrame->document();
        String message = String::format("Unsafe JavaScript attempt to initiate a navigation change for frame with URL %s from frame with URL %s.\n",
            targetDocument->url().string().utf8().data(), activeDocument->url().string().utf8().data());

        targetFrame->domWindow()->console()->addMessage(JSMessageSource, LogMessageType, ErrorMessageLevel, message, 1, String());
    }
    
    return false;
}
