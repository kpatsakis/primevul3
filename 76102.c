static void _ewk_view_scroll_add(Ewk_View_Private_Data* priv, Evas_Coord deltaX, Evas_Coord deltaY, Evas_Coord x, Evas_Coord y, Evas_Coord width, Evas_Coord height, Eina_Bool mainScroll)
{
    Ewk_Scroll_Request* rect;
    Ewk_Scroll_Request* rect_end;
    Evas_Coord x2 = x + width, y2 = y + height;

    rect = priv->scrolls.array;
    rect_end = rect + priv->scrolls.count;
    for (; rect < rect_end; rect++) {
        if (rect->x == x && rect->y == y && rect->w == width && rect->h == height) {
            DBG("region already scrolled %d,%d+%dx%d %+03d,%+03d add "
                "%+03d,%+03d",
                rect->x, rect->y, rect->w, rect->h, rect->dx, rect->dy, deltaX, deltaY);
            rect->dx += deltaX;
            rect->dy += deltaY;
            return;
        }
        if ((x <= rect->x && x2 >= rect->x2) && (y <= rect->y && y2 >= rect->y2)) {
            DBG("old viewport (%d,%d+%dx%d %+03d,%+03d) was scrolled itself, "
                "add %+03d,%+03d",
                rect->x, rect->y, rect->w, rect->h, rect->dx, rect->dy, deltaX, deltaY);
            rect->x += deltaX;
            rect->y += deltaY;
        }
    }

    if (priv->scrolls.allocated == priv->scrolls.count) {
        size_t size;
        if (!priv->scrolls.allocated)
            size = EWK_VIEW_SCROLLS_SIZE_INITIAL;
        else
            size = priv->scrolls.allocated + EWK_VIEW_SCROLLS_SIZE_STEP;
        if (!_ewk_view_scrolls_resize(priv, size))
            return;
    }

    rect = priv->scrolls.array + priv->scrolls.count;
    priv->scrolls.count++;

    rect->x = x;
    rect->y = y;
    rect->w = width;
    rect->h = height;
    rect->x2 = x2;
    rect->y2 = y2;
    rect->dx = deltaX;
    rect->dy = deltaY;
    rect->main_scroll = mainScroll;
    DBG("add scroll in region: %d, %d+%dx%d %+03d, %+03d", x, y, width, height, deltaX, deltaY);

    Eina_Rectangle* pr;
    Eina_Rectangle* pr_end;
    size_t count;
    pr = priv->repaints.array;
    count = priv->repaints.count;
    pr_end = pr + count;
    for (; pr < pr_end; pr++) {
        pr->x += deltaX;
        pr->y += deltaY;
    }
}
