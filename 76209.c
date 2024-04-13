void ewk_view_ready(Evas_Object* ewkView)
{
    DBG("ewkView=%p", ewkView);
    evas_object_smart_callback_call(ewkView, "ready", 0);
}
