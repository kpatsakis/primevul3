void ewk_frame_load_provisional(Evas_Object* ewkFrame)
{
    evas_object_smart_callback_call(ewkFrame, "load,provisional", 0);
}
