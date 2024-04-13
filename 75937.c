IntRect RenderBox::absoluteContentBox() const
{
    IntRect rect = contentBoxRect();
    FloatPoint absPos = localToAbsolute(FloatPoint());
    rect.move(absPos.x(), absPos.y());
    return rect;
}
