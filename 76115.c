static Eina_Bool _ewk_view_smart_focus_out(Ewk_View_Smart_Data* smartData)
{
    EWK_VIEW_PRIV_GET(smartData, priv);
    WebCore::FocusController* focusController = priv->page->focusController();
    DBG("ewkView=%p, fc=%p", smartData->self, focusController);
    EINA_SAFETY_ON_NULL_RETURN_VAL(focusController, false);

    focusController->setActive(false);
    focusController->setFocused(false);
    return true;
}
