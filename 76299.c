Eina_Bool ewk_view_zoom_animated_set(Evas_Object* ewkView, float zoom, float duration, Evas_Coord centerX, Evas_Coord centerY)
{
    double now;
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
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

    if (priv->animatedZoom.animator)
        priv->animatedZoom.zoom.start = _ewk_view_zoom_animated_current(priv);
    else {
        priv->animatedZoom.zoom.start = ewk_frame_page_zoom_get(smartData->main_frame);
        _ewk_view_zoom_animation_start(smartData);
    }

    if (centerX < 0)
        centerX = 0;
    if (centerY < 0)
        centerY = 0;

    now = ecore_loop_time_get();
    priv->animatedZoom.time.start = now;
    priv->animatedZoom.time.end = now + duration;
    priv->animatedZoom.time.duration = duration;
    priv->animatedZoom.zoom.end = zoom;
    priv->animatedZoom.zoom.range = (priv->animatedZoom.zoom.end - priv->animatedZoom.zoom.start);
    priv->animatedZoom.center.x = centerX;
    priv->animatedZoom.center.y = centerY;
    smartData->animated_zoom.zoom.current = priv->animatedZoom.zoom.start;
    smartData->animated_zoom.zoom.start = priv->animatedZoom.zoom.start;
    smartData->animated_zoom.zoom.end = priv->animatedZoom.zoom.end;

    return true;
}
