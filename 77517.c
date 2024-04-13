Eina_Bool ewk_frame_visible_content_geometry_get(const Evas_Object* ewkFrame, Eina_Bool includeScrollbars, int* x, int* y, int* width, int* height)
{
    if (x)
        *x = 0;
    if (y)
        *y = 0;
    if (width)
        *width = 0;
    if (height)
        *height = 0;
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame->view(), false);
    WebCore::IntRect rect = smartData->frame->view()->visibleContentRect(includeScrollbars);
    if (x)
        *x = rect.x();
    if (y)
        *y = rect.y();
    if (width)
        *width = rect.width();
    if (height)
        *height = rect.height();
    return true;
}
