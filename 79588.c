bool HTMLButtonElement::appendFormData(FormDataList& formData, bool)
{
    if (m_type != SUBMIT || name().isEmpty() || !m_isActivatedSubmit)
        return false;
    formData.appendData(name(), value());
    return true;
}
