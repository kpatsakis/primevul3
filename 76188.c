void ewk_view_need_touch_events_set(Evas_Object* ewkView, bool needed)
{
    EWK_VIEW_SD_GET(ewkView, smartData);
    EWK_VIEW_PRIV_GET(smartData, priv);

    priv->flags.needTouchEvents = needed;
}
