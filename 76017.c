void RenderBox::paintFillLayers(const PaintInfo& paintInfo, const Color& c, const FillLayer* fillLayer, const LayoutRect& rect,
    BackgroundBleedAvoidance bleedAvoidance, CompositeOperator op, RenderObject* backgroundObject)
{
    if (!fillLayer)
        return;

    paintFillLayers(paintInfo, c, fillLayer->next(), rect, bleedAvoidance, op, backgroundObject);
    paintFillLayer(paintInfo, c, fillLayer, rect, bleedAvoidance, op, backgroundObject);
}
