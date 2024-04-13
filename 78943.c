void Element::removeAllEventListeners()
{
    ContainerNode::removeAllEventListeners();
    if (ElementShadow* shadow = this->shadow())
        shadow->removeAllEventListeners();
}
