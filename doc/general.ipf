:p.
:hp2.eM Bee 3 Junior!:ehp2. intercepts messages generated by clicking the middle button
( mouse button 3 ) on your mouse. This is done via something called a system hook, wich means
that :hp2.Junior:ehp2. takes a peek at whats happening before any other window/process gets
to act upon it.
:p.
When :hp3.Junior!:ehp3. intercepts these event no-one else can act upon them, however
you can :link reftype=hd res=7003.disable:elink. :hp3.Junior!:ehp3. without having to exit.
:p.
After mouse button 3 has been pressed, :hp2.Junior!:ehp2. takes a look at what :link reftype=fn refid=win_type.kind:elink.
of window it is.
.br
:hp2.Junior!:ehp2. distinguishes between containers and other windows:
:p.
:hp3.Containers:ehp3.
:p.
Emulates a doubleclick. In effect this either opens an object, or shows the WarpCenter.
:p.
:hp3.Others:ehp3.
:p.
Show Window list

:fn id=win_type.
There are several kinds of windows. You've got ordinary frame windows, buttons, scrollbars etc.
:efn.