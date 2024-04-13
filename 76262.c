Eina_Bool ewk_view_setting_scripts_can_close_windows_set(Evas_Object* ewkView, Eina_Bool allow)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    allow = !!allow;
    if (priv->settings.scriptsCanCloseWindows != allow) {
        priv->pageSettings->setAllowScriptsToCloseWindows(allow);
        priv->settings.scriptsCanCloseWindows = allow;
    }
    return true;
}
