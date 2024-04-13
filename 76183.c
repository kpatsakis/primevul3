void ewk_view_mouse_link_hover_in(Evas_Object* ewkView, void* data)
{
    evas_object_smart_callback_call(ewkView, "link,hover,in", data);
}
