Eina_Bool ewk_view_zoom_range_set(Evas_Object* ewkView, float minScale, float maxScale)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);

    if (maxScale < minScale) {
        WRN("min_scale is larger than max_scale");
        return false;
    }

    priv->settings.zoomRange.minScale = minScale;
    priv->settings.zoomRange.maxScale = maxScale;

    return true;
}
