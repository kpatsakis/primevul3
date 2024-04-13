static u32 GetNbBits(u32 MaxVal)
{
	u32 k=0;
	while ((s32) MaxVal > ((1<<k)-1) ) k+=1;
	return k;
}
