void ewk_view_mouse_link_hover_out(Evas_Object* ewkView)
{
    evas_object_smart_callback_call(ewkView, "link,hover,out", 0);
}
