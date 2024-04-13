static void _ewk_view_smart_resize(Evas_Object* ewkView, Evas_Coord w, Evas_Coord h)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);

    evas_object_resize(smartData->backing_store, w, h);

    smartData->changed.size = true;
    _ewk_view_smart_changed(smartData);
}
