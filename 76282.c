void ewk_view_theme_set(Evas_Object* ewkView, const char* path)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);
    if (!eina_stringshare_replace(&priv->settings.theme, path))
        return;

    WebCore::FrameView* view = priv->mainFrame->view();
    if (view) {
        view->setEdjeTheme(WTF::String(path));
        priv->page->theme()->themeChanged();
    }

}
