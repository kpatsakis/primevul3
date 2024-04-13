void ewk_frame_force_layout(Evas_Object* ewkFrame)
{
    DBG("ewkFrame=%p", ewkFrame);
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);
    EINA_SAFETY_ON_NULL_RETURN(smartData->frame);
    WebCore::FrameView* view = smartData->frame->view();
    if (view)
        view->forceLayout(true);
}
