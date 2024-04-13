static PassRefPtr<Range> collapsedToBoundary(const Range* range, bool forward)
{
    RefPtr<Range> result = range->cloneRange(ASSERT_NO_EXCEPTION);
    result->collapse(!forward, ASSERT_NO_EXCEPTION);
    return result.release();
}
