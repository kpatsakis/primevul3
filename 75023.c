bool InlineFlowBox::requiresIdeographicBaseline(const GlyphOverflowAndFallbackFontsMap& textBoxDataMap) const
{
    if (isHorizontal())
        return false;
    
    if (renderer()->style(m_firstLine)->fontDescription().textOrientation() == TextOrientationUpright
        || renderer()->style(m_firstLine)->font().primaryFont()->hasVerticalGlyphs())
        return true;

    for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
        if (curr->renderer()->isPositioned())
            continue; // Positioned placeholders don't affect calculations.
        
        if (curr->isInlineFlowBox()) {
            if (static_cast<InlineFlowBox*>(curr)->requiresIdeographicBaseline(textBoxDataMap))
                return true;
        } else {
            if (curr->renderer()->style(m_firstLine)->font().primaryFont()->hasVerticalGlyphs())
                return true;
            
            const Vector<const SimpleFontData*>* usedFonts = 0;
            if (curr->isInlineTextBox()) {
                GlyphOverflowAndFallbackFontsMap::const_iterator it = textBoxDataMap.find(static_cast<InlineTextBox*>(curr));
                usedFonts = it == textBoxDataMap.end() ? 0 : &it->second.first;
            }

            if (usedFonts) {
                for (size_t i = 0; i < usedFonts->size(); ++i) {
                    if (usedFonts->at(i)->hasVerticalGlyphs())
                        return true;
                }
            }
        }
    }
    
    return false;
}
