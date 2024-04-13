void ewk_view_input_method_state_set(Evas_Object* ewkView, bool active)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET(smartData, priv);
    WebCore::Frame* focusedFrame = priv->page->focusController()->focusedOrMainFrame();

    priv->imh = 0;
    if (focusedFrame
        && focusedFrame->document()
        && focusedFrame->document()->focusedNode()
        && focusedFrame->document()->focusedNode()->hasTagName(WebCore::HTMLNames::inputTag)) {
        WebCore::HTMLInputElement* inputElement;

        inputElement = static_cast<WebCore::HTMLInputElement*>(focusedFrame->document()->focusedNode());
        if (inputElement) {
            if (!active) {
                active = inputElement->isPasswordField();
                priv->imh = inputElement->isPasswordField() * EWK_IMH_PASSWORD;
            } else {
                priv->imh |= inputElement->isTelephoneField() * EWK_IMH_TELEPHONE;
                priv->imh |= inputElement->isNumberField() * EWK_IMH_NUMBER;
                priv->imh |= inputElement->isEmailField() * EWK_IMH_EMAIL;
                priv->imh |= inputElement->isURLField() * EWK_IMH_URL;
            }
        }
    }

    evas_object_smart_callback_call(ewkView, "inputmethod,changed", (void*)active);
}
