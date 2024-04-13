Eina_Bool ewk_view_user_scalable_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);

    return priv->settings.zoomRange.userScalable;
}
