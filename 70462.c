static void printstr(struct printparam* pp, char* str){
	stdpr(pp, str, str?(int)strlen(str):0);
}
