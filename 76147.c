void ewk_view_download_request(Evas_Object* ewkView, Ewk_Download* download)
{
    DBG("ewkView=%p", ewkView);
    evas_object_smart_callback_call(ewkView, "download,request", download);
}
