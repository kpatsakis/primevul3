void GetIntersection (uint64 start1, uint32 length1, uint64 start2, uint64 end2, uint64 *intersectStart, uint32 *intersectLength)
{
	uint64 end1 = start1 + length1 - 1;
	uint64 intersectEnd = (end1 <= end2) ? end1 : end2;

	*intersectStart = (start1 >= start2) ? start1 : start2;
	*intersectLength = (uint32) ((*intersectStart > intersectEnd) ? 0 : intersectEnd + 1 - *intersectStart);

	if (*intersectLength == 0)
		*intersectStart = start1;
}
