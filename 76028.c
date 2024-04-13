IntRect RenderBox::reflectionBox() const
{
    IntRect result;
    if (!style()->boxReflect())
        return result;
    IntRect box = borderBoxRect();
    result = box;
    switch (style()->boxReflect()->direction()) {
        case ReflectionBelow:
            result.move(0, box.height() + reflectionOffset());
            break;
        case ReflectionAbove:
            result.move(0, -box.height() - reflectionOffset());
            break;
        case ReflectionLeft:
            result.move(-box.width() - reflectionOffset(), 0);
            break;
        case ReflectionRight:
            result.move(box.width() + reflectionOffset(), 0);
            break;
    }
    return result;
}
