char* ewk_frame_selection_get(const Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, 0);
    WTF::CString selectedText = smartData->frame->editor()->selectedText().utf8();
    if (selectedText.isNull())
        return 0;
    return strdup(selectedText.data());
}
