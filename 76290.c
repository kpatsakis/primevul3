void ewk_view_viewport_attributes_get(const Evas_Object* ewkView, int* width, int* height, float* initScale, float* maxScale, float* minScale, float* devicePixelRatio, Eina_Bool* userScalable)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);
    WebCore::ViewportAttributes attributes = _ewk_view_viewport_attributes_compute(priv);

    if (width)
        *width = attributes.layoutSize.width();
    if (height)
        *height = attributes.layoutSize.height();
    if (initScale)
        *initScale = attributes.initialScale;
    if (maxScale)
        *maxScale = attributes.maximumScale;
    if (minScale)
        *minScale = attributes.minimumScale;
    if (devicePixelRatio)
        *devicePixelRatio = attributes.devicePixelRatio;
    if (userScalable)
        *userScalable = static_cast<bool>(attributes.userScalable);
}
