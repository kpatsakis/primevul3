LayoutUnit RenderBox::maxPreferredLogicalWidth() const
{
    if (preferredLogicalWidthsDirty())
        const_cast<RenderBox*>(this)->computePreferredLogicalWidths();
        
    return m_maxPreferredLogicalWidth;
}
