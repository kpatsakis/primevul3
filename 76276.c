Eina_Bool ewk_view_text_matches_highlight_set(Evas_Object* ewkView, Eina_Bool highlight)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    return ewk_frame_text_matches_highlight_set(smartData->main_frame, highlight);
}
