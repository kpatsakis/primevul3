Ewk_View_Mode ewk_view_mode_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, EWK_VIEW_MODE_WINDOWED);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, EWK_VIEW_MODE_WINDOWED);

    return static_cast<Ewk_View_Mode>(priv->page->viewMode());
}
