static LayoutRect sizingBox(RenderObject* renderer)
{
    if (!renderer->isBox())
        return LayoutRect();

    RenderBox* box = toRenderBox(renderer);
    return box->style()->boxSizing() == BORDER_BOX ? box->borderBoxRect() : box->computedCSSContentBoxRect();
}
