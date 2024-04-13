Eina_Bool ewk_view_setting_minimum_timer_interval_set(Evas_Object* ewkView, double interval)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    if (fabs(priv->settings.domTimerInterval - interval) >= std::numeric_limits<double>::epsilon()) {
        priv->pageSettings->setMinDOMTimerInterval(interval);
        priv->settings.domTimerInterval = interval;
    }
    return true;
}
