String HTMLFormControlElement::formEnctype() const
{
    const AtomicString& formEnctypeAttr = fastGetAttribute(formenctypeAttr);
    if (formEnctypeAttr.isNull())
        return emptyString();
    return FormSubmission::Attributes::parseEncodingType(formEnctypeAttr);
}
