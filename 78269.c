static inline HTMLFormControlElement* submitElementFromEvent(const Event* event)
{
    for (Node* node = event->target()->toNode(); node; node = node->parentNode()) {
        if (node->isElementNode() && toElement(node)->isFormControlElement())
            return toHTMLFormControlElement(node);
    }
    return 0;
}
