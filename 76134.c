static void _ewk_view_zoom_animation_start(Ewk_View_Smart_Data* smartData)
{
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);
    if (priv->animatedZoom.animator)
        return;
    priv->animatedZoom.animator = ecore_animator_add
                                       (_ewk_view_zoom_animator_cb, smartData);
}
