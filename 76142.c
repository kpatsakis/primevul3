void ewk_view_contents_size_changed(Evas_Object* ewkView, int width, int height)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api->contents_resize);

    if (!smartData->api->contents_resize(smartData, width, height))
        ERR("failed to resize contents to %dx%d", width, height);
}
