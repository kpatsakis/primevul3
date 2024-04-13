void ewk_view_popup_selected_set(Evas_Object* ewkView, int index)
{
    INF("ewkView=%p", ewkView);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);
    EINA_SAFETY_ON_NULL_RETURN(priv->popup.menuClient);

    priv->popup.menuClient->valueChanged(index);
}
