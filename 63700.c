record_func_map(struct bpf_verifier_env *env, struct bpf_call_arg_meta *meta,
		int func_id, int insn_idx)
{
	struct bpf_insn_aux_data *aux = &env->insn_aux_data[insn_idx];

	if (func_id != BPF_FUNC_tail_call &&
	    func_id != BPF_FUNC_map_lookup_elem &&
	    func_id != BPF_FUNC_map_update_elem &&
	    func_id != BPF_FUNC_map_delete_elem)
		return 0;

	if (meta->map_ptr == NULL) {
		verbose(env, "kernel subsystem misconfigured verifier\n");
		return -EINVAL;
	}

	if (!BPF_MAP_PTR(aux->map_state))
		bpf_map_ptr_store(aux, meta->map_ptr,
				  meta->map_ptr->unpriv_array);
	else if (BPF_MAP_PTR(aux->map_state) != meta->map_ptr)
		bpf_map_ptr_store(aux, BPF_MAP_PTR_POISON,
				  meta->map_ptr->unpriv_array);
	return 0;
}
