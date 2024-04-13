void ewk_frame_request_will_send(Evas_Object* ewkFrame, Ewk_Frame_Resource_Messages* messages)
{
    evas_object_smart_callback_call(ewkFrame, "resource,request,willsend", messages);
}
