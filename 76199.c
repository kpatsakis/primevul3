void ewk_view_paint_context_restore(Ewk_View_Paint_Context* context)
{
    EINA_SAFETY_ON_NULL_RETURN(context);
    context->graphicContext->restore();
    cairo_restore(context->cr);
}
