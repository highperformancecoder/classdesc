# heuristics to discover location of TCL/Tk libraries, as many 
# implementations of TCL/Tk are broken.

if {$tcl_platform(platform)=="windows"} {
  set cygp [open {|cygpath -w /} "r"]
  set cygpath [gets $cygp]
  set tcl_library [lindex \
     [glob $cygpath/usr/share/tcl $cygpath/usr/share/tcl\[0-9\]*] \
     0]
  set tk_library [lindex \
     [glob $cygpath/usr/share/tk $cygpath/usr/share/tk\[0-9\]*] \
     0]
  close $cygp
  }

if {! [info exists tcl_library]} {
    if [info exists env(TCL_LIBRARY)] {set tcl_library $env(TCL_LIBRARY)} {
	set tcl_library [lindex \
	[glob ~/usr/lib/tcl ~/usr/lib/tcl\[0-9\]* /usr/lib/tcl \
		/usr/lib/tcl\[0-9\]* /usr/lib64/tcl/tcl\[0-9\].* /usr/local/lib/tcl \
		/usr/local/lib/tcl\[0-9\]* \
           /usr/share/tcl /usr/share/tcl\[0-9\]*
	] \
	0]
    }
}

if {! [info exists tk_library]} {
    if [info exists env(TK_LIBRARY)] {set tk_library $env(TK_LIBRARY)} {
	set tk_library [lindex \
	[glob  ~/usr/lib/tk ~/usr/lib/tk\[0-9\]* /usr/lib/tk \
		/usr/lib/tk\[0-9\]* /usr/lib64/tcl/tk\[0-9\]* /usr/local/lib/tk \
		/usr/local/lib/tk\[0-9\]*\
           /usr/share/tk /usr/share/tk\[0-9\]*
	]\
	0]
    }
}

source [info library]/init.tcl
source $tk_library/tk.tcl
button .resume -text "resume" -command {global paused; set paused false;}
#button .resume -text "resume" -command exit
canvas .raster
pack .resume .raster -in .

image create photo .pixmap 
.raster create image 0 0 -image .pixmap -anchor nw

