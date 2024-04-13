Eina_Bool ewk_view_history_enable_set(Evas_Object* ewkView, Eina_Bool enable)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    static_cast<WebCore::BackForwardListImpl*>(priv->page->backForwardList())->setEnabled(enable);
    return true;
}
