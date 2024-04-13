void ewk_view_zoom_weak_smooth_scale_set(Evas_Object* ewkView, Eina_Bool smoothScale)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    smoothScale = !!smoothScale;
    if (smartData->zoom_weak_smooth_scale == smoothScale)
        return;
    smartData->zoom_weak_smooth_scale = smoothScale;
    EINA_SAFETY_ON_NULL_RETURN(smartData->api);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api->zoom_weak_smooth_scale_set);
    smartData->api->zoom_weak_smooth_scale_set(smartData, smoothScale);
}
