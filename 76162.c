void ewk_view_frame_main_load_started(Evas_Object* ewkView)
{
    DBG("ewkView=%p", ewkView);
    Evas_Object* frame = ewk_view_frame_main_get(ewkView);
    evas_object_smart_callback_call(frame, "load,started", 0);
}
