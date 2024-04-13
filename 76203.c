WTF::PassRefPtr<WebCore::Widget> ewk_view_plugin_create(Evas_Object* ewkView, Evas_Object* frame, const WebCore::IntSize& pluginSize, WebCore::HTMLPlugInElement* element, const WebCore::KURL& url, const WTF::Vector<WTF::String>& paramNames, const WTF::Vector<WTF::String>& paramValues, const WTF::String& mimeType, bool loadManually)
{
    DBG("ewkView=%p, frame=%p, size=%dx%d, element=%p, url=%s, mimeType=%s",
        ewkView, frame, pluginSize.width(), pluginSize.height(), element,
        url.string().utf8().data(), mimeType.utf8().data());

    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, 0);
    smartData->changed.frame_rect = true;
    _ewk_view_smart_changed(smartData);

    return ewk_frame_plugin_create
               (frame, pluginSize, element, url, paramNames, paramValues,
               mimeType, loadManually);
}
