bool FrameLoader::requestFrame(HTMLFrameOwnerElement* ownerElement, const String& urlString, const AtomicString& frameName)
{
    KURL scriptURL;
    KURL url;
    if (protocolIsJavaScript(urlString)) {
        scriptURL = completeURL(urlString); // completeURL() encodes the URL.
        url = blankURL();
    } else
        url = completeURL(urlString);

    Frame* frame = ownerElement->contentFrame();
    if (frame)
        frame->redirectScheduler()->scheduleLocationChange(url.string(), m_outgoingReferrer, true, true, isProcessingUserGesture());
    else
        frame = loadSubframe(ownerElement, url, frameName, m_outgoingReferrer);
    
    if (!frame)
        return false;

    if (!scriptURL.isEmpty())
        frame->script()->executeIfJavaScriptURL(scriptURL);

    return true;
}
