void ewk_frame_load_document_finished(Evas_Object* ewkFrame)
{
    evas_object_smart_callback_call(ewkFrame, "load,document,finished", 0);
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);
    ewk_view_load_document_finished(smartData->view, ewkFrame);
}
