void ewk_frame_editor_client_selection_changed(Evas_Object* ewkFrame)
{
    evas_object_smart_callback_call(ewkFrame, "editorclient,selection,changed", 0);
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);
    ewk_view_editor_client_selection_changed(smartData->view);
}
