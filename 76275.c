Eina_Bool ewk_view_text_matches_highlight_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    return ewk_frame_text_matches_highlight_get(smartData->main_frame);
}
