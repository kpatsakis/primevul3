static bool alwaysRequiresLineBox(RenderObject* flow)
{
    return isEmptyInline(flow) && toRenderInline(flow)->hasInlineDirectionBordersPaddingOrMargin();
}
