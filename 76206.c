void ewk_view_pre_render_cancel(Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api->pre_render_cancel);
    smartData->api->pre_render_cancel(smartData);
}
