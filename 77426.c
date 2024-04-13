static WebCore::FrameLoaderClientEfl* _ewk_frame_loader_efl_get(const WebCore::Frame* frame)
{
    return static_cast<WebCore::FrameLoaderClientEfl*>(frame->loader()->client());
}
