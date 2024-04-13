void InlineFlowBox::computeOverflow(int lineTop, int lineBottom, bool strictMode, GlyphOverflowAndFallbackFontsMap& textBoxDataMap)
{
    int topOverflow = max(logicalTop(), lineTop);
    int bottomOverflow = min(logicalBottom(), lineBottom);
    
    IntRect logicalLayoutOverflow(enclosingIntRect(FloatRect(logicalLeft(), topOverflow, logicalWidth(), bottomOverflow - topOverflow)));
    IntRect logicalVisualOverflow(logicalLayoutOverflow);
  
    addBoxShadowVisualOverflow(logicalVisualOverflow);

    for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
        if (curr->renderer()->isPositioned())
            continue; // Positioned placeholders don't affect calculations.
        
        if (curr->renderer()->isText()) {
            InlineTextBox* text = static_cast<InlineTextBox*>(curr);
            RenderText* rt = toRenderText(text->renderer());
            if (rt->isBR())
                continue;
            addTextBoxVisualOverflow(text, textBoxDataMap, logicalVisualOverflow);
        } else  if (curr->renderer()->isRenderInline()) {
            InlineFlowBox* flow = static_cast<InlineFlowBox*>(curr);
            flow->computeOverflow(lineTop, lineBottom, strictMode, textBoxDataMap);
            if (!flow->boxModelObject()->hasSelfPaintingLayer())
                logicalVisualOverflow.unite(flow->logicalVisualOverflowRect());
            IntRect childLayoutOverflow = flow->logicalLayoutOverflowRect();
            childLayoutOverflow.move(flow->boxModelObject()->relativePositionLogicalOffset());
            logicalLayoutOverflow.unite(childLayoutOverflow);
        } else
            addReplacedChildOverflow(curr, logicalLayoutOverflow, logicalVisualOverflow);
    }
    
    setOverflowFromLogicalRects(logicalLayoutOverflow, logicalVisualOverflow);
}
