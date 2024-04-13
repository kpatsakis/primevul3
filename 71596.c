static void emitnumber(JF, double num)
{
	if (num == 0) {
		emit(J, F, OP_INTEGER);
		emitarg(J, F, 32768);
		if (signbit(num))
			emit(J, F, OP_NEG);
	} else {
		double nv = num + 32768;
		js_Instruction iv = nv;
		if (nv == iv) {
			emit(J, F, OP_INTEGER);
			emitarg(J, F, iv);
		} else {
			emit(J, F, OP_NUMBER);
			emitarg(J, F, addnumber(J, F, num));
		}
	}
}
