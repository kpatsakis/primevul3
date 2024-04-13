FloatQuad RenderBox::absoluteContentQuad() const
{
    IntRect rect = contentBoxRect();
    return localToAbsoluteQuad(FloatRect(rect));
}
