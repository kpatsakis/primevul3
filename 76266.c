Eina_Bool ewk_view_setting_spatial_navigation_set(Evas_Object* ewkView, Eina_Bool enable)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    enable = !!enable;
    if (priv->settings.spatialNavigation != enable) {
        priv->pageSettings->setSpatialNavigationEnabled(enable);
        priv->settings.spatialNavigation = enable;
    }
    return true;
}
