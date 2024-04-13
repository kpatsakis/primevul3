void RenderBox::paintRootBoxFillLayers(const PaintInfo& paintInfo)
{
    const FillLayer* bgLayer = style()->backgroundLayers();
    Color bgColor = style()->visitedDependentColor(CSSPropertyBackgroundColor);
    RenderObject* bodyObject = 0;
    if (!hasBackground() && node() && node()->hasTagName(HTMLNames::htmlTag)) {
        HTMLElement* body = document()->body();
        bodyObject = (body && body->hasLocalName(bodyTag)) ? body->renderer() : 0;
        if (bodyObject) {
            bgLayer = bodyObject->style()->backgroundLayers();
            bgColor = bodyObject->style()->visitedDependentColor(CSSPropertyBackgroundColor);
        }
    }

    paintFillLayers(paintInfo, bgColor, bgLayer, view()->documentRect(), BackgroundBleedNone, CompositeSourceOver, bodyObject);
}
