Eina_Bool ewk_view_editable_set(Evas_Object* ewkView, Eina_Bool editable)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    return ewk_frame_editable_set(smartData->main_frame, editable);
}
