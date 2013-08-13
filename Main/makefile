# Makefile for eM Bee 3 Junior
MB3LIB=F:\Work\Produce\mb3hook\freeware\dll\mb3dll.lib

CC=gcc -Zomf -Zmt -mprobe  $(MB3LIB) -o
pCC=gcc -c -Zomf -Zmt -o
LINK=-lstdcpp
OBPATH=.\objs
OBJS=$(OBPATH)\mainprog.obj\
	$(OBPATH)\FreeDlg.obj\
	$(OBPATH)\ErrorDlg.obj\
	$(OBPATH)\SwitchList.obj\
	$(OBPATH)\paint.obj\
	$(OBPATH)\open_folder.obj\
	$(OBPATH)\thread.obj

all: MB3Jr.exe

MB3Jr.exe: $(OBJS) resources.RES MB3Jr.DEF
	$(CC) MB3Jr.exe $(OBJS) $(MB3LIB) resources.RES MB3Jr.DEF $(LINK)
	rc resources.res MB3Jr.exe

$(OBPATH)\mainprog.obj: mainprog.cpp interface.h\
                        MB3_Exits.h\
                        FreeDlg.h ErrorDlg.h
	$(pCC) $(OBPATH)\mainprog.obj mainprog.cpp

$(OBPATH)\FreeDlg.obj: FreeDlg.cpp interface.h
	$(pCC) $(OBPATH)\FreeDlg.obj FreeDlg.cpp

$(OBPATH)\ErrorDlg.obj: ErrorDlg.cpp interface.h ErrorDlg.h
	$(pCC) $(OBPATH)\ErrorDlg.obj ErrorDlg.cpp

$(OBPATH)\SwitchList.obj: SwitchList.cpp SwitchList.h
	$(pCC) $(OBPATH)\SwitchList.obj SwitchList.cpp
	
$(OBPATH)\paint.obj: paint.cpp
	$(pCC) 	$(OBPATH)\paint.obj 	paint.cpp

$(OBPATH)\open_folder.obj: open_folder.cpp open_folder.h
	$(pCC) 	$(OBPATH)\open_folder.obj 	open_folder.cpp
	
$(OBPATH)\thread.obj: thread.cpp thread.h
	$(pCC)	$(OBPATH)\thread.obj thread.cpp
