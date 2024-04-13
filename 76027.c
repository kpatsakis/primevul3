IntRect RenderBox::reflectedRect(const IntRect& r) const
{
    if (!style()->boxReflect())
        return IntRect();

    IntRect box = borderBoxRect();
    IntRect result = r;
    switch (style()->boxReflect()->direction()) {
        case ReflectionBelow:
            result.setY(box.maxY() + reflectionOffset() + (box.maxY() - r.maxY()));
            break;
        case ReflectionAbove:
            result.setY(box.y() - reflectionOffset() - box.height() + (box.maxY() - r.maxY()));
            break;
        case ReflectionLeft:
            result.setX(box.x() - reflectionOffset() - box.width() + (box.maxX() - r.maxX()));
            break;
        case ReflectionRight:
            result.setX(box.maxX() + reflectionOffset() + (box.maxX() - r.maxX()));
            break;
    }
    return result;
}
