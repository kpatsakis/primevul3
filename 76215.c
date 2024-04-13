void ewk_view_run_javascript_alert(Evas_Object* ewkView, Evas_Object* frame, const char* message)
{
    DBG("ewkView=%p frame=%p message=%s", ewkView, frame, message);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api);

    if (!smartData->api->run_javascript_alert)
        return;

    smartData->api->run_javascript_alert(smartData, frame, message);
}
