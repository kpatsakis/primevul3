RenderLayer* RenderBox::enclosingFloatPaintingLayer() const
{
    const RenderObject* curr = this;
    while (curr) {
        RenderLayer* layer = curr->hasLayer() && curr->isBox() ? toRenderBoxModelObject(curr)->layer() : 0;
        if (layer && layer->isSelfPaintingLayer())
            return layer;
        curr = curr->parent();
    }
    return 0;
}
