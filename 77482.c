void ewk_frame_redirect_cancelled(Evas_Object* ewkFrame)
{
    evas_object_smart_callback_call(ewkFrame, "redirect,cancelled", 0);
}
