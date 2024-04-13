Eina_Bool ewk_frame_text_matches_unmark_all(Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);

    smartData->frame->document()->markers()->removeMarkers(WebCore::DocumentMarker::TextMatch);
    return true;
}
