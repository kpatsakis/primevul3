void ewk_view_paint_context_translate(Ewk_View_Paint_Context* context, float x, float y)
{
    EINA_SAFETY_ON_NULL_RETURN(context);

    context->graphicContext->translate(x, y);
}
