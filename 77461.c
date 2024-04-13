void ewk_frame_load_committed(Evas_Object* ewkFrame)
{
    evas_object_smart_callback_call(ewkFrame, "load,committed", 0);
}
