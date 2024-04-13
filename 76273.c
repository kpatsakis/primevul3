void ewk_view_statusbar_visible_set(Evas_Object* ewkView, bool  visible)
{
    DBG("ewkView=%p (visible=%d)", ewkView, !!visible);
    evas_object_smart_callback_call(ewkView, "statusbar,visible,set", &visible);
}
