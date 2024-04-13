Eina_Bool ewk_view_text_zoom_set(Evas_Object* ewkView, float textZoomFactor)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    return ewk_frame_text_zoom_set(smartData->main_frame, textZoomFactor);
}
