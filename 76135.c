static Eina_Bool _ewk_view_zoom_animator_cb(void* data)
{
    Ewk_View_Smart_Data* smartData = static_cast<Ewk_View_Smart_Data*>(data);
    Evas_Coord centerX, centerY;
    EWK_VIEW_PRIV_GET(smartData, priv);
    double now = ecore_loop_time_get();

    centerX = priv->animatedZoom.center.x;
    centerY = priv->animatedZoom.center.y;

    if (centerX >= smartData->view.w)
        centerX = smartData->view.w - 1;
    if (centerY >= smartData->view.h)
        centerY = smartData->view.h - 1;

    if ((now >= priv->animatedZoom.time.end)
        || (now < priv->animatedZoom.time.start)) {
        _ewk_view_zoom_animated_finish(smartData);
        ewk_view_zoom_set(smartData->self, priv->animatedZoom.zoom.end, centerX, centerY);
        smartData->api->sc.calculate(smartData->self);
        return false;
    }

    smartData->animated_zoom.zoom.current = _ewk_view_zoom_animated_current(priv);
    smartData->api->zoom_weak_set(smartData, smartData->animated_zoom.zoom.current, centerX, centerY);
    return true;
}
