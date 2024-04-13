Eina_Bool ewk_view_reload_full(Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    return ewk_frame_reload_full(smartData->main_frame);
}
