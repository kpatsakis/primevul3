Eina_Bool ewk_view_setting_auto_load_images_set(Evas_Object* ewkView, Eina_Bool automatic)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    automatic = !!automatic;
    if (priv->settings.autoLoadImages != automatic) {
        priv->pageSettings->setLoadsImagesAutomatically(automatic);
        priv->settings.autoLoadImages = automatic;
    }
    return true;
}
