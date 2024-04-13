unsigned int ewk_frame_text_matches_mark(Evas_Object* ewkFrame, const char* string, Eina_Bool caseSensitive, Eina_Bool highlight, unsigned int limit)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(string, 0);

    smartData->frame->editor()->setMarkedTextMatchesAreHighlighted(highlight);
    return smartData->frame->editor()->countMatchesForText(WTF::String::fromUTF8(string), caseSensitive, limit, true);
}
