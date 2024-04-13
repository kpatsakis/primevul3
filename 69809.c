BOOL RegionsOverlap (unsigned __int64 start1, unsigned __int64 end1, unsigned __int64 start2, unsigned __int64 end2)
{
	return (start1 < start2) ? (end1 >= start2) : (start1 <= end2);
}
