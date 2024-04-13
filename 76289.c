Eina_Bool ewk_view_user_scalable_set(Evas_Object* ewkView, Eina_Bool userScalable)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);

    priv->settings.zoomRange.userScalable = userScalable;

    return true;
}
