bool ewk_view_run_javascript_confirm(Evas_Object* ewkView, Evas_Object* frame, const char* message)
{
    DBG("ewkView=%p frame=%p message=%s", ewkView, frame, message);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api, false);

    if (!smartData->api->run_javascript_confirm)
        return false;

    return smartData->api->run_javascript_confirm(smartData, frame, message);
}
