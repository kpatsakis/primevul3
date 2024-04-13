bool RenderBox::shrinkToAvoidFloats() const
{
    if ((isInline() && !isHTMLMarquee()) || !avoidsFloats() || isFloating())
        return false;

    return style()->width().isAuto(); 
}
