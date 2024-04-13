PassRefPtr<ClientRectList> Element::getClientRects()
{
    document()->updateLayoutIgnorePendingStylesheets();

    RenderBoxModelObject* renderBoxModelObject = this->renderBoxModelObject();
    if (!renderBoxModelObject)
        return ClientRectList::create();


    Vector<FloatQuad> quads;
    renderBoxModelObject->absoluteQuads(quads);
    document()->adjustFloatQuadsForScrollAndAbsoluteZoom(quads, renderBoxModelObject);
    return ClientRectList::create(quads);
}
