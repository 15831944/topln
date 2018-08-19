p1old:boxed_column{
label="P1";
:edit_box{
label="North";
key="p1oldn";
}
:edit_box{
label=" East";
key="p1olde";
}
:button{
label="Pick";
key="getp1old";
}
}
p2old:boxed_column{
label="P2";
:edit_box{
label="North";
key="p2oldn";
}
:edit_box{
label=" East";
key="p2olde";
}
:button{
label="Pick";
key="getp2old";
}
}
p1new:boxed_column{
label="P1";
:edit_box{
label="North";
key="p1newn";
}
:edit_box{
label=" East";
key="p1newe";
}
:text{label="Attention XY orientation";}
}
p2new:boxed_column{
label="P2";
:edit_box{
label="North";
key="p2newn";
}
:edit_box{
label=" East";
key="p2newe";
}
:text{label="Attention XY orientation";}
}
scaleset: button{
label="====>";
key="scaleset";
}
Myok_button : retirement_button{
label ="   OK   ";
key ="accept";
is_default = true;
}
Mycancel_button : retirement_button{
label ="cancel";
key ="cancel";
is_cancel = true;
}
Myhelp_button : retirement_button{
label ="help";
key ="help";
is_help = true;
}
Myinfo_button : retirement_button{
label ="info";
key ="info";
}
Myabout_button : retirement_button{
label ="about";
key ="about";
}
Mybonus_button : retirement_button{
label ="register";
key ="bonus";
}
ok_cancel_info : column{
: row{
fixed_width = true;
alignment = centered;
Myok_button;
: spacer{width = 2;}
Mycancel_button;
}
}
get2pucs : dialog{label ="°ÔTwo Point Pick°Ô--[netbee Tool]";
:text{label="Attention:CAD default to mathematical coordinate system.,East is X,North is Y. ";}
:row{
:boxed_column{
label="Source coordinate system";
p1old;
p2old;
}
:boxed_column{
label="Target coordinate system";
p1new;
p2new;
}
}
:row{
:edit_box{label="calculate scale";key="calucssc";is_enabled=false;}
scaleset;
:edit_box{label="reality scale";key="trueucssc";}
}
:row{
:edit_box{label="Distance difference";key="ddist";}
spacer;
ok_cancel_info;
}
:boxed_column{
label="Explain";
fixed_width = true; :text{label="calculate scale=(Distance of Source coordinate) / (Distance of Target coordinate)";}
:text{label="Distance difference=(Distance of Source coordinate)-(Distance of Target coordinate)*(reality scale)";}
:text{label="=======================================================";}
:text{label="Warning£∫The calculated ratio is for reference only because the actual coordinate points will be error.";is_bold=true;fixed_width=true;}
:text{label="         The percentage of calculation can not be used as the actual proportion.";}
:text{label="         If the actual proportion is known, it must be input accurately, otherwise the error is too large.";}
:text{label="         The precision of the ratio is more than 6 bits after the decimal point.";}
:text{label="  ";}
:text{label="This coordinate transformation converts all the 10 bits after the decimal point.";}
}
errtile;
}