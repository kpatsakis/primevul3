bool HTMLInputElement::hasValidDataListOptions() const
{
    HTMLDataListElement* dataList = this->dataList();
    if (!dataList)
        return false;
    RefPtr<HTMLCollection> options = dataList->options();
    for (unsigned i = 0; HTMLOptionElement* option = toHTMLOptionElement(options->item(i)); ++i) {
        if (isValidValue(option->value()))
            return true;
    }
    return false;
}
