static void einj_exec_ctx_init(struct apei_exec_context *ctx)
{
	apei_exec_ctx_init(ctx, einj_ins_type, ARRAY_SIZE(einj_ins_type),
			   EINJ_TAB_ENTRY(einj_tab), einj_tab->entries);
}
