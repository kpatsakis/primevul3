Eina_Bool ewk_frame_editable_set(Evas_Object* ewkFrame, Eina_Bool editable)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    editable = !!editable;
    if (smartData->editable == editable)
        return true;
    smartData->editable = editable;
    if (editable)
        smartData->frame->editor()->applyEditingStyleToBodyElement();
    return true;
}
