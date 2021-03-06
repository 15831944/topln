(defun c:zd (/ bnm fn f s p)
 (setvar "CMDECHO" 0)
 (setq attreq (getvar "ATTREQ") attdia (getvar "ATTDIA"))
 (setvar "ATTREQ" 1) (setvar "ATTDIA" 0)
 (setq bnm "GC200")
 (if (setq fn (getfiled "请选择坐标数据文件" "G:/" "txt" 8)) (progn
  (if (null (tblsearch "STYLE" "HZ"))
   (entmake
    '((0 . "STYLE")
     (100 . "AcDbSymbolTableRecord")
     (100 . "AcDbTextStyleTableRecord")
     (2 . "HZ")
     (70 . 0)
     (40 . 0.0)
     (41 . 0.7)
     (50 . 0.0)
     (71 . 0)
     (42 . 1.0)
     (3 . "ROMANS,HZTXT")
     (4 . "")
    )
   )
  )
  (if (null (tblsearch "BLOCK" bnm)) (progn
   (entmake (list '(0 . "BLOCK") (cons 2 bnm) '(70 . 2) '(10 0.0 0.0 0.0)))
   (entmake
    (list
     '(0 . "LWPOLYLINE")
     '(100 . "AcDbEntity")
     '(67 . 0)
     '(8 . "0")
     '(6 . "Continuous")
     '(100 . "AcDbPolyline")
     '(90 . 2)
     '(70 . 129)
     '(43 . 0.1)
     '(38 . 0.0)
     '(39 . 0.0)
     '(10 -0.05 0.0)
     '(40 . 0.1)
     '(41 . 0.1)
     '(42 . 1.0)
     '(10 0.05 0.0)
     '(40 . 0.1)
     '(41 . 0.1)
     '(42 . 1.0)
    )
   )
   (entmake
    (list
     '(0 . "ATTDEF")
     '(100 . "AcDbEntity")
     '(67 . 0)
     '(8 . "GCD")
     '(100 . "AcDbText")
     '(10 0.0 0.0 0.0)
     '(40 . 1.0)
     '(1 . "0")
     '(50 . 0.0)
     '(41 . 0.8)
     '(51 . 0.0)
     '(7 . "HZ")
     '(71 . 0)
     '(72 . 0)
     '(11 0.6 0.0 0.0)
     '(100 . "AcDbAttributeDefinition")
     '(280 . 0)
     '(3 . "高程")
     '(2 . "height")
     '(70 . 0)
     '(73 . 0)
     '(74 . 2)
     '(280 . 0)
    )
   )
   (entmake '((0 . "ENDBLK")))
  ))  
  (if (setq f (open fn "r")) (progn
   (while (setq s (read-line f))
    (setq p (cdr (read(strcat "(" (vl-string-translate "," " " s) ")"))))
    (command "_.-INSERT" bnm "_non" p 1 1 0 (rtos (last p) 2 3))
   )
   (close f)
   (command "zoom" "e")
  ))
 ))
 (setvar "ATTREQ" attreq)
 (setvar "ATTDIA" attdia)
 (setvar "CMDECHO" 1)
 (prin1)
)
(princ "\nzd.lsp已经加载。")
(princ "\n输入命令： zd 进行展点。")
