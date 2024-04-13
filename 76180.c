void ewk_view_mixed_content_run_set(Evas_Object* ewkView, bool hasRun)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);

    priv->flags.hasRunMixedContent = hasRun;

    if (hasRun)
        evas_object_smart_callback_call(ewkView, "mixedcontent,run", 0);
}
