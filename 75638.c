bool SubsetterImpl::HasName(const char* font_name, Font* font) {
  UnicodeString font_string = UnicodeString::fromUTF8(font_name);
  if (font_string.isEmpty())
    return false;
  UnicodeString regular_suffix = UnicodeString::fromUTF8(" Regular");
  UnicodeString alt_font_string = font_string;
  alt_font_string += regular_suffix;

  typedef std::map<int32_t, UnicodeString> NameMap;
  NameMap names;
  NameTablePtr name_table = down_cast<NameTable*>(font->GetTable(Tag::name));

  for (int32_t i = 0; i < name_table->NameCount(); ++i) {
    switch(name_table->NameId(i)) {
      case NameId::kFontFamilyName:
      case NameId::kFontSubfamilyName:
      case NameId::kFullFontName:
      case NameId::kPreferredFamily:
      case NameId::kPreferredSubfamily:
      case NameId::kWWSFamilyName:
      case NameId::kWWSSubfamilyName: {
        int32_t hash_code = HashCode(name_table->PlatformId(i),
                                     name_table->EncodingId(i),
                                     name_table->LanguageId(i),
                                     name_table->NameId(i));
        UChar* name_part = name_table->Name(i);
        ConstructName(name_part, &(names[hash_code]), name_table->NameId(i));
        break;
      }
      default:
        break;
    }
  }

  if (!names.empty()) {
    for (NameMap::iterator b = names.begin(), e = names.end(); b != e; ++b) {
      if (b->second.caseCompare(font_string, 0) == 0 ||
          b->second.caseCompare(alt_font_string, 0) == 0) {
        return true;
      }
    }
  }
  return false;
}
