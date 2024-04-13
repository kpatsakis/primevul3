ewk_frame_scroll_set(Evas_Object* ewkFrame, int x, int y)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame->view(), false);
    smartData->frame->view()->setScrollPosition(WebCore::IntPoint(x, y));
    return true;
}
