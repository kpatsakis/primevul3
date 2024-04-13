static void paintScrollbar(Scrollbar* scrollbar, GraphicsContext& context, const IntRect& clip)
{
    if (!scrollbar)
        return;

    context.save();
    const IntRect& scrollbarRect = scrollbar->frameRect();
    context.translate(-scrollbarRect.x(), -scrollbarRect.y());
    IntRect transformedClip = clip;
    transformedClip.moveBy(scrollbarRect.location());
    scrollbar->paint(&context, transformedClip);
    context.restore();
}
