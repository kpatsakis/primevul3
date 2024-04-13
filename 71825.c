static void fast_csum(struct snmp_ctx *ctx, unsigned char offset)
{
	unsigned char s[12] = {0,};
	int size;

	if (offset & 1) {
		memcpy(&s[1], &ctx->from, 4);
		memcpy(&s[7], &ctx->to, 4);
		s[0] = ~0;
		s[1] = ~s[1];
		s[2] = ~s[2];
		s[3] = ~s[3];
		s[4] = ~s[4];
		s[5] = ~0;
		size = 12;
	} else {
		memcpy(&s[0], &ctx->from, 4);
		memcpy(&s[4], &ctx->to, 4);
		s[0] = ~s[0];
		s[1] = ~s[1];
		s[2] = ~s[2];
		s[3] = ~s[3];
		size = 8;
	}
	*ctx->check = csum_fold(csum_partial(s, size,
					     ~csum_unfold(*ctx->check)));
}
