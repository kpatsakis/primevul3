bool isPathSkiaSafe(const SkMatrix& transform, const SkPath& path)
{
#ifdef ENSURE_VALUE_SAFETY_FOR_SKIA
    SkPoint current_points[4];
    SkPath::Iter iter(path, false);
    for (SkPath::Verb verb = iter.next(current_points);
         verb != SkPath::kDone_Verb;
         verb = iter.next(current_points)) {
        switch (verb) {
        case SkPath::kMove_Verb:
            break;
        case SkPath::kLine_Verb:
            if (!isPointSkiaSafe(transform, current_points[0])
                || !isPointSkiaSafe(transform, current_points[1]))
                return false;
            break;
        case SkPath::kQuad_Verb:
            if (!isPointSkiaSafe(transform, current_points[0])
                || !isPointSkiaSafe(transform, current_points[1])
                || !isPointSkiaSafe(transform, current_points[2]))
                return false;
            break;
        case SkPath::kCubic_Verb:
            if (!isPointSkiaSafe(transform, current_points[0])
                || !isPointSkiaSafe(transform, current_points[1])
                || !isPointSkiaSafe(transform, current_points[2])
                || !isPointSkiaSafe(transform, current_points[3]))
                return false;
            break;
        case SkPath::kClose_Verb:
        case SkPath::kDone_Verb:
        default:
            break;
        }
    }
    return true;
#else
    return true;
#endif
}
