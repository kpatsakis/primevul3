Eina_Bool ewk_view_pre_render_region(Evas_Object* ewkView, Evas_Coord x, Evas_Coord y, Evas_Coord width, Evas_Coord height, float zoom)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api->pre_render_region, false);
    float cur_zoom;
    Evas_Coord contentsWidth, contentsHeight;

    /* When doing animated zoom it's not possible to call pre-render since it
     * would screw up parameters that animation is currently using
     */
    if (priv->animatedZoom.animator)
        return false;

    cur_zoom = ewk_frame_page_zoom_get(smartData->main_frame);

    if (cur_zoom < 0.00001)
        return false;
    if (!ewk_frame_contents_size_get(smartData->main_frame, &contentsWidth, &contentsHeight))
        return false;

    contentsWidth *= zoom / cur_zoom;
    contentsHeight *= zoom / cur_zoom;
    DBG("region %d,%d+%dx%d @ %f contents=%dx%d", x, y, width, height, zoom, contentsWidth, contentsHeight);

    if (x + width > contentsWidth)
        width = contentsWidth - x;

    if (y + height > contentsHeight)
        height = contentsHeight - y;

    if (x < 0) {
        width += x;
        x = 0;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }

    return smartData->api->pre_render_region(smartData, x, y, width, height, zoom);
}
