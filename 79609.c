void HTMLFormControlElement::didRecalcStyle(StyleRecalcChange)
{
    if (RenderObject* renderer = this->renderer())
        renderer->updateFromElement();
}
