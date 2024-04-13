static void _ewk_view_scrolls_flush(Ewk_View_Private_Data* priv)
{
    priv->scrolls.count = 0;
    if (priv->scrolls.allocated <= EWK_VIEW_SCROLLS_SIZE_MAX_FREE)
        return;
    _ewk_view_scrolls_resize(priv, EWK_VIEW_SCROLLS_SIZE_MAX_FREE);
}
