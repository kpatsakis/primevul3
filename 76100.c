static void _ewk_view_repaints_flush(Ewk_View_Private_Data* priv)
{
    priv->repaints.count = 0;
    if (priv->repaints.allocated <= EWK_VIEW_REPAINTS_SIZE_MAX_FREE)
        return;
    _ewk_view_repaints_resize(priv, EWK_VIEW_REPAINTS_SIZE_MAX_FREE);
}
