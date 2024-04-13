Eina_Bool ewk_view_setting_font_minimum_logical_size_set(Evas_Object* ewkView, int size)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    if (priv->settings.fontMinimumLogicalSize != size) {
        priv->pageSettings->setMinimumLogicalFontSize(size);
        priv->settings.fontMinimumLogicalSize = size;
    }
    return true;
}
