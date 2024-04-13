Eina_Bool ewk_view_setting_encoding_custom_set(Evas_Object* ewkView, const char* encoding)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    Evas_Object* main_frame = ewk_view_frame_main_get(ewkView);
    WebCore::Frame* coreFrame = EWKPrivate::coreFrame(main_frame);
    DBG("%s", encoding);
    if (eina_stringshare_replace(&priv->settings.encodingCustom, encoding))
        coreFrame->loader()->reloadWithOverrideEncoding(String::fromUTF8(encoding));
    return true;
}
