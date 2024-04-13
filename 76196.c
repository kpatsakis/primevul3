Ewk_View_Paint_Context* ewk_view_paint_context_new(Ewk_View_Private_Data* priv, cairo_t* cr)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(priv, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(cr, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(priv->mainFrame, 0);
    WebCore::FrameView* view = priv->mainFrame->view();
    EINA_SAFETY_ON_NULL_RETURN_VAL(view, 0);
    Ewk_View_Paint_Context* context = static_cast<Ewk_View_Paint_Context*>(malloc(sizeof(*context)));
    EINA_SAFETY_ON_NULL_RETURN_VAL(context, 0);

    context->graphicContext = new WebCore::GraphicsContext(cr);
    if (!context->graphicContext) {
        free(context);
        return 0;
    }
    context->view = view;
    context->cr = cairo_reference(cr);
    return context;
}
