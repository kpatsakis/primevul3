static struct bpf_prog *bpf_patch_insn_data(struct bpf_verifier_env *env, u32 off,
					    const struct bpf_insn *patch, u32 len)
{
	struct bpf_prog *new_prog;

	new_prog = bpf_patch_insn_single(env->prog, off, patch, len);
	if (!new_prog)
		return NULL;
	if (adjust_insn_aux_data(env, new_prog->len, off, len))
		return NULL;
	adjust_subprog_starts(env, off, len);
	return new_prog;
}
