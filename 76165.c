Ewk_History* ewk_view_history_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, 0);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, 0);
    if (!static_cast<WebCore::BackForwardListImpl*>(priv->page->backForwardList())->enabled()) {
        ERR("asked history, but it's disabled! Returning 0!");
        return 0;
    }
    return priv->history;
}
