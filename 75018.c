float InlineFlowBox::placeBoxesInInlineDirection(float logicalLeft, bool& needsWordSpacing, GlyphOverflowAndFallbackFontsMap& textBoxDataMap)
{
    setLogicalLeft(logicalLeft);
  
    float startLogicalLeft = logicalLeft;
    logicalLeft += borderLogicalLeft() + paddingLogicalLeft();
    
    for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
        if (curr->renderer()->isText()) {
            InlineTextBox* text = static_cast<InlineTextBox*>(curr);
            RenderText* rt = toRenderText(text->renderer());
            if (rt->textLength()) {
                if (needsWordSpacing && isSpaceOrNewline(rt->characters()[text->start()]))
                    logicalLeft += rt->style(m_firstLine)->font().wordSpacing();
                needsWordSpacing = !isSpaceOrNewline(rt->characters()[text->end()]);
            }
            text->setLogicalLeft(logicalLeft);
            logicalLeft += text->logicalWidth();
        } else {
            if (curr->renderer()->isPositioned()) {
                if (curr->renderer()->parent()->style()->isLeftToRightDirection())
                    curr->setLogicalLeft(logicalLeft);
                else
                    curr->setLogicalLeft(root()->block()->logicalWidth() - logicalLeft);
                continue; // The positioned object has no effect on the width.
            }
            if (curr->renderer()->isRenderInline()) {
                InlineFlowBox* flow = static_cast<InlineFlowBox*>(curr);
                logicalLeft += flow->marginLogicalLeft();
                logicalLeft = flow->placeBoxesInInlineDirection(logicalLeft, needsWordSpacing, textBoxDataMap);
                logicalLeft += flow->marginLogicalRight();
            } else if (!curr->renderer()->isListMarker() || toRenderListMarker(curr->renderer())->isInside()) {
                int logicalLeftMargin = isHorizontal() ? curr->boxModelObject()->marginLeft() : curr->boxModelObject()->marginTop();
                int logicalRightMargin = isHorizontal() ? curr->boxModelObject()->marginRight() : curr->boxModelObject()->marginBottom();
                
                logicalLeft += logicalLeftMargin;
                curr->setLogicalLeft(logicalLeft);
                logicalLeft += curr->logicalWidth() + logicalRightMargin;
            }
        }
    }

    logicalLeft += borderLogicalRight() + paddingLogicalRight();
    setLogicalWidth(logicalLeft - startLogicalLeft);
    return logicalLeft;
}
