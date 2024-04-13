void ewk_frame_load_firstlayout_nonempty_finished(Evas_Object* ewkFrame)
{
    evas_object_smart_callback_call(ewkFrame, "load,nonemptylayout,finished", 0);
}
