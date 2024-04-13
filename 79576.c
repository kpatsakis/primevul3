void RenderLayerCompositor::updateDirectCompositingReasons(RenderLayer* layer)
{
    CompositingReasons layerReasons = layer->compositingReasons();

    layerReasons &= ~CompositingReasonComboAllDirectReasons;
    layerReasons |= directReasonsForCompositing(layer);
    layer->setCompositingReasons(layerReasons);
}
