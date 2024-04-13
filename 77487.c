void ewk_frame_request_assign_identifier(Evas_Object* ewkFrame, const Ewk_Frame_Resource_Request* request)
{
    evas_object_smart_callback_call(ewkFrame, "resource,request,new", (void*)request);
}
