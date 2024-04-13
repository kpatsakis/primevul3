int RenderBox::lineHeight(bool /*firstLine*/, LineDirectionMode direction, LinePositionMode /*linePositionMode*/) const
{
    if (isReplaced())
        return direction == HorizontalLine ? m_marginTop + height() + m_marginBottom : m_marginRight + width() + m_marginLeft;
    return 0;
}
