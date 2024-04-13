string16 PromptBrowserLoginFunction::GetLoginMessage() {
  using l10n_util::GetStringUTF16;
  using l10n_util::GetStringFUTF16;

  string16 message;
  message = ASCIIToUTF16("<p>")
      + GetStringUTF16(IDS_WEB_STORE_LOGIN_INTRODUCTION_1)
      + ASCIIToUTF16("</p>");
  message = message + ASCIIToUTF16("<p>")
      + GetStringFUTF16(IDS_WEB_STORE_LOGIN_INTRODUCTION_2,
                        GetStringUTF16(IDS_PRODUCT_NAME))
      + ASCIIToUTF16("</p>");
  return message;
}
