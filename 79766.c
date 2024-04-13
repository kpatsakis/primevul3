void HTMLTextAreaElement::parseAttribute(const QualifiedName& name, const AtomicString& value)
{
    if (name == rowsAttr) {
        int rows = value.toInt();
        if (rows <= 0)
            rows = defaultRows;
        if (m_rows != rows) {
            m_rows = rows;
            if (renderer())
                renderer()->setNeedsLayoutAndPrefWidthsRecalc();
        }
    } else if (name == colsAttr) {
        int cols = value.toInt();
        if (cols <= 0)
            cols = defaultCols;
        if (m_cols != cols) {
            m_cols = cols;
            if (renderer())
                renderer()->setNeedsLayoutAndPrefWidthsRecalc();
        }
    } else if (name == wrapAttr) {
        WrapMethod wrap;
        if (equalIgnoringCase(value, "physical") || equalIgnoringCase(value, "hard") || equalIgnoringCase(value, "on"))
            wrap = HardWrap;
        else if (equalIgnoringCase(value, "off"))
            wrap = NoWrap;
        else
            wrap = SoftWrap;
        if (wrap != m_wrap) {
            m_wrap = wrap;
            if (renderer())
                renderer()->setNeedsLayoutAndPrefWidthsRecalc();
        }
    } else if (name == accesskeyAttr) {
    } else if (name == maxlengthAttr)
        setNeedsValidityCheck();
    else
        HTMLTextFormControlElement::parseAttribute(name, value);
}
