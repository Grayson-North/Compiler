Project 4: PL/0 COMPILER 
Grayson North


How to compile:
================================================================================
make
================================================================================






How to run:
================================================================================
Using a.out:
--------------------------------------------------------------------------------
	virtual machine only:
		./a.out "file_name.txt" -v
	lexeme list only:
		./a.out "file_name.txt" -l
	assembly only:
		./a.out "file_name.txt" -a
	virtual machine and lexeme list:
		./a.out "file_name.txt" -v -l
	virtual machine and assembly:
		./a.out "file_name.txt" -v -a
	assembly and lexeme list:
		./a.out "file_name.txt" -a -l
	lexeme list, assembly and virtual machine
		./a.out "file_name.txt" -v -a -l

Using tester.sh:
	./tester.sh

	or

	bash tester.sh
================================================================================
