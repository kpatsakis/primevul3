void ewk_view_window_close(Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);

    ewk_view_stop(ewkView);
    if (!smartData->api->window_close)
        return;
    smartData->api->window_close(smartData);
}
