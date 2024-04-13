static bool shouldAddBorderPaddingMargin(RenderObject* child, bool &checkSide)
{
    if (!child || (child->isText() && !toRenderText(child)->textLength()))
        return true;
    checkSide = false;
    return checkSide;
}
