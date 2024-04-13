void ewk_frame_load_resource_failed(Evas_Object* ewkFrame, Ewk_Frame_Load_Error* error)
{
    evas_object_smart_callback_call(ewkFrame, "load,resource,failed", error);
}
