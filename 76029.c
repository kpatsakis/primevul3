int RenderBox::reflectionOffset() const
{
    if (!style()->boxReflect())
        return 0;
    if (style()->boxReflect()->direction() == ReflectionLeft || style()->boxReflect()->direction() == ReflectionRight)
        return style()->boxReflect()->offset().calcValue(borderBoxRect().width());
    return style()->boxReflect()->offset().calcValue(borderBoxRect().height());
}
