void ewk_frame_response_received(Evas_Object* ewkFrame, Ewk_Frame_Resource_Response* response)
{
    evas_object_smart_callback_call(ewkFrame, "resource,response,received", response);
}
