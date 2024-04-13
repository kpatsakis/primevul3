static void _ewk_frame_smart_add(Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET(ewkFrame, smartData);

    if (!smartData) {
        smartData = static_cast<Ewk_Frame_Smart_Data*>(calloc(1, sizeof(Ewk_Frame_Smart_Data)));
        if (!smartData) {
            CRITICAL("could not allocate Ewk_Frame_Smart_Data");
            return;
        }
        evas_object_smart_data_set(ewkFrame, smartData);
    }

    smartData->self = ewkFrame;

    _parent_sc.add(ewkFrame);
    evas_object_static_clip_set(smartData->base.clipper, false);
    evas_object_move(smartData->base.clipper, 0, 0);
    evas_object_resize(smartData->base.clipper, 0, 0);

#ifdef EWK_FRAME_DEBUG
    smartData->region = evas_object_rectangle_add(smartData->base.evas);
    static int i = 0;
    switch (i) {
    case 0:
        evas_object_color_set(smartData->region, 128, 0, 0, 128);
        break;
    case 1:
        evas_object_color_set(smartData->region, 0, 128, 0, 128);
        break;
    case 2:
        evas_object_color_set(smartData->region, 0, 0, 128, 128);
        break;
    case 3:
        evas_object_color_set(smartData->region, 128, 0, 0, 128);
        break;
    case 4:
        evas_object_color_set(smartData->region, 128, 128, 0, 128);
        break;
    case 5:
        evas_object_color_set(smartData->region, 128, 0, 128, 128);
        break;
    case 6:
        evas_object_color_set(smartData->region, 0, 128, 128, 128);
        break;
    default:
        break;
    }
    i++;
    if (i > 6)
        i = 0;

    evas_object_smart_member_add(smartData->region, ewkFrame);
    evas_object_hide(smartData->region);
#endif
}
