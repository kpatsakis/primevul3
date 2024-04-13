static void _ewk_view_zoom_animated_finish(Ewk_View_Smart_Data* smartData)
{
    EWK_VIEW_PRIV_GET(smartData, priv);
    ecore_animator_del(priv->animatedZoom.animator);
    priv->animatedZoom.animator = 0;
    _ewk_view_zoom_animated_mark_stop(smartData);
    evas_object_smart_callback_call(smartData->self, "zoom,animated,end", 0);
}
