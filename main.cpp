//#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "segy_class.h"
#include "class_func.cpp"
int main () {

	segy sgy;
	//	sgy.OpenFile("velocity.segy");
	sgy.openfile();

	sgy.write_file();
	
	sgy.print_binary_header();

	sgy.print_trace_header();

	sgy.print_text_header();
	
	return 0;
}
