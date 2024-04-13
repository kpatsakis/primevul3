void ewk_view_frame_main_cleared(Evas_Object* ewkView)
{
    DBG("ewkView=%p", ewkView);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api->flush);
    smartData->api->flush(smartData);

    ewk_view_mixed_content_displayed_set(ewkView, false);
    ewk_view_mixed_content_run_set(ewkView, false);
}
