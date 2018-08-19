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
label ="前綴";
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
label ="無";
key ="ZB_C_NO";
}
}
zb_la:row{
fixed_width = true; :text{label="標註圖層";}
:popup_list{
edit_width = 24;
key ="dimzb_layer";
}
}
zb_ucs:radio_column{
: radio_button{
label ="當前坐標系";
key ="zb_ucs_act";
}
: radio_button{
label ="世界坐標系";
key ="ZB_ucs_word";
}
: radio_button{
label ="自定義坐標系";
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
label="標註比例";
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
label="基點N=";
edit_width=12;
key="ucs_N";
}
:edit_box{
label="基點E=";
key="ucs_E";
edit_width=12;
}
:edit_box{
label="旋轉角";
key="ucs_a";
edit_width=12;
}
:button{
label="已知基點及角度";
key="get_ucs1";
}
:button{
label="已知 两點 拾取";
key="get_ucs2";
}
}
dimtstyle:row{
fixed_width = true; :text{
label="文字樣式";
}
:popup_list{
edit_width = 10;
key ="dimzb_tstyle";
}
}
dimpstyle:row{
fixed_width = true; :text{
label="箭头樣式";
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
label="自動換向";
key="AutoOrientation";
}
xy_yx: toggle{
label="縱橫互換";
key="yx_T";
}
NB_PSTYLE: toggle{
label="編組開";
key="nb_pstyle";
}
NB_wipeout: toggle{
label="背景遮罩(2005及以上)";
key="nb_wipeout";
}
Myok_button : retirement_button{
label ="  確定  ";
key ="accept";
is_default = true;
}
Mycancel_button : retirement_button{
label ="取消";
key ="cancel";
is_cancel = true;
}
Myhelp_button : retirement_button{
label ="幫助";
key ="help";
is_help = true;
}
Myinfo_button : retirement_button{
label ="信息";
key ="info";
}
Myabout_button : retirement_button{
label ="關於";
key ="about";
}
Mybonus_button : retirement_button{
label ="注册";
key ="bonus";
}
zbh:boxed_column{
label="編號";
:row{
fixed_width = true ;
width = 10 ;
:edit_box{
label ="前綴";
key ="Dimzb_BHPrev";
edit_width = 6;
}
:edit_box{
label =" 當前";
key ="Dimzb_ActBH";
edit_width = 4;
}
:edit_box{
label =" 步長";
key ="dimzb_addbh";
edit_width = 2 ;
}
}
:radio_row{
key="bhtest";
: radio_button{
label ="不標注";
key ="nobh";
}
: radio_button{
label ="上方";
key ="bhtop";
is_enabled=false;
}
: radio_button{
label ="橫線尾";
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
:text{label ="請用微信掃描二維碼打賞";key="myinfo";}
}
language:boxed_column{
label ="language";
:popup_list{
list ="简体中文\nEnglish\n繁體中文";
key ="V_languagelist";
}
}
SetDimZB : dialog{label ="★坐標標註設置★--[網蜂工具-坐標標註 2.54 免費版]";
:row{
:boxed_column{
children_alignment=left;
label="坐標系";
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
