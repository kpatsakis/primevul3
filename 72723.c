GF_Err gf_sm_load_run(GF_SceneLoader *load)
{
	if (load->process) return load->process(load);
	return GF_OK;
}
