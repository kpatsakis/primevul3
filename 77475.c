Eina_Bool ewk_frame_navigate(Evas_Object* ewkFrame, int steps)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    WebCore::Page* page = smartData->frame->page();
    if (!page->canGoBackOrForward(steps))
        return false;
    page->goBackOrForward(steps);
    return true;
}
