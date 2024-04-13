void ewk_frame_xss_detected(Evas_Object* ewkFrame, const Ewk_Frame_Xss_Notification* xssInfo)
{
    evas_object_smart_callback_call(ewkFrame, "xss,detected", (void*)xssInfo);
}
