static Eina_Bool _ewk_view_smart_focus_in(Ewk_View_Smart_Data* smartData)
{
    EWK_VIEW_PRIV_GET(smartData, priv);
    WebCore::FocusController* fc = priv->page->focusController();
    DBG("ewkView=%p, fc=%p", smartData->self, fc);
    EINA_SAFETY_ON_NULL_RETURN_VAL(fc, false);

    fc->setActive(true);
    fc->setFocused(true);
    return true;
}
