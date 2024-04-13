void RenderBox::updateBoxModelInfoFromStyle()
{
    RenderBoxModelObject::updateBoxModelInfoFromStyle();

    bool isRootObject = isRoot();
    bool isViewObject = isRenderView();

    if (isRootObject || isViewObject)
        setHasBoxDecorations(true);

    setPositioned(style()->position() == AbsolutePosition || style()->position() == FixedPosition);
    setFloating(!isPositioned() && style()->isFloating());

    if (style()->overflowX() != OVISIBLE && !isRootObject && (isRenderBlock() || isTableRow() || isTableSection())) {
        bool boxHasOverflowClip = true;
        if (isBody()) {
            if (document()->documentElement()->hasTagName(htmlTag) &&
                document()->body() == node() &&
                document()->documentElement()->renderer()->style()->overflowX() == OVISIBLE)
                boxHasOverflowClip = false;
        }
        
        if (boxHasOverflowClip) {
            if (!s_hadOverflowClip)
                repaint();
            setHasOverflowClip();
        }
    }

    setHasTransform(style()->hasTransformRelatedProperty());
    setHasReflection(style()->boxReflect());
}
