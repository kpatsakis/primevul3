static u16 GetWord(struct ngiflib_gif * g) {
	u16 r = (u16)GetByte(g);
	r |= ((u16)GetByte(g) << 8);
	return r;
}
