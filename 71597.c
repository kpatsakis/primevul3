static void emitstring(JF, int opcode, const char *str)
{
	emit(J, F, opcode);
	emitarg(J, F, addstring(J, F, str));
}
