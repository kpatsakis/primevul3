Eina_Bool ewk_view_setting_user_stylesheet_set(Evas_Object* ewkView, const char* uri)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    if (eina_stringshare_replace(&priv->settings.userStylesheet, uri)) {
        WebCore::KURL kurl(WebCore::KURL(), String::fromUTF8(uri));
        priv->pageSettings->setUserStyleSheetLocation(kurl);
    }
    return true;
}
