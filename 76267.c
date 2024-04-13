const char* ewk_view_setting_user_agent_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, 0);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, 0);
    return priv->settings.userAgent;
}
