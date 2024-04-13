static inline bool isCombiningVoicedSoundMark(UChar character)
{
    switch (character) {
    case 0x3099: // COMBINING KATAKANA-HIRAGANA VOICED SOUND MARK
    case 0x309A: // COMBINING KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK
        return true;
    }
    return false;
}
