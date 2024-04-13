Vector<Animation*>* Element::activeAnimations() const
{
    if (!elementRareData())
        return 0;
    return elementRareData()->activeAnimations();
}
