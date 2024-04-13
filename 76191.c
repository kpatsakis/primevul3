Eina_Bool ewk_view_page_zoom_set(Evas_Object* ewkView, float pageZoomFactor)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    return ewk_frame_page_zoom_set(smartData->main_frame, pageZoomFactor);
}
