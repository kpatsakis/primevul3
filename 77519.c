Evas_Object* kitFrame(const WebCore::Frame* coreFrame)
{
    if (!coreFrame)
        return 0;

    WebCore::FrameLoaderClientEfl* frameLoaderClient = _ewk_frame_loader_efl_get(coreFrame);
    if (!frameLoaderClient)
        return 0;

    return frameLoaderClient->webFrame();
}
