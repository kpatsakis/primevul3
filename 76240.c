const char* ewk_view_setting_encoding_custom_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, 0);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, 0);
    Evas_Object* main_frame = ewk_view_frame_main_get(ewkView);
    WebCore::Frame* core_frame = EWKPrivate::coreFrame(main_frame);

    String overrideEncoding = core_frame->loader()->documentLoader()->overrideEncoding();

    if (overrideEncoding.isEmpty())
        return 0;

    eina_stringshare_replace(&priv->settings.encodingCustom, overrideEncoding.utf8().data());
    return priv->settings.encodingCustom;
}
