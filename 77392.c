static bool isPointSkiaSafe(const SkMatrix& transform, const SkPoint& pt)
{
#ifdef ENSURE_VALUE_SAFETY_FOR_SKIA
    SkPoint xPt;
    transform.mapPoints(&xPt, &pt, 1);
    return isCoordinateSkiaSafe(xPt.fX) && isCoordinateSkiaSafe(xPt.fY);
#else
    return true;
#endif
}
