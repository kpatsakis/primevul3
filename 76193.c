Eina_Bool ewk_view_paint_contents(Ewk_View_Private_Data* priv, cairo_t* cr, const Eina_Rectangle* area)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(priv, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(cr, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(area, false);
    WebCore::FrameView* view = priv->mainFrame->view();
    EINA_SAFETY_ON_NULL_RETURN_VAL(view, false);

    WebCore::GraphicsContext graphicsContext(cr);
    WebCore::IntRect rect(area->x, area->y, area->w, area->h);

    cairo_save(cr);
    graphicsContext.save();
    graphicsContext.clip(rect);
    if (view->isTransparent())
        graphicsContext.clearRect(rect);
    view->paintContents(&graphicsContext,  rect);
    graphicsContext.restore();
    cairo_restore(cr);

    return true;
}
