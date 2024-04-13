void FrameLoader::changeLocation(const KURL& url, const String& referrer, bool lockHistory, bool lockBackForwardList, bool userGesture, bool refresh)
{
    RefPtr<Frame> protect(m_frame);

    ResourceRequest request(url, referrer, refresh ? ReloadIgnoringCacheData : UseProtocolCachePolicy);
    
    if (m_frame->script()->executeIfJavaScriptURL(request.url(), userGesture))
        return;

    urlSelected(request, "_self", 0, lockHistory, lockBackForwardList, userGesture, SendReferrer);
}
