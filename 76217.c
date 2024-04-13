bool ewk_view_run_javascript_prompt(Evas_Object* ewkView, Evas_Object* frame, const char* message, const char* defaultValue, char** value)
{
    DBG("ewkView=%p frame=%p message=%s", ewkView, frame, message);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api, false);

    if (!smartData->api->run_javascript_prompt)
        return false;

    return smartData->api->run_javascript_prompt(smartData, frame, message, defaultValue, value);
}
