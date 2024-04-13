static void _ewk_view_smart_changed(Ewk_View_Smart_Data* smartData)
{
    if (smartData->changed.any)
        return;
    smartData->changed.any = true;
    evas_object_smart_changed(smartData->self);
}
