Evas_Object* ewk_frame_add(Evas* canvas)
{
    return evas_object_smart_add(canvas, _ewk_frame_smart_class_new());
}
