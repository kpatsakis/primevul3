void RenderBox::mapAbsoluteToLocalPoint(bool fixed, bool useTransforms, TransformState& transformState) const
{
    ASSERT(!view() || !view()->layoutStateEnabled());
    
    bool isFixedPos = style()->position() == FixedPosition;
    bool hasTransform = hasLayer() && layer()->transform();
    if (hasTransform) {
        fixed &= isFixedPos;
    } else
        fixed |= isFixedPos;
    
    RenderObject* o = container();
    if (!o)
        return;

    o->mapAbsoluteToLocalPoint(fixed, useTransforms, transformState);

    LayoutSize containerOffset = offsetFromContainer(o, LayoutPoint());

    bool preserve3D = useTransforms && (o->style()->preserves3D() || style()->preserves3D());
    if (useTransforms && shouldUseTransformFromContainer(o)) {
        TransformationMatrix t;
        getTransformFromContainer(o, containerOffset, t);
        transformState.applyTransform(t, preserve3D ? TransformState::AccumulateTransform : TransformState::FlattenTransform);
    } else
        transformState.move(-containerOffset.width(), -containerOffset.height(), preserve3D ? TransformState::AccumulateTransform : TransformState::FlattenTransform);
}
