Ewk_View_Smart_Data* ewk_view_smart_data_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, 0);
    return smartData;
}
