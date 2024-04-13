Evas_Object* ewk_view_frame_focused_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, 0);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, 0);

    WebCore::Frame* core = priv->page->focusController()->focusedFrame();
    if (!core)
        return 0;

    WebCore::FrameLoaderClientEfl* client = static_cast<WebCore::FrameLoaderClientEfl*>(core->loader()->client());
    if (!client)
        return 0;
    return client->webFrame();
}
