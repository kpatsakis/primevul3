PassRefPtr<ClientRect> Element::getBoundingClientRect()
{
    document()->updateLayoutIgnorePendingStylesheets();

    Vector<FloatQuad> quads;
    if (isSVGElement() && renderer() && !renderer()->isSVGRoot()) {
        SVGElement* svgElement = toSVGElement(this);
        FloatRect localRect;
        if (svgElement->getBoundingBox(localRect))
            quads.append(renderer()->localToAbsoluteQuad(localRect));
    } else {
        if (renderBoxModelObject())
            renderBoxModelObject()->absoluteQuads(quads);
    }

    if (quads.isEmpty())
        return ClientRect::create();

    FloatRect result = quads[0].boundingBox();
    for (size_t i = 1; i < quads.size(); ++i)
        result.unite(quads[i].boundingBox());

    document()->adjustFloatRectForScrollAndAbsoluteZoom(result, renderer());
    return ClientRect::create(result);
}
