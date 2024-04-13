static void _ewk_view_smart_calculate(Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET(ewkView, smartData);
    EWK_VIEW_PRIV_GET(smartData, priv);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api->contents_resize);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api->scrolls_process);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api->repaints_process);
    Evas_Coord x, y, width, height;

    smartData->changed.any = false;

    if (!smartData->main_frame || !priv->mainFrame)
        return;

    evas_object_geometry_get(ewkView, &x, &y, &width, &height);

    DBG("ewkView=%p geo=[%d, %d + %dx%d], changed: size=%hhu, "
        "scrolls=%zu, repaints=%zu",
        ewkView, x, y, width, height, smartData->changed.size,
        priv->scrolls.count, priv->repaints.count);

    if (smartData->changed.size && ((width != smartData->view.w) || (height != smartData->view.h))) {
        WebCore::FrameView* view = priv->mainFrame->view();
        if (view) {
            view->resize(width, height);
            view->forceLayout();
            view->adjustViewSize();
        }
        evas_object_resize(smartData->main_frame, width, height);
        evas_object_resize(smartData->events_rect, width, height);
        smartData->changed.frame_rect = true;
        smartData->view.w = width;
        smartData->view.h = height;

        _ewk_view_repaint_add(priv, 0, 0, width, height);

        evas_object_smart_callback_call(ewkView, "view,resized", 0);
    }
    smartData->changed.size = false;

    if (smartData->changed.position && ((x != smartData->view.x) || (y != smartData->view.y))) {
        evas_object_move(smartData->main_frame, x, y);
        evas_object_move(smartData->backing_store, x, y);
        evas_object_move(smartData->events_rect, x, y);
        smartData->changed.frame_rect = true;
        smartData->view.x = x;
        smartData->view.y = y;
    }
    smartData->changed.position = false;

    if (!smartData->api->scrolls_process(smartData))
        ERR("failed to process scrolls.");
    _ewk_view_scrolls_flush(priv);

    if (!smartData->api->repaints_process(smartData))
        ERR("failed to process repaints.");
    _ewk_view_repaints_flush(priv);

    if (smartData->changed.frame_rect) {
        WebCore::FrameView* view = priv->mainFrame->view();
        view->frameRectsChanged(); /* force tree to get position from root */
        smartData->changed.frame_rect = false;
    }
}
