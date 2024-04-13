Eina_Bool ewk_view_forward(Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    return ewk_frame_forward(smartData->main_frame);
}
