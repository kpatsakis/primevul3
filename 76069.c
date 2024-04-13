static inline bool isDenseEnoughForVector(unsigned length, unsigned numValues)
{
    return length / minDensityMultiplier <= numValues;
}
