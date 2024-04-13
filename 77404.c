void GraphicsContext::setLineCap(LineCap cap)
{
    if (paintingDisabled())
        return;
    switch (cap) {
    case ButtCap:
        platformContext()->setLineCap(SkPaint::kButt_Cap);
        break;
    case RoundCap:
        platformContext()->setLineCap(SkPaint::kRound_Cap);
        break;
    case SquareCap:
        platformContext()->setLineCap(SkPaint::kSquare_Cap);
        break;
    default:
        ASSERT(0);
        break;
    }
}
