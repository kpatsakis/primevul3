void ewk_frame_load_progress_changed(Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);
    EINA_SAFETY_ON_NULL_RETURN(smartData->frame);

    double progress = smartData->frame->page()->progress()->estimatedProgress();

    DBG("ewkFrame=%p (p=%0.3f)", ewkFrame, progress);

    evas_object_smart_callback_call(ewkFrame, "load,progress", &progress);
    ewk_view_load_progress_changed(smartData->view);
}
