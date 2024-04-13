ewk_frame_scroll_add(Evas_Object* ewkFrame, int deltaX, int deltaY)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame->view(), false);
    smartData->frame->view()->scrollBy(WebCore::IntSize(deltaX, deltaY));
    return true;
}
