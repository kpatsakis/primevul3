void ewk_view_paint_context_paint_contents(Ewk_View_Paint_Context* context, const Eina_Rectangle* area)
{
    EINA_SAFETY_ON_NULL_RETURN(context);
    EINA_SAFETY_ON_NULL_RETURN(area);

    WebCore::IntRect rect(area->x, area->y, area->w, area->h);

    if (context->view->isTransparent())
        context->graphicContext->clearRect(rect);

    context->view->paintContents(context->graphicContext, rect);
}
