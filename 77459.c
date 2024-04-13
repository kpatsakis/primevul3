void ewk_frame_icon_changed(Evas_Object* ewkFrame)
{
    DBG("ewkFrame=%p", ewkFrame);
    evas_object_smart_callback_call(ewkFrame, "icon,changed", 0);
}
