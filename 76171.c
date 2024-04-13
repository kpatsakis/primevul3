void ewk_view_load_finished(Evas_Object* ewkView, const Ewk_Frame_Load_Error* error)
{
    DBG("ewkView=%p, error=%p", ewkView, error);
    evas_object_smart_callback_call(ewkView, "load,finished", (void*)error);
}
