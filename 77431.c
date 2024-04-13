static void _ewk_frame_smart_set(Evas_Smart_Class* api)
{
    evas_object_smart_clipped_smart_set(api);
    api->add = _ewk_frame_smart_add;
    api->del = _ewk_frame_smart_del;
    api->resize = _ewk_frame_smart_resize;
}
