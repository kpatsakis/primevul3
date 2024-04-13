Eina_Bool ewk_view_setting_application_cache_set(Evas_Object* ewkView, Eina_Bool enable)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    enable = !!enable;
    if (priv->settings.offlineAppCache != enable) {
        priv->pageSettings->setOfflineWebApplicationCacheEnabled(enable);
        priv->settings.offlineAppCache = enable;
    }
    return true;
}
