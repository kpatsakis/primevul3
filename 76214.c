void ewk_view_restore_state(Evas_Object* ewkView, Evas_Object* frame)
{
    evas_object_smart_callback_call(ewkView, "restore", frame);
}
