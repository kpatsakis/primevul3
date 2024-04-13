void ewk_view_load_progress_changed(Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);

    double progress = priv->page->progress()->estimatedProgress();

    DBG("ewkView=%p (p=%0.3f)", ewkView, progress);

    evas_object_smart_callback_call(ewkView, "load,progress", &progress);
}
