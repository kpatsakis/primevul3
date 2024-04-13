float ewk_frame_text_zoom_get(const Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, -1.0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, -1.0);
    return smartData->frame->textZoomFactor();
}
