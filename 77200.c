void Clipboard::ReadAvailableTypes(Clipboard::Buffer buffer,
                                   std::vector<string16>* types,
                                   bool* contains_filenames) const {
  if (!types || !contains_filenames) {
    NOTREACHED();
    return;
  }

  types->clear();
  if (IsFormatAvailable(GetPlainTextFormatType(), buffer))
    types->push_back(UTF8ToUTF16(kMimeTypeText));
  if (IsFormatAvailable(GetHtmlFormatType(), buffer))
    types->push_back(UTF8ToUTF16(kMimeTypeHTML));
  if (IsFormatAvailable(GetBitmapFormatType(), buffer))
    types->push_back(UTF8ToUTF16(kMimeTypePNG));
  *contains_filenames = false;
}
