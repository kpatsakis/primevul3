Ewk_Text_Selection_Type ewk_frame_text_selection_type_get(const Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, EWK_TEXT_SELECTION_NONE);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, EWK_TEXT_SELECTION_NONE);

    WebCore::FrameSelection* controller = smartData->frame->selection();
    if (!controller)
        return EWK_TEXT_SELECTION_NONE;

    return static_cast<Ewk_Text_Selection_Type>(controller->selectionType());
}
