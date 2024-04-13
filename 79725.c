RenderObject* HTMLSelectElement::createRenderer(RenderStyle*)
{
    if (usesMenuList())
        return new RenderMenuList(this);
    return new RenderListBox(this);
}
