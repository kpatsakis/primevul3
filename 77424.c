static inline void _ewk_frame_debug(Evas_Object* ewkFrame)
{
    Evas_Object* clip, * parent;
    Evas_Coord x, y, width, height, contentX, contentY, contentWidth, contentHeight;
    int red, green, blue, alpha, contentRed, contentGreen, contentBlue, contentAlpha;

    evas_object_color_get(ewkFrame, &red, &green, &blue, &alpha);
    evas_object_geometry_get(ewkFrame, &x, &y, &width, &height);

    clip = evas_object_clip_get(ewkFrame);
    evas_object_color_get(clip, &contentRed, &contentGreen, &contentBlue, &contentAlpha);
    evas_object_geometry_get(clip, &contentX, &contentY, &contentWidth, &contentHeight);

    fprintf(stderr, "%p: type=%s name=%s, visible=%d, color=%02x%02x%02x%02x, %d,%d+%dx%d, clipper=%p (%d, %02x%02x%02x%02x, %d,%d+%dx%d)\n",
            ewkFrame, evas_object_type_get(ewkFrame), evas_object_name_get(ewkFrame), evas_object_visible_get(ewkFrame),
            red, green, blue, alpha, x, y, width, height,
            clip, evas_object_visible_get(clip), contentRed, contentGreen, contentBlue, contentAlpha, contentX, contentY, contentWidth, contentHeight);
    parent = evas_object_smart_parent_get(ewkFrame);
    if (!parent)
        fprintf(stderr, "\n");
    else
        _ewk_frame_debug(parent);
}
