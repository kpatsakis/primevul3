static int verticalPositionForBox(InlineBox* box, FontBaseline baselineType, bool firstLine, VerticalPositionCache& verticalPositionCache)
{
    if (box->renderer()->isText())
        return box->parent()->logicalTop();
    
    RenderBoxModelObject* renderer = box->boxModelObject();
    ASSERT(renderer->isInline());
    if (!renderer->isInline())
        return 0;

    if (firstLine && !renderer->document()->usesFirstLineRules())
        firstLine = false;

    bool isRenderInline = renderer->isRenderInline();
    if (isRenderInline && !firstLine) {
        int verticalPosition = verticalPositionCache.get(renderer, baselineType);
        if (verticalPosition != PositionUndefined)
            return verticalPosition;
    }

    int verticalPosition = 0;
    EVerticalAlign verticalAlign = renderer->style()->verticalAlign();
    if (verticalAlign == TOP || verticalAlign == BOTTOM)
        return 0;
   
    RenderObject* parent = renderer->parent();
    if (parent->isRenderInline() && parent->style()->verticalAlign() != TOP && parent->style()->verticalAlign() != BOTTOM)
        verticalPosition = box->parent()->logicalTop();
    
    if (verticalAlign != BASELINE) {
        const Font& font = parent->style(firstLine)->font();
        const FontMetrics& fontMetrics = font.fontMetrics();
        int fontSize = font.pixelSize();

        LineDirectionMode lineDirection = parent->style()->isHorizontalWritingMode() ? HorizontalLine : VerticalLine;

        if (verticalAlign == SUB)
            verticalPosition += fontSize / 5 + 1;
        else if (verticalAlign == SUPER)
            verticalPosition -= fontSize / 3 + 1;
        else if (verticalAlign == TEXT_TOP)
            verticalPosition += renderer->baselinePosition(baselineType, firstLine, lineDirection) - fontMetrics.ascent(baselineType);
        else if (verticalAlign == MIDDLE)
            verticalPosition += -static_cast<int>(fontMetrics.xHeight() / 2) - renderer->lineHeight(firstLine, lineDirection) / 2 + renderer->baselinePosition(baselineType, firstLine, lineDirection);
        else if (verticalAlign == TEXT_BOTTOM) {
            verticalPosition += fontMetrics.descent(baselineType);
            if (!renderer->isReplaced() || renderer->isInlineBlockOrInlineTable())
                verticalPosition -= (renderer->lineHeight(firstLine, lineDirection) - renderer->baselinePosition(baselineType, firstLine, lineDirection));
        } else if (verticalAlign == BASELINE_MIDDLE)
            verticalPosition += -renderer->lineHeight(firstLine, lineDirection) / 2 + renderer->baselinePosition(baselineType, firstLine, lineDirection);
        else if (verticalAlign == LENGTH)
            verticalPosition -= renderer->style()->verticalAlignLength().calcValue(renderer->lineHeight(firstLine, lineDirection));
    }

    if (isRenderInline && !firstLine)
        verticalPositionCache.set(renderer, baselineType, verticalPosition);

    return verticalPosition;
}
