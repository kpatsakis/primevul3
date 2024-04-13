Eina_Bool ewk_view_pre_render_relative_radius(Evas_Object* ewkView, unsigned int number)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api->pre_render_relative_radius, false);
    float currentZoom;

    if (priv->animatedZoom.animator)
        return false;

    currentZoom = ewk_frame_page_zoom_get(smartData->main_frame);
    return smartData->api->pre_render_relative_radius(smartData, number, currentZoom);
}
