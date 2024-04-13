Eina_Bool ewk_view_setting_auto_shrink_images_set(Evas_Object* ewkView, Eina_Bool automatic)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    automatic = !!automatic;
    if (priv->settings.autoShrinkImages != automatic) {
        priv->pageSettings->setShrinksStandaloneImagesToFit(automatic);
        priv->settings.autoShrinkImages = automatic;
    }
    return true;
}
