void ewk_view_mixed_content_displayed_set(Evas_Object* ewkView, bool hasDisplayed)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);

    priv->flags.hasDisplayedMixedContent = hasDisplayed;

    if (hasDisplayed)
        evas_object_smart_callback_call(ewkView, "mixedcontent,displayed", 0);
}
