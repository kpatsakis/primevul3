void RenderBox::addShadowOverflow()
{
    int shadowLeft;
    int shadowRight;
    int shadowTop;
    int shadowBottom;
    style()->getBoxShadowExtent(shadowTop, shadowRight, shadowBottom, shadowLeft);
    IntRect borderBox = borderBoxRect();
    int overflowLeft = borderBox.x() + shadowLeft;
    int overflowRight = borderBox.maxX() + shadowRight;
    int overflowTop = borderBox.y() + shadowTop;
    int overflowBottom = borderBox.maxY() + shadowBottom;
    addVisualOverflow(IntRect(overflowLeft, overflowTop, overflowRight - overflowLeft, overflowBottom - overflowTop));
}
