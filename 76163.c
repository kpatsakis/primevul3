Eina_Bool ewk_view_history_enable_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    return static_cast<WebCore::BackForwardListImpl*>(priv->page->backForwardList())->enabled();
}
