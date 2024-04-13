void ewk_frame_contents_size_changed(Evas_Object* ewkFrame, Evas_Coord width, Evas_Coord height)
{
    Evas_Coord size[2] = {width, height};
    evas_object_smart_callback_call(ewkFrame, "contents,size,changed", size);
}
