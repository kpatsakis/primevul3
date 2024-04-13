bool FrameLoader::prepareRequestForThisFrame(FrameLoadRequest& request)
{
    if (!request.requester())
        return true;

    KURL url = request.resourceRequest().url();
    if (m_frame->script()->executeScriptIfJavaScriptURL(url))
        return false;

    if (!request.requester()->canDisplay(url)) {
        reportLocalLoadFailed(m_frame, url.elidedString());
        return false;
    }

    if (request.requester() && !request.formState() && request.frameName().isEmpty())
        request.setFrameName(m_frame->document()->baseTarget());

    if (request.requester() == m_frame->document()->securityOrigin())
        setReferrerForFrameRequest(request.resourceRequest(), request.shouldSendReferrer());
    return true;
}
