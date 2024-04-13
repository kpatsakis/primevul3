Eina_Bool ewk_view_zoom_set(Evas_Object* ewkView, float zoom, Evas_Coord centerX, Evas_Coord centerY)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET(smartData, priv);

    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api->zoom_set, false);

    if (!priv->settings.zoomRange.userScalable) {
        WRN("userScalable is false");
        return false;
    }

    if (zoom < priv->settings.zoomRange.minScale) {
        WRN("zoom level is < %f : %f", priv->settings.zoomRange.minScale, zoom);
        return false;
    }
    if (zoom > priv->settings.zoomRange.maxScale) {
        WRN("zoom level is > %f : %f", priv->settings.zoomRange.maxScale, zoom);
        return false;
    }

    _ewk_view_zoom_animated_mark_stop(smartData);
    return smartData->api->zoom_set(smartData, zoom, centerX, centerY);
}
