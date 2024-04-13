void ewk_frame_redirect_requested(Evas_Object* ewkFrame, const char* url)
{
    evas_object_smart_callback_call(ewkFrame, "redirect,requested", (void*)url);
}
