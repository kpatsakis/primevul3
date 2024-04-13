void ewk_view_fixed_layout_size_get(const Evas_Object* ewkView, Evas_Coord* width, Evas_Coord* height)
{
    if (width)
        *width = 0;
    if (height)
        *height = 0;
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);
    WebCore::FrameView* view = priv->mainFrame->view();
    if (view->useFixedLayout()) {
        WebCore::IntSize size = view->fixedLayoutSize();
        if (width)
            *width = size.width();
        if (height)
            *height = size.height();
    }
}
