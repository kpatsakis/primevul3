static void _ewk_view_smart_flush(Ewk_View_Smart_Data* smartData)
{
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);
    _ewk_view_repaints_flush(priv);
    _ewk_view_scrolls_flush(priv);
}
