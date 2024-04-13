void RenderBox::absoluteQuads(Vector<FloatQuad>& quads)
{
    quads.append(localToAbsoluteQuad(FloatRect(0, 0, width(), height())));
}
