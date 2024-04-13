static IntPoint computeOffsetFromAbsolute(RenderLayer* layer)
{
    TransformState transformState(TransformState::ApplyTransformDirection, FloatPoint());
    layer->renderer()->mapLocalToContainer(0, transformState, ApplyContainerFlip);
    transformState.flatten();
    return roundedIntPoint(transformState.lastPlanarPoint());
}
