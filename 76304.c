Eina_Bool ewk_view_zoom_weak_set(Evas_Object* ewkView, float zoom, Evas_Coord centerX, Evas_Coord centerY)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET(smartData, priv);

    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api->zoom_weak_set, false);

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

    smartData->animated_zoom.zoom.start = ewk_frame_page_zoom_get(smartData->main_frame);
    smartData->animated_zoom.zoom.end = zoom;
    smartData->animated_zoom.zoom.current = zoom;
    return smartData->api->zoom_weak_set(smartData, zoom, centerX, centerY);
}
