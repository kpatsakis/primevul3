Eina_Bool ewk_frame_page_zoom_set(Evas_Object* ewkFrame, float pageZoomFactor)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    smartData->frame->setPageZoomFactor(pageZoomFactor);
    return true;
}
