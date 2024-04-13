bool ewk_frame_init(Evas_Object* ewkFrame, Evas_Object* view, WebCore::Frame* frame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    if (!smartData->frame) {
        WebCore::FrameLoaderClientEfl* frameLoaderClient = _ewk_frame_loader_efl_get(frame);
        frameLoaderClient->setWebFrame(ewkFrame);
        smartData->frame = frame;
        smartData->view = view;
        frame->init();
        return true;
    }

    ERR("frame %p already set for %p, ignored new %p",
        smartData->frame, ewkFrame, frame);
    return false;
}
