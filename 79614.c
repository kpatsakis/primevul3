HTMLFormControlElement* HTMLFormControlElement::enclosingFormControlElement(Node* node)
{
    for (; node; node = node->parentNode()) {
        if (node->isElementNode() && toElement(node)->isFormControlElement())
            return toHTMLFormControlElement(node);
    }
    return 0;
}
