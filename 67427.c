void signal_setup_done(int failed, struct ksignal *ksig, int stepping)
{
	if (failed)
		force_sigsegv(ksig->sig, current);
	else
		signal_delivered(ksig, stepping);
}
