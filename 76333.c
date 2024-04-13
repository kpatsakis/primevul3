void InlineTextBox::paint(PaintInfo& paintInfo, int tx, int ty)
{
    if (isLineBreak() || !paintInfo.shouldPaintWithinRoot(renderer()) || renderer()->style()->visibility() != VISIBLE ||
        m_truncation == cFullTruncation || paintInfo.phase == PaintPhaseOutline || !m_len)
        return;

    ASSERT(paintInfo.phase != PaintPhaseSelfOutline && paintInfo.phase != PaintPhaseChildOutlines);

    int leftOverflow = parent()->x() - parent()->leftVisualOverflow();
    int rightOverflow = parent()->rightVisualOverflow() - (parent()->x() + parent()->logicalWidth());
    int xPos = tx + m_x - leftOverflow;
    int w = logicalWidth() + leftOverflow + rightOverflow;
    if (xPos >= paintInfo.rect.right() || xPos + w <= paintInfo.rect.x())
        return;

    bool isPrinting = textRenderer()->document()->printing();
    
    bool haveSelection = !isPrinting && paintInfo.phase != PaintPhaseTextClip && selectionState() != RenderObject::SelectionNone;
    if (!haveSelection && paintInfo.phase == PaintPhaseSelection)
        return;

    if (m_truncation != cNoTruncation) {
        if (renderer()->containingBlock()->style()->isLeftToRightDirection() != isLeftToRightDirection()) {
            int widthOfVisibleText = toRenderText(renderer())->width(m_start, m_truncation, textPos(), m_firstLine);
            int widthOfHiddenText = m_logicalWidth - widthOfVisibleText;
            if (!m_isVertical)
                tx += isLeftToRightDirection() ? widthOfHiddenText : -widthOfHiddenText;
            else
                ty += isLeftToRightDirection() ? widthOfHiddenText : -widthOfHiddenText;
        }
    }

    GraphicsContext* context = paintInfo.context;

    RenderStyle* styleToUse = renderer()->style(m_firstLine);
    int baseline = styleToUse->font().ascent();
    ty -= styleToUse->isFlippedLinesWritingMode() ? baseline : 0;
    IntPoint textOrigin(m_x + tx, m_y + ty + baseline);
    
    if (m_isVertical) {
        context->save();
        context->translate(textOrigin.x(), textOrigin.y());
        context->rotate(static_cast<float>(deg2rad(90.)));
        context->translate(-textOrigin.x(), -textOrigin.y());
    }
    
    bool containsComposition = renderer()->node() && renderer()->frame()->editor()->compositionNode() == renderer()->node();
    bool useCustomUnderlines = containsComposition && renderer()->frame()->editor()->compositionUsesCustomUnderlines();

    int d = styleToUse->textDecorationsInEffect();
    const Font& font = styleToUse->font();

    if (paintInfo.phase != PaintPhaseSelection && paintInfo.phase != PaintPhaseTextClip && !isPrinting) {
#if PLATFORM(MAC)
        if (styleToUse->highlight() != nullAtom && !context->paintingDisabled())
            paintCustomHighlight(tx, ty, styleToUse->highlight());
#endif

        if (containsComposition && !useCustomUnderlines)
            paintCompositionBackground(context, tx, ty, styleToUse, font,
                renderer()->frame()->editor()->compositionStart(),
                renderer()->frame()->editor()->compositionEnd());

        paintDocumentMarkers(context, tx, ty, styleToUse, font, true);

        if (haveSelection && !useCustomUnderlines)
            paintSelection(context, tx, ty, styleToUse, font);
    }

    Color textFillColor;
    Color textStrokeColor;
    float textStrokeWidth = styleToUse->textStrokeWidth();
    const ShadowData* textShadow = paintInfo.forceBlackText ? 0 : styleToUse->textShadow();

    if (paintInfo.forceBlackText) {
        textFillColor = Color::black;
        textStrokeColor = Color::black;
    } else {
        textFillColor = styleToUse->visitedDependentColor(CSSPropertyWebkitTextFillColor);
        
        if (styleToUse->forceBackgroundsToWhite())
            textFillColor = correctedTextColor(textFillColor, Color::white);

        textStrokeColor = styleToUse->visitedDependentColor(CSSPropertyWebkitTextStrokeColor);
        
        if (styleToUse->forceBackgroundsToWhite())
            textStrokeColor = correctedTextColor(textStrokeColor, Color::white);
    }

    bool paintSelectedTextOnly = (paintInfo.phase == PaintPhaseSelection);
    bool paintSelectedTextSeparately = false;

    Color selectionFillColor = textFillColor;
    Color selectionStrokeColor = textStrokeColor;
    float selectionStrokeWidth = textStrokeWidth;
    const ShadowData* selectionShadow = textShadow;
    if (haveSelection) {
        Color foreground = paintInfo.forceBlackText ? Color::black : renderer()->selectionForegroundColor();
        if (foreground.isValid() && foreground != selectionFillColor) {
            if (!paintSelectedTextOnly)
                paintSelectedTextSeparately = true;
            selectionFillColor = foreground;
        }

        if (RenderStyle* pseudoStyle = renderer()->getCachedPseudoStyle(SELECTION)) {
            const ShadowData* shadow = paintInfo.forceBlackText ? 0 : pseudoStyle->textShadow();
            if (shadow != selectionShadow) {
                if (!paintSelectedTextOnly)
                    paintSelectedTextSeparately = true;
                selectionShadow = shadow;
            }

            float strokeWidth = pseudoStyle->textStrokeWidth();
            if (strokeWidth != selectionStrokeWidth) {
                if (!paintSelectedTextOnly)
                    paintSelectedTextSeparately = true;
                selectionStrokeWidth = strokeWidth;
            }

            Color stroke = paintInfo.forceBlackText ? Color::black : pseudoStyle->visitedDependentColor(CSSPropertyWebkitTextStrokeColor);
            if (stroke != selectionStrokeColor) {
                if (!paintSelectedTextOnly)
                    paintSelectedTextSeparately = true;
                selectionStrokeColor = stroke;
            }
        }
    }

    const UChar* characters = textRenderer()->text()->characters() + m_start;
    int length = m_len;
    BufferForAppendingHyphen charactersWithHyphen;
    if (hasHyphen())
        adjustCharactersAndLengthForHyphen(charactersWithHyphen, styleToUse, characters, length);

    TextRun textRun(characters, length, textRenderer()->allowTabs(), textPos(), m_toAdd, !isLeftToRightDirection(), m_dirOverride || styleToUse->visuallyOrdered());

    int sPos = 0;
    int ePos = 0;
    if (paintSelectedTextOnly || paintSelectedTextSeparately)
        selectionStartEnd(sPos, ePos);

    if (m_truncation != cNoTruncation) {
        sPos = min<int>(sPos, m_truncation);
        ePos = min<int>(ePos, m_truncation);
        length = m_truncation;
    }

    if (!paintSelectedTextOnly) {
        if (textStrokeWidth > 0)
            context->save();

        updateGraphicsContext(context, textFillColor, textStrokeColor, textStrokeWidth, styleToUse->colorSpace());
        if (!paintSelectedTextSeparately || ePos <= sPos) {
            paintTextWithShadows(context, font, textRun, 0, length, length, textOrigin, m_x + tx, m_y + ty, logicalWidth(), logicalHeight(), textShadow, textStrokeWidth > 0, m_isVertical);
        } else
            paintTextWithShadows(context, font, textRun, ePos, sPos, length, textOrigin, m_x + tx, m_y + ty, logicalWidth(), logicalHeight(), textShadow, textStrokeWidth > 0, m_isVertical);

        if (textStrokeWidth > 0)
            context->restore();
    }

    if ((paintSelectedTextOnly || paintSelectedTextSeparately) && sPos < ePos) {
        if (selectionStrokeWidth > 0)
            context->save();

        updateGraphicsContext(context, selectionFillColor, selectionStrokeColor, selectionStrokeWidth, styleToUse->colorSpace());
        paintTextWithShadows(context, font, textRun, sPos, ePos, length, textOrigin, m_x + tx, m_y + ty, logicalWidth(), logicalHeight(), selectionShadow, selectionStrokeWidth > 0, m_isVertical);

        if (selectionStrokeWidth > 0)
            context->restore();
    }

    if (d != TDNONE && paintInfo.phase != PaintPhaseSelection) {
        updateGraphicsContext(context, textFillColor, textStrokeColor, textStrokeWidth, styleToUse->colorSpace());
        paintDecoration(context, tx, ty, d, textShadow);
    }

    if (paintInfo.phase == PaintPhaseForeground) {
        paintDocumentMarkers(context, tx, ty, styleToUse, font, false);

        if (useCustomUnderlines) {
            const Vector<CompositionUnderline>& underlines = renderer()->frame()->editor()->customCompositionUnderlines();
            size_t numUnderlines = underlines.size();

            for (size_t index = 0; index < numUnderlines; ++index) {
                const CompositionUnderline& underline = underlines[index];

                if (underline.endOffset <= start())
                    continue;
                
                if (underline.startOffset <= end()) {
                    paintCompositionUnderline(context, tx, ty, underline);
                    if (underline.endOffset > end() + 1)
                        break;
                } else
                    break;
            }
        }
    }
    
    if (m_isVertical)
        context->restore();
}
