Eina_Bool ewk_view_setting_enable_scripts_set(Evas_Object* ewkView, Eina_Bool enable)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    enable = !!enable;
    if (priv->settings.enableScripts != enable) {
        priv->pageSettings->setScriptEnabled(enable);
        priv->settings.enableScripts = enable;
    }
    return true;
}
