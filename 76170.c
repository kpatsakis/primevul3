void ewk_view_load_error(Evas_Object* ewkView, const Ewk_Frame_Load_Error* error)
{
    DBG("ewkView=%p, error=%p", ewkView, error);
    evas_object_smart_callback_call(ewkView, "load,error", (void*)error);
}
