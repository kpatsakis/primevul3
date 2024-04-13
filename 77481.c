WTF::PassRefPtr<WebCore::Widget> ewk_frame_plugin_create(Evas_Object* ewkFrame, const WebCore::IntSize& pluginSize, WebCore::HTMLPlugInElement* element, const WebCore::KURL& url, const WTF::Vector<WTF::String>& paramNames, const WTF::Vector<WTF::String>& paramValues, const WTF::String& mimeType, bool loadManually)
{
#if ENABLE(NETSCAPE_PLUGIN_API)
    DBG("ewkFrame=%p, size=%dx%d, element=%p, url=%s, mimeType=%s",
        ewkFrame, pluginSize.width(), pluginSize.height(), element,
        url.string().utf8().data(), mimeType.utf8().data());

    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, sd, 0);


    WTF::RefPtr<WebCore::PluginView> pluginView = WebCore::PluginView::create
        (sd->frame, pluginSize, element, url, paramNames, paramValues,
        mimeType, loadManually);

    if (pluginView->status() == WebCore::PluginStatusLoadedSuccessfully)
        return pluginView.release();
#endif // #if ENABLE(NETSCAPE_PLUGIN_API)
    return 0;
}
