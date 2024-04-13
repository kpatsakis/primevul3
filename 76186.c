bool ewk_view_navigation_policy_decision(Evas_Object* ewkView, Ewk_Frame_Resource_Request* request)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, true);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api, true);

    if (!smartData->api->navigation_policy_decision)
        return true;

    return smartData->api->navigation_policy_decision(smartData, request);
}
