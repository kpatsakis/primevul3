void ewk_view_editor_client_selection_changed(Evas_Object* ewkView)
{
    evas_object_smart_callback_call(ewkView, "editorclient,selection,changed", 0);
}
