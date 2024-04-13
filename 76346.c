int InlineTextBox::textPos() const
{
    if (x() == 0)
        return 0;
        
    RenderBlock* blockElement = renderer()->containingBlock();
    return !isLeftToRightDirection() ? x() - blockElement->borderRight() - blockElement->paddingRight()
                      : x() - blockElement->borderLeft() - blockElement->paddingLeft();
}
