 PassRefPtr<RenderStyle> HTMLInputElement::customStyleForRenderer()
 {
    return m_inputTypeView->customStyleForRenderer(originalStyleForRenderer());
}
