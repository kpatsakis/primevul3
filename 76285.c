void ewk_view_tooltip_text_set(Evas_Object* ewkView, const char* text)
{
    DBG("ewkView=%p text=%s", ewkView, text);
    evas_object_smart_callback_call(ewkView, "tooltip,text,set", (void*)text);
}
