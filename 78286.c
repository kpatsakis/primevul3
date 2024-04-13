void OmniboxEditModel::ClassifyStringForPasteAndGo(
    const base::string16& text,
    AutocompleteMatch* match,
    GURL* alternate_nav_url) const {
  DCHECK(match);
  AutocompleteClassifierFactory::GetForProfile(profile_)->Classify(
      text, false, false, ClassifyPage(), match, alternate_nav_url);
}
