p1old:boxed_column{
label="P1";
:edit_box{
label="����";
key="p1oldn";
}
:edit_box{
label="���|";
key="p1olde";
}
:button{
label="ʰ ȡ";
key="getp1old";
}
}
p2old:boxed_column{
label="P2";
:edit_box{
label="����";
key="p2oldn";
}
:edit_box{
label="���|";
key="p2olde";
}
:button{
label="ʰ ȡ";
key="getp2old";
}
}
p1new:boxed_column{
label="P1";
:edit_box{
label="����";
key="p1newn";
}
:edit_box{
label="���|";
key="p1newe";
}
:text{label="ע��XY����";}
}
p2new:boxed_column{
label="P2";
:edit_box{
label="����";
key="p2newn";
}
:edit_box{
label="���|";
key="p2newe";
}
:text{label="ע��XY����";}
}
scaleset: button{
label="====>";
key="scaleset";
}
Myok_button : retirement_button{
label ="   �_��   ";
key ="accept";
is_default = true;
}
Mycancel_button : retirement_button{
label ="ȡ��";
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
get2pucs : dialog{label ="����c������ϵ��--[�W�乤����]";
:text{label="ע�⣺CADĬ�J�锵�W����ϵ,���|ΪX������ΪY��     ";}
:row{
:boxed_column{
label="�D��Դ����ϵ";
p1old;
p2old;
}
:boxed_column{
label="�D��Ŀ������ϵ";
p1new;
p2new;
}
}
:row{
:edit_box{label="Ӌ�����";key="calucssc";is_enabled=false;}
scaleset;
:edit_box{label="���H����";key="trueucssc";}
}
:row{
:edit_box{label="���x��";key="ddist";}
spacer;
ok_cancel_info;
}
:boxed_column{
label="�f��";
fixed_width = true; :text{label="����  =Դ����ϵ���x / Ŀ������ϵ���x";}
:text{label="���x��=Դ����ϵ���x - Ŀ������ϵ���x*���H����";}
:text{label="=======================================================";}
:text{label="������ʾ��Ӌ������H����������춌��H�����c�����`�Ӌ�����";is_bold=true;fixed_width=true;}
:text{label="          �������錍�H���������H������֪�������һ��Ҫ�ʴ_ݔ";}
:text{label="          �룬��Ȼ�`��̫�󡣱�������Ҫ����С���c��6λ����";}
:text{label="ע�⣺������ϵ�D�Q�D�Q���о��Ȟ�С���c��10λ";}
}
errtile;
}