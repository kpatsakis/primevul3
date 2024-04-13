RenderObject* HTMLButtonElement::createRenderer(RenderStyle*)
{
    return new RenderButton(this);
}
