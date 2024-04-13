LayoutUnit RenderBox::minPreferredLogicalWidth() const
{
    if (preferredLogicalWidthsDirty())
        const_cast<RenderBox*>(this)->computePreferredLogicalWidths();
        
    return m_minPreferredLogicalWidth;
}
