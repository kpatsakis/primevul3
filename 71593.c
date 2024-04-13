static int emitjump(JF, int opcode)
{
	int inst;
	emit(J, F, opcode);
	inst = F->codelen;
	emitarg(J, F, 0);
	return inst;
}
