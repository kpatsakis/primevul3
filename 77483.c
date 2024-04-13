void ewk_frame_redirect_provisional_load(Evas_Object* ewkFrame)
{
    evas_object_smart_callback_call(ewkFrame, "redirect,load,provisional", 0);
}
