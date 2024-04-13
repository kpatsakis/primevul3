void FrameLoader::reportLocalLoadFailed(Frame* frame, const String& url)
{
    ASSERT(!url.isEmpty());
    if (!frame)
        return;

    frame->domWindow()->console()->addMessage(JSMessageSource, LogMessageType, ErrorMessageLevel, "Not allowed to load local resource: " + url, 0, String());
}
