void ewk_frame_load_started(Evas_Object* ewkFrame)
{
    Evas_Object* mainFrame;
    DBG("ewkFrame=%p", ewkFrame);
    evas_object_smart_callback_call(ewkFrame, "load,started", 0);
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);
    ewk_view_load_started(smartData->view, ewkFrame);

    mainFrame = ewk_view_frame_main_get(smartData->view);
    if (mainFrame == ewkFrame)
        ewk_view_frame_main_load_started(smartData->view);
}
