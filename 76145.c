void ewk_view_did_first_visually_nonempty_layout(Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);
    if (!priv->flags.viewCleared) {
        ewk_view_frame_main_cleared(ewkView);
        ewk_view_enable_render(ewkView);
        priv->flags.viewCleared = true;
    }
}
