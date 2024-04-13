Eina_Bool ewk_view_zoom_animated_mark_start(Evas_Object* ewkView, float zoom)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    smartData->animated_zoom.zoom.start = zoom;
    return true;
}
