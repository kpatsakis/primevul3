void ewk_view_editor_client_contents_changed(Evas_Object* ewkView)
{
    evas_object_smart_callback_call(ewkView, "editorclient,contents,changed", 0);
}
