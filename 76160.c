Evas_Object* ewk_view_frame_main_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, 0);
    return smartData->main_frame;
}
