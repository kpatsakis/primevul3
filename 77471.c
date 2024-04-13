Eina_Bool ewk_frame_mixed_content_displayed_get(const Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    return smartData->hasDisplayedMixedContent;
}
