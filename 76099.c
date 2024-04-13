static void _ewk_view_repaint_add(Ewk_View_Private_Data* priv, Evas_Coord x, Evas_Coord y, Evas_Coord width, Evas_Coord height)
{
    Eina_Rectangle* rect;

    if (priv->repaints.allocated == priv->repaints.count) {
        size_t size;
        if (!priv->repaints.allocated)
            size = EWK_VIEW_REPAINTS_SIZE_INITIAL;
        else
            size = priv->repaints.allocated + EWK_VIEW_REPAINTS_SIZE_STEP;
        if (!_ewk_view_repaints_resize(priv, size))
            return;
    }

    rect = priv->repaints.array + priv->repaints.count;
    priv->repaints.count++;

    rect->x = x;
    rect->y = y;
    rect->w = width;
    rect->h = height;

    DBG("add repaint %d, %d+%dx%d", x, y, width, height);
}
