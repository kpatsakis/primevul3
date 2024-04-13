void ewk_frame_did_perform_first_navigation(Evas_Object* ewkFrame)
{
    evas_object_smart_callback_call(ewkFrame, "navigation,first", 0);
}
