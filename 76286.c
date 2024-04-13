void ewk_view_uri_changed(Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    const char* uri = ewk_frame_uri_get(smartData->main_frame);
    DBG("ewkView=%p, uri=%s", ewkView, uri ? uri : "(null)");
    evas_object_smart_callback_call(ewkView, "uri,changed", (void*)uri);
}
