Eina_Bool ewk_view_zoom_weak_smooth_scale_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    return smartData->zoom_weak_smooth_scale;
}
