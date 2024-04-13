void ewk_view_paint_context_scale(Ewk_View_Paint_Context* context, float scaleX, float scaleY)
{
    EINA_SAFETY_ON_NULL_RETURN(context);

    context->graphicContext->scale(WebCore::FloatSize(scaleX, scaleY));
}
