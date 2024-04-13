bool ewk_view_should_interrupt_javascript(Evas_Object* ewkView)
{
    DBG("ewkView=%p", ewkView);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api, false);

    if (!smartData->api->should_interrupt_javascript)
        return false;

    return smartData->api->should_interrupt_javascript(smartData);
}
