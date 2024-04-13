void ewk_frame_load_resource_finished(Evas_Object* ewkFrame, unsigned long identifier)
{
    evas_object_smart_callback_call(ewkFrame, "load,resource,finished", &identifier);
}
