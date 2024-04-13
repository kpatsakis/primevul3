bool Element::isUnresolvedCustomElement()
{
    return isCustomElement() && document()->registry()->isUnresolved(this);
}
