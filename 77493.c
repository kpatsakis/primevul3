ewk_frame_scroll_pos_get(const Evas_Object* ewkFrame, int* x, int* y)
{
    if (x)
        *x = 0;
    if (y)
        *y = 0;
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame->view(), false);
    WebCore::IntPoint pos = smartData->frame->view()->scrollPosition();
    if (x)
        *x = pos.x();
    if (y)
        *y = pos.y();
    return true;
}
