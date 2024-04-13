float ewk_view_zoom_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, -1.0);
    return ewk_frame_page_zoom_get(smartData->main_frame);
}
