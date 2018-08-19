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
label ="前缀";
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
label ="无";
key ="ZB_C_NO";
}
}
zb_la:row{
fixed_width = true; :text{label="标注图层";}
:popup_list{
edit_width = 24;
key ="dimzb_layer";
}
}
zb_ucs:radio_column{
: radio_button{
label ="当前坐标系";
key ="zb_ucs_act";
}
: radio_button{
label ="世界坐标系";
key ="ZB_ucs_word";
}
: radio_button{
label ="自定义坐标系";
key ="ZB_ucs_cus";
}
}
set_th:row{
fixed_width = true; :text{label ="    字高   ";}
:edit_box{
key ="ZB_th";
edit_width=11;
}
}
set_jd:row{
fixed_width = true; :text{
width=4;
label="    精度   ";
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
label="标注比例";
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
label="  方位角  ";
key="dimzb_an";
edit_width=11;
}
: button{
key="get_angle";
width=4;
label="拾取";
}
}
cmd_cu:column{
:edit_box{
label="基点N=";
edit_width=12;
key="ucs_N";
}
:edit_box{
label="基点E=";
key="ucs_E";
edit_width=12;
}
:edit_box{
label="旋转角";
key="ucs_a";
edit_width=12;
}
:button{
label="已知基点及角度";
key="get_ucs1";
}
:button{
label="已知 两点 拾取";
key="get_ucs2";
}
}
dimtstyle:row{
fixed_width = true; :text{
label="文字样式";
}
:popup_list{
edit_width = 10;
key ="dimzb_tstyle";
}
}
dimpstyle:row{
fixed_width = true; :text{
label="箭头样式";
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
label="大小";
}
}
cfile:row{
:button{label="生成DAT文件";key="creatfile";}
}
fxauto: toggle{
label="自动换向";
key="AutoOrientation";
}
xy_yx: toggle{
label="纵横互换";
key="yx_T";
}
NB_PSTYLE: toggle{
label="编组开";
key="nb_pstyle";
}
NB_wipeout: toggle{
label="背景遮罩(2005及以上)";
key="nb_wipeout";
}
Myok_button : retirement_button{
label ="  确定  ";
key ="accept";
is_default = true;
}
Mycancel_button : retirement_button{
label ="取消";
key ="cancel";
is_cancel = true;
}
Myhelp_button : retirement_button{
label ="帮助";
key ="help";
is_help = true;
}
Myinfo_button : retirement_button{
label ="信息";
key ="info";
}
Myabout_button : retirement_button{
label ="关于";
key ="about";
}
Mybonus_button : retirement_button{
label ="注册";
key ="bonus";
}
zbh:boxed_column{
label="编号";
:row{
fixed_width = true ;
width = 10 ;
:edit_box{
label ="前缀";
key ="Dimzb_BHPrev";
edit_width = 6;
}
:edit_box{
label =" 当前";
key ="Dimzb_ActBH";
edit_width = 4;
}
:edit_box{
label =" 步长";
key ="dimzb_addbh";
edit_width = 2 ;
}
}
:radio_row{
key="bhtest";
: radio_button{
label ="不标注";
key ="nobh";
}
: radio_button{
label ="上方";
key ="bhtop";
is_enabled=false;
}
: radio_button{
label ="横线尾";
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
:text{label ="请用微信扫描二维码打赏。 email:y340@163.com";key="myinfo";}
}
language:boxed_column{
label ="language";
:popup_list{
list ="简体中文\nEnglish\n繁體中文";
key ="V_languagelist";
}
}
SetDimZB : dialog{label ="★坐标标注设置★--[网蜂工具-坐标标注 2.54 免费版]";
:row{
:boxed_column{
children_alignment=left;
label="坐标系";
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
