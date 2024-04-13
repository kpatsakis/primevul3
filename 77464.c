void ewk_frame_load_firstlayout_finished(Evas_Object* ewkFrame)
{
    evas_object_smart_callback_call(ewkFrame, "load,firstlayout,finished", 0);
}
