void ewk_view_paint_context_clip(Ewk_View_Paint_Context* context, const Eina_Rectangle* area)
{
    EINA_SAFETY_ON_NULL_RETURN(context);
    EINA_SAFETY_ON_NULL_RETURN(area);
    context->graphicContext->clip(WebCore::IntRect(area->x, area->y, area->w, area->h));
}
