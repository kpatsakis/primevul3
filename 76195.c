void ewk_view_paint_context_free(Ewk_View_Paint_Context* context)
{
    EINA_SAFETY_ON_NULL_RETURN(context);
    delete context->graphicContext;
    cairo_destroy(context->cr);
    free(context);
}
