void ewk_view_viewport_attributes_set(Evas_Object* ewkView, const WebCore::ViewportArguments& arguments)
{
    EWK_VIEW_SD_GET(ewkView, smartData);
    EWK_VIEW_PRIV_GET(smartData, priv);

    priv->viewportArguments = arguments;
    evas_object_smart_callback_call(ewkView, "viewport,changed", 0);
}
