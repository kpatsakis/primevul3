const char* ewk_view_uri_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, 0);
    return ewk_frame_uri_get(smartData->main_frame);
}
