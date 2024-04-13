ewk_frame_scroll_size_get(const Evas_Object* ewkFrame, int* width, int* height)
{
    if (width)
        *width = 0;
    if (height)
        *height = 0;
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame->view(), false);
    WebCore::IntPoint point = smartData->frame->view()->maximumScrollPosition();
    if (width)
        *width = point.x();
    if (height)
        *height = point.y();
    return true;
}
