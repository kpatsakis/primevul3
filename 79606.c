void HTMLFormControlElement::attach(const AttachContext& context)
{
    HTMLElement::attach(context);

    if (!renderer())
        return;

    renderer()->updateFromElement();

    if (shouldAutofocusOnAttach(this)) {
        setAutofocused();
        document().setAutofocusElement(this);
    }
}
