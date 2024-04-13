void InlineTextBox::paintDecoration(GraphicsContext* context, int tx, int ty, int deco, const ShadowData* shadow)
{
    tx += m_x;
    ty += m_y;

    if (m_truncation == cFullTruncation)
        return;

    int width = m_logicalWidth;
    if (m_truncation != cNoTruncation) {
        width = toRenderText(renderer())->width(m_start, m_truncation, textPos(), m_firstLine);
        if (!isLeftToRightDirection())
            tx += (m_logicalWidth - width);
    }
    
    Color underline, overline, linethrough;
    renderer()->getTextDecorationColors(deco, underline, overline, linethrough, true);
    
    bool isPrinting = textRenderer()->document()->printing();
    context->setStrokeThickness(1.0f); // FIXME: We should improve this rule and not always just assume 1.

    bool linesAreOpaque = !isPrinting && (!(deco & UNDERLINE) || underline.alpha() == 255) && (!(deco & OVERLINE) || overline.alpha() == 255) && (!(deco & LINE_THROUGH) || linethrough.alpha() == 255);

    RenderStyle* styleToUse = renderer()->style(m_firstLine);
    int baseline = styleToUse->font().ascent();

    bool setClip = false;
    int extraOffset = 0;
    if (!linesAreOpaque && shadow && shadow->next()) {
        context->save();
        IntRect clipRect(tx, ty, width, baseline + 2);
        for (const ShadowData* s = shadow; s; s = s->next()) {
            IntRect shadowRect(tx, ty, width, baseline + 2);
            shadowRect.inflate(s->blur());
            int shadowX = m_isVertical ? s->y() : s->x();
            int shadowY = m_isVertical ? -s->x() : s->y();
            shadowRect.move(shadowX, shadowY);
            clipRect.unite(shadowRect);
            extraOffset = max(extraOffset, max(0, shadowY) + s->blur());
        }
        context->save();
        context->clip(clipRect);
        extraOffset += baseline + 2;
        ty += extraOffset;
        setClip = true;
    }

    ColorSpace colorSpace = renderer()->style()->colorSpace();
    bool setShadow = false;
    
    do {
        if (shadow) {
            if (!shadow->next()) {
                ty -= extraOffset;
                extraOffset = 0;
            }
            int shadowX = m_isVertical ? shadow->y() : shadow->x();
            int shadowY = m_isVertical ? -shadow->x() : shadow->y();
            context->setShadow(IntSize(shadowX, shadowY - extraOffset), shadow->blur(), shadow->color(), colorSpace);
            setShadow = true;
            shadow = shadow->next();
        }

        if (deco & UNDERLINE) {
            context->setStrokeColor(underline, colorSpace);
            context->setStrokeStyle(SolidStroke);
            context->drawLineForText(IntPoint(tx, ty + baseline + 1), width, isPrinting);
        }
        if (deco & OVERLINE) {
            context->setStrokeColor(overline, colorSpace);
            context->setStrokeStyle(SolidStroke);
            context->drawLineForText(IntPoint(tx, ty), width, isPrinting);
        }
        if (deco & LINE_THROUGH) {
            context->setStrokeColor(linethrough, colorSpace);
            context->setStrokeStyle(SolidStroke);
            context->drawLineForText(IntPoint(tx, ty + 2 * baseline / 3), width, isPrinting);
        }
    } while (shadow);

    if (setClip)
        context->restore();
    else if (setShadow)
        context->clearShadow();
}
