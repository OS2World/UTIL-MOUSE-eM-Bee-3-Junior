OBJ=freehook.obj



MB3DLL.DLL: $(OBJ) mb3DLL.def
	@echo Linker____________________________________:
	@gcc -Zdll -Zomf -o MB3DLL.DLL $(OBJ) mb3DLL.def
	@echo Fjerner gammel DLL fra ..\Main____________:
	@del ..\Main\*.DLL
	@echo Kopierer DLL______________________________:
	@copy MB3DLL.DLL ..\Main
	@echo Lager lib_________________________________:
	@if exist MB3DLL.lib del mb3dll.lib
	@emximp -o MB3DLL.lib MB3DLL.DEF
	@copy *.lib ..\main
	@echo ferdig.

freehook.obj: freehook.c
	@echo Kompilerer freehook.c_____________________:
	@gcc -c -Zomf freehook.c
	@echo ferdig.

