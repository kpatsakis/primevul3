void GraphicsContext::fillPath(const Path& pathToFill)
{
    if (paintingDisabled())
        return;

    SkPath path = *pathToFill.platformPath();
    if (!isPathSkiaSafe(getCTM(), path))
      return;

    const GraphicsContextState& state = m_state;
    path.setFillType(state.fillRule == RULE_EVENODD ?
        SkPath::kEvenOdd_FillType : SkPath::kWinding_FillType);

    SkPaint paint;
    platformContext()->setupPaintForFilling(&paint);

    platformContext()->canvas()->drawPath(path, paint);
}
