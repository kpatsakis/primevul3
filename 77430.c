static void _ewk_frame_smart_resize(Evas_Object* ewkFrame, Evas_Coord width, Evas_Coord height)
{
    EWK_FRAME_SD_GET(ewkFrame, smartData);
    evas_object_resize(smartData->base.clipper, width, height);

#ifdef EWK_FRAME_DEBUG
    evas_object_resize(smartData->region, width, height);
    Evas_Coord x, y;
    evas_object_geometry_get(smartData->region, &x, &y, &width, &height);
    INF("region=%p, visible=%d, geo=%d,%d + %dx%d",
        smartData->region, evas_object_visible_get(smartData->region), x, y, width, height);
    _ewk_frame_debug(ewkFrame);
#endif
}
