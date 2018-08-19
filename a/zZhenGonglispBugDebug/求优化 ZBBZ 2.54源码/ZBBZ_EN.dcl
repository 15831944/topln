Image: image_button{
color=0;
aspect_ratio=1.5;
key="ImagePreview";
height=16;
}
Imagebonus: image_button{
color=255;
aspect_ratio=1.0;
key="ImageBonus";
height=10.5;
}
ok_help : column{
: row{
fixed_width = true;
alignment = centered;
ok_button;
: spacer{width = 2;}
help_button;
}
}
dcl_settings :default_dcl_settings{audit_level=3;}
zb_c : boxed_radio_row{
key ="ZB_C";
label ="Coord prefix";
: radio_button{
label ="XY";
key ="ZB_C_XY";
value="1";
}
: radio_button{
label ="AB";
key ="ZB_C_AB";
}
: radio_button{
label ="NE";
key ="ZB_C_NE";
}
: radio_button{
label ="None";
key ="ZB_C_NO";
}
}
zb_la:row{
fixed_width = true; :text{label="  DimLayer";}
:popup_list{
edit_width = 24;
key ="dimzb_layer";
}
}
zb_ucs:radio_column{
: radio_button{
label ="ActiveCoordSystem";
key ="zb_ucs_act";
}
: radio_button{
label ="WorldCoordSystem";
key ="ZB_ucs_word";
}
: radio_button{
label ="CustomCoordSystem";
key ="ZB_ucs_cus";
}
}
set_th:row{
fixed_width = true; :text{label =" TextHight ";}
:edit_box{
key ="ZB_th";
edit_width=11;
}
}
set_jd:row{
fixed_width = true; :text{
width=4;
label=" Precision ";
}
:popup_list{
edit_width = 10;
list ="0\n0.0\n0.00\n0.000\n0.0000\n0.00000\n0.000000\n0.0000000\n0.00000000";
key ="dimzb_jd";
}
}
set_bl:row{
fixed_width = true; width=10;
:edit_box{
label="  DimScale";
key="dim_bl";
edit_width=11;
}
: slider{
key="Dim_bL_Sl";
min_value=-10;
max_value=10;
small_increment=1;
layout=vertical;
}
}
set_ang:row{
fixed_width = true; width=10;
:edit_box{
label="   azimuth  ";
key="dimzb_an";
edit_width=11;
}
: button{
key="get_angle";
width=4;
label="Pick";
}
}
cmd_cu:column{
:edit_box{
label="BasePtN=";
edit_width=12;
key="ucs_N";
}
:edit_box{
label="BasePtE=";
key="ucs_E";
edit_width=12;
}
:edit_box{
label="Rotation=";
key="ucs_a";
edit_width=12;
}
:button{
label="BasePointAndAngle";
key="get_ucs1";
}
:button{
label="Two Point Pick";
key="get_ucs2";
}
}
dimtstyle:row{
fixed_width = true; :text{
label=" textStyle  ";
}
:popup_list{
edit_width = 10;
key ="dimzb_tstyle";
}
}
dimpstyle:row{
fixed_width = true; :text{
label="ArrowStyle";
}
:popup_list{
edit_width = 10;
key ="dimpoitstyle";
}
:edit_box{
key="dimpsize";
edit_width=4;
}
:text{
label="Size";
}
}
cfile:row{
:button{label="Export DAT file";key="creatfile";}
}
fxauto: toggle{
label="AutoOrnt";
key="AutoOrientation";
}
xy_yx: toggle{
label="X<->Y";
key="yx_T";
}
NB_PSTYLE: toggle{
label="GroupsON";
key="nb_pstyle";
}
NB_wipeout: toggle{
label="BackgroundFill(2005up)";
key="nb_wipeout";
}
Myok_button : retirement_button{
label ="  OK  ";
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
zbh:boxed_column{
label="Number";
:row{
fixed_width = true ;
width = 10 ;
:edit_box{
label ="prefix";
key ="Dimzb_BHPrev";
edit_width = 6;
}
:edit_box{
label ="Active";
key ="Dimzb_ActBH";
edit_width = 4;
}
:edit_box{
label ="step";
key ="dimzb_addbh";
edit_width = 2 ;
}
}
:radio_row{
key="bhtest";
: radio_button{
label ="None";
key ="nobh";
}
: radio_button{
label ="Top";
key ="bhtop";
is_enabled=false;
}
: radio_button{
label ="Linetail";
key ="bhline";
is_enabled=false;
}
}
}
ok_help_info : column{
: row{
fixed_width = true;
alignment = centered;
Myok_button;
: spacer{width = 2;}
Myhelp_button;
}
}
info:row{
:text{label ="Please use WeChat scan qr code to bonus. Support QQ:14997-84427 QQGroup:5821780 email:ymf5340@163.com";key="myinfo";}
}
version:row{
:text{label ="       【DimCoord 2.54 Free by NetbeeTool】";key="version";}
}
language:boxed_column{
label ="language";
:popup_list{
list ="简体中文\nEnglish\n繁體中文";
key ="V_languagelist";
}
}
SetDimZB : dialog{label ="★DimCoordSettings★--[DimCoord 2.54 Free By NetBeeTool]";
:row{
:boxed_column{
children_alignment=left;
label="CoordSystem";
zb_ucs;
cmd_cu;
Imagebonus;
}
:column{
:row{
children_alignment=top;
:column{
Image;
:row{
:column{
zb_c; zbh;}
:column{
xy_yx;
NB_PSTYLE;
fxauto;
:spacer{height=1;}
:spacer{height=1;}
}
}
}
:column{
fixed_height = true;
zb_la; dimpstyle; dimtstyle; set_jd; set_ang; set_bl; set_th;
NB_wipeout;
language;
cfile;
:spacer{height=1;}
ok_help_info;
}
}
}
}
dcl_settings;
info;
errtile;
}
