void HTMLSelectElement::setOption(unsigned index, HTMLOptionElement* option, ExceptionState& exceptionState)
{
    if (index > maxSelectItems - 1)
        index = maxSelectItems - 1;
    int diff = index - length();
    RefPtr<HTMLElement> before = 0;
    if (diff > 0) {
        setLength(index, exceptionState);
    } else if (diff < 0) {
        before = toHTMLElement(options()->item(index+1));
        remove(index);
    }
    if (!exceptionState.hadException()) {
        add(option, before.get(), exceptionState);
        if (diff >= 0 && option->selected())
            optionSelectionStateChanged(option, true);
    }
}
