Eina_Bool ewk_view_setting_font_minimum_size_set(Evas_Object* ewkView, int size)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    if (priv->settings.fontMinimumSize != size) {
        priv->pageSettings->setMinimumFontSize(size);
        priv->settings.fontMinimumSize = size;
    }
    return true;
}
