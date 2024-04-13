void ewk_view_paint_context_save(Ewk_View_Paint_Context* context)
{
    EINA_SAFETY_ON_NULL_RETURN(context);
    cairo_save(context->cr);
    context->graphicContext->save();
}
