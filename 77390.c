static bool isCoordinateSkiaSafe(float coord)
{
#if defined(_MSC_VER)
    if (!_finite(coord))
#else
    if (!finite(coord))
#endif
        return false;

    static const int maxPointMagnitude = 32767;
    if (coord > maxPointMagnitude || coord < -maxPointMagnitude)
        return false;

    return true;
}
