UScriptCode NormalizeScript(UScriptCode code) {
  switch (code) {
    case USCRIPT_KATAKANA:
    case USCRIPT_HIRAGANA:
    case USCRIPT_KATAKANA_OR_HIRAGANA:
    case USCRIPT_HANGUL:  // This one is arguable.
      return USCRIPT_HAN;
    default:
      return code;
  }
}
