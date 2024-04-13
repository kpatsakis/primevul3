RenderObject* HTMLTextAreaElement::createRenderer(RenderStyle*)
{
    return new RenderTextControlMultiLine(this);
}
