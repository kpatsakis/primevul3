Eina_Bool ewk_view_zoom_animated_mark_stop(Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    _ewk_view_zoom_animated_mark_stop(smartData);
    return true;
}
