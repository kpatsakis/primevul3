float TextAutosizer::computeAutosizedFontSize(float specifiedSize, float multiplier)
{
    const float pleasantSize = 16;

    const float gradientAfterPleasantSize = 0.5;

    float computedSize;
    if (specifiedSize <= pleasantSize)
        computedSize = multiplier * specifiedSize;
    else {
        computedSize = multiplier * pleasantSize + gradientAfterPleasantSize * (specifiedSize - pleasantSize);
        if (computedSize < specifiedSize)
            computedSize = specifiedSize;
    }
    return computedSize;
}
