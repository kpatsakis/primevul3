Eina_Bool ewk_view_navigate_possible(Evas_Object* ewkView, int steps)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    return ewk_frame_navigate_possible(smartData->main_frame, steps);
}
