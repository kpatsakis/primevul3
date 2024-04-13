static int valueForRepeatRule(int rule)
{
    switch (rule) {
        case RepeatImageRule:
            return CSSValueRepeat;
        case RoundImageRule:
            return CSSValueRound;
        case SpaceImageRule:
            return CSSValueSpace;
        default:
            return CSSValueStretch;
    }
}
