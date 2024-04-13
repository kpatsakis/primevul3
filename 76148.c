Eina_Bool ewk_view_editable_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    return ewk_frame_editable_get(smartData->main_frame);
}
