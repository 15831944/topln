p1old:boxed_column{
label="P1";
:edit_box{
label="正北";
key="p1oldn";
}
:edit_box{
label="正東";
key="p1olde";
}
:button{
label="拾 取";
key="getp1old";
}
}
p2old:boxed_column{
label="P2";
:edit_box{
label="正北";
key="p2oldn";
}
:edit_box{
label="正東";
key="p2olde";
}
:button{
label="拾 取";
key="getp2old";
}
}
p1new:boxed_column{
label="P1";
:edit_box{
label="正北";
key="p1newn";
}
:edit_box{
label="正東";
key="p1newe";
}
:text{label="注意XY方向";}
}
p2new:boxed_column{
label="P2";
:edit_box{
label="正北";
key="p2newn";
}
:edit_box{
label="正東";
key="p2newe";
}
:text{label="注意XY方向";}
}
scaleset: button{
label="====>";
key="scaleset";
}
Myok_button : retirement_button{
label ="   確定   ";
key ="accept";
is_default = true;
}
Mycancel_button : retirement_button{
label ="取消";
key ="cancel";
is_cancel = true;
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
get2pucs : dialog{label ="★兩點定坐標系★--[網蜂工具箱]";
:text{label="注意：CAD默認為數學坐標系,正東为X，正北为Y，     ";}
:row{
:boxed_column{
label="圖中源坐標系";
p1old;
p2old;
}
:boxed_column{
label="圖中目標坐標系";
p1new;
p2new;
}
}
:row{
:edit_box{label="計算比例";key="calucssc";is_enabled=false;}
scaleset;
:edit_box{label="實際比例";key="trueucssc";}
}
:row{
:edit_box{label="距離差";key="ddist";}
spacer;
ok_cancel_info;
}
:boxed_column{
label="說明";
fixed_width = true; :text{label="比例  =源坐標系距離 / 目標坐標系距離";}
:text{label="距離差=源坐標系距離 - 目標坐標系距離*實際比例";}
:text{label="=======================================================";}
:text{label="鄭重提示：計算比例僅供參考，由於實際座標點會有誤差，計算比例";is_bold=true;fixed_width=true;}
:text{label="          不能作為實際比例，實際比例已知的情况下一定要準確輸";}
:text{label="          入，不然誤差太大。比例精度要求在小數點後6位以上";}
:text{label="注意：此坐標系轉換轉換所有精度為小數點後10位";}
}
errtile;
}