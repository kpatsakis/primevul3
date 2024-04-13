void ewk_view_fixed_layout_size_set(Evas_Object* ewkView, Evas_Coord width, Evas_Coord height)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);

    WebCore::FrameView* view = priv->mainFrame->view();
    if (!view)
        return;

    WebCore::IntSize layoutSize(width, height);
    if (layoutSize.width() <= 0 && layoutSize.height() <= 0) {
        if (!view->useFixedLayout())
            return;
        view->setUseFixedLayout(false);
    } else {
        WebCore::IntSize fixedLayoutSize = view->fixedLayoutSize();
        if (fixedLayoutSize == layoutSize)
            return;
        view->setFixedLayoutSize(layoutSize);
        view->setUseFixedLayout(true);
    }

    view->setNeedsLayout();
}
