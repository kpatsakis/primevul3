void ewk_view_scrolls_process(Ewk_View_Smart_Data* smartData)
{
    EINA_SAFETY_ON_NULL_RETURN(smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);
    if (!smartData->api->scrolls_process(smartData))
        ERR("failed to process scrolls.");
    _ewk_view_scrolls_flush(priv);
}
