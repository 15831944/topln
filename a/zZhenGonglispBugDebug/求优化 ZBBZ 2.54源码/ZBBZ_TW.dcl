Image: image_button{
color=0;
aspect_ratio=1.3;
key="ImagePreview";
height=18;
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
label ="ǰ�Y";
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
label ="�o";
key ="ZB_C_NO";
}
}
zb_la:row{
fixed_width = true; :text{label="���]�D��";}
:popup_list{
edit_width = 24;
key ="dimzb_layer";
}
}
zb_ucs:radio_column{
: radio_button{
label ="��ǰ����ϵ";
key ="zb_ucs_act";
}
: radio_button{
label ="��������ϵ";
key ="ZB_ucs_word";
}
: radio_button{
label ="�Զ��x����ϵ";
key ="ZB_ucs_cus";
}
}
set_th:row{
fixed_width = true; :text{label ="    �ָ�   ";}
:edit_box{
key ="ZB_th";
edit_width=11;
}
}
set_jd:row{
fixed_width = true; :text{
width=4;
label="    ����   ";
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
label="���]����";
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
label="  ��λ��  ";
key="dimzb_an";
edit_width=11;
}
: button{
key="get_angle";
width=4;
label="ʰȡ";
}
}
cmd_cu:column{
:edit_box{
label="���cN=";
edit_width=12;
key="ucs_N";
}
:edit_box{
label="���cE=";
key="ucs_E";
edit_width=12;
}
:edit_box{
label="���D��";
key="ucs_a";
edit_width=12;
}
:button{
label="��֪���c���Ƕ�";
key="get_ucs1";
}
:button{
label="��֪ ���c ʰȡ";
key="get_ucs2";
}
}
dimtstyle:row{
fixed_width = true; :text{
label="���֘�ʽ";
}
:popup_list{
edit_width = 10;
key ="dimzb_tstyle";
}
}
dimpstyle:row{
fixed_width = true; :text{
label="��ͷ��ʽ";
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
label="��С";
}
}
cfile:row{
:button{label="����DAT�ļ�";key="creatfile";}
}
fxauto: toggle{
label="�ԄӓQ��";
key="AutoOrientation";
}
xy_yx: toggle{
label="�v�M���Q";
key="yx_T";
}
NB_PSTYLE: toggle{
label="���M�_";
key="nb_pstyle";
}
NB_wipeout: toggle{
label="��������(2005������)";
key="nb_wipeout";
}
Myok_button : retirement_button{
label ="  �_��  ";
key ="accept";
is_default = true;
}
Mycancel_button : retirement_button{
label ="ȡ��";
key ="cancel";
is_cancel = true;
}
Myhelp_button : retirement_button{
label ="����";
key ="help";
is_help = true;
}
Myinfo_button : retirement_button{
label ="��Ϣ";
key ="info";
}
Myabout_button : retirement_button{
label ="�P�";
key ="about";
}
Mybonus_button : retirement_button{
label ="ע��";
key ="bonus";
}
zbh:boxed_column{
label="��̖";
:row{
fixed_width = true ;
width = 10 ;
:edit_box{
label ="ǰ�Y";
key ="Dimzb_BHPrev";
edit_width = 6;
}
:edit_box{
label =" ��ǰ";
key ="Dimzb_ActBH";
edit_width = 4;
}
:edit_box{
label =" ���L";
key ="dimzb_addbh";
edit_width = 2 ;
}
}
:radio_row{
key="bhtest";
: radio_button{
label ="����ע";
key ="nobh";
}
: radio_button{
label ="�Ϸ�";
key ="bhtop";
is_enabled=false;
}
: radio_button{
label ="�M��β";
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
:text{label ="Ո��΢�Œ�����S�a���p";key="myinfo";}
}
language:boxed_column{
label ="language";
:popup_list{
list ="��������\nEnglish\n���w����";
key ="V_languagelist";
}
}
SetDimZB : dialog{label ="�����˘��]�O�á�--[�W�乤��-���˘��] 2.54 ���M��]";
:row{
:boxed_column{
children_alignment=left;
label="����ϵ";
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
