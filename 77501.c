Eina_Bool ewk_frame_text_matches_highlight_set(Evas_Object* ewkFrame, Eina_Bool highlight)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    smartData->frame->editor()->setMarkedTextMatchesAreHighlighted(highlight);
    return true;
}
