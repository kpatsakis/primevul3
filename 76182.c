Eina_Bool ewk_view_mode_set(Evas_Object* ewkView, Ewk_View_Mode viewMode)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);

    priv->page->setViewMode(static_cast<WebCore::Page::ViewMode>(viewMode));
    return true;
}
