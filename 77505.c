Eina_Bool ewk_frame_text_search(const Evas_Object* ewkFrame, const char* text, Eina_Bool caseSensitive, Eina_Bool forward, Eina_Bool wrap)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(text, false);

    return smartData->frame->editor()->findString(WTF::String::fromUTF8(text), forward, caseSensitive, wrap, true);
}
