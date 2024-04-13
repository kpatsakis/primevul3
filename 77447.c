Eina_Bool ewk_frame_feed_focus_in(Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    WebCore::FocusController* focusController = smartData->frame->page()->focusController();
    focusController->setFocusedFrame(smartData->frame);
    return true;
}
