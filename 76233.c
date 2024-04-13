Eina_Bool ewk_view_setting_enable_auto_resize_window_set(Evas_Object* ewkView, Eina_Bool resizable)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    priv->settings.enableAutoResizeWindow = resizable;
    return true;
}
