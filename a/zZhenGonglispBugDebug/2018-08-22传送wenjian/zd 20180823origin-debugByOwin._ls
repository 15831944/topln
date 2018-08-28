(princ "\nZD V2.0 By Q1548253108 已加载。")
(princ "\n输入命令： zd 进行展点。")

(defun c:zd (/ fn f s p  index_str)
  (setvar "CMDECHO" 0)
  (setq fn (getfiled "请选择坐标数据文件" "G:/" "txt" 8))
  (if (/= nil fn)
    (progn
      (setq f (open fn "r"))
      (if (/= nil f)
	(progn
	  (setvar "DIMZIN" 0)
	  (if (= nil (tblobjname "layer" "GCD"))
	    (mkgcdlayer)
	  )
	  (if (= nil (tblobjname "style" "HZ"))
	    (mkhzstyle)
	  )
	  (if (= nil (tblobjname "block" "GC200"))
	    (mkgc200blk)
	  )
	  (while (/= nil (setq s (read-line f)))
	    (setq s (vl-string-subst "," ",," s)
		  lista (parse2 s ",")
		  index_str (car lista)
		  p (cdr lista)
		  p (list (nth 0 p) (nth 1 p) (nth 2 p))
		  p (mapcar 'atof p)
	    )	    
	    (mkgc200blr p (rtos (nth 2 p) 2 3) index_str)
					;(entmake (list 
					;					 '(0 . "POINT") 
					;					 '(100 . "AcDbEntity") 
					;					 '(100 . "AcDbPoint") 
					;					 p 
					;				 )	
					;)

	  )
	  (close f)
	  (command "zoom" "e")
	)
      )
    )
  )
  (setvar "CMDECHO" 1)
  (princ "展点已经完成。")
  (princ)
)

(defun parse2 (str delim / LST POS)
  (while (setq pos (vl-string-search delim str))
    (setq lst (append lst (list (substr str 1 pos))))
    (setq str (substr str (+ 2 pos)))
  )
  (if (> (strlen str) 0)
    (append lst (list str))
    lst
  )
)

(defun mkgcdlayer nil
  (entmake '((0 . "LAYER")
	     (100 . "AcDbSymbolTableRecord")
	     (100 . "AcDbLayerTableRecord")
	     (2 . "GCD")
	     (70 . 0)
	     (62 . 7)
	     (6 . "CONTINUOUS")
	     (290 . 1)
	     (370 . -3)
	    )
  )
)
(defun mkhzstyle nil
  (entmake '((0 . "STYLE")
	     (100 . "AcDbSymbolTableRecord")
	     (100 . "AcDbTextStyleTableRecord")
	     (2 . "HZ")
	     (70 . 0)
	     (40 . 0.0)
	     (41 . 0.8)
	     (50 . 0.0)
	     (71 . 0)
	     (42 . 0.2)
	     (3 . "rs.shx")
	     (4 . "hztxt.shx")
	    )
  )
)
(defun mkgc200blk nil
  (progn (entmake '((0 . "BLOCK")
		    (100 . "AcDbEntity")
		    (67 . 0)
		    (8 . "0")
		    (100 . "AcDbBlockBegin")
		    (70 . 0)
		    (10 0.0 0.0 0.0)
		    (2 . "GC200")
		    (1 . "GC200")
		   )
	 )
	 (entmake '((0 . "LWPOLYLINE")
		    (100 . "AcDbEntity")
		    (67 . 0)
		    (8 . "0")
		    (6 . "Continuous")
		    (100 . "AcDbPolyline")
		    (90 . 2)
		    (70 . 129)
		    (43 . 0.2)
		    (38 . 0.0)
		    (39 . 0.0)
		    (10 -0.1 0.0)
		    (40 . 0.2)
		    (41 . 0.2)
		    (42 . 1.0)
		    (10 0.1 0.0)
		    (40 . 0.2)
		    (41 . 0.2)
		    (42 . 1.0)
		    (210 0.0 0.0 1.0)
		   )
	 )
	 (entmake '((0 . "ATTDEF")
		    (67 . 0)
		    (410 . "Model")
		    (8 . "GCD")
		    (6 . "Continuous")
		    (10 478916.0 2.36937e+006 11.797)
		    (40 . 1.0)
		    (1 . "11.80")
		    (50 . 0.0)
		    (41 . 0.8)
		    (51 . 0.0)
		    (7 . "HZ")
		    (71 . 0)
		    (72 . 0)
		    (11 478916.0 2.36937e+006 11.797)
		    (210 0.0 0.0 1.0)
		    (2 . "height")
		    (3 . "")
		    (62 . 256)
		    (70 . 0)
		    (73 . 0)
		    (74 . 2)
		    (280 . 0)
		   )
	 )
	 (entmake '((0 . "ENDBLK")
		    (100 . "AcDbEntity")
		    (100 . "AcDbBlockEnd")
		    (8 . "0")
		   )
	 )
  )
)
(defun mkgc200blr (p str  index_str)
  (progn


    (regapp "SOUTH")
    (entmake (list '(0 . "INSERT")
		   '(100 . "AcDbEntity")
		   '(67 . 0)
		   '(410 . "Model")
		   '(8 . "GCD")
		   '(6 . "Continuous")
		   '(100 . "AcDbBlockReference")
		   '(66 . 1)
		   '(2 . "GC200")
		   (cons 10 p)
		   '(41 . 0.5)
		   '(42 . 0.5)
		   '(43 . 0.5)
		   '(50 . 0.0)
		   '(70 . 0)
		   '(71 . 0)
		   '(44 . 0.0)
		   '(45 . 0.0)
		   '(210 0.0 0.0 1.0)
		   '(-3 ("SOUTH" (1000 . "202101")))
	     )
    )
    (entmake
      (list '(0 . "ATTRIB")
	    '(100 . "AcDbEntity")
	    '(67 . 0)
	    '(410 . "Model")
	    '(8 . "GCD")
	    '(6 . "Continuous")
	    '(100 . "AcDbText")
	    (cons 10 (list (+ 0.6 (nth 0 p)) (-  (nth 1 p) 0.6) (nth 2 p)))
	    '(40 . 0.55)
	    (cons 1 str)
	    '(50 . 0.0)
	    '(41 . 0.8)
	    '(51 . 0.0)
	    '(7 . "standard")
	    '(71 . 0)
	    '(72 . 0)
	    (cons 11 (list (+ 0.6 (nth 0 p)) (nth 1 p) (nth 2 p)))
	    '(210 0.0 0.0 1.0)
	    '(100 . "AcDbAttribute")
	    '(2 . "height")
	    '(70 . 2)
	    '(73 . 0)
	    '(74 . 2)
	    '(280 . 0)
      )      
    )
     ;index
    (entmake     
      (list '(0 . "ATTRIB")
	    '(100 . "AcDbEntity")
	    '(67 . 0)
	    '(410 . "Model")
	    '(8 . "GCD")
	    '(6 . "Continuous")
	    '(100 . "AcDbText")
	    (cons 10 (list (+ 0.6 (nth 0 p)) (+ 0.7 (nth 1 p)) (nth 2 p)))
	    '(40 . 0.55)
	    (cons 1 index_str)
	    '(50 . 0.0)
	    '(41 . 0.8)
	    '(51 . 0.0)
	    '(7 . "standard")
	    '(71 . 0)
	    '(72 . 0)
	    (cons 11 (list (+ 0.6 (nth 0 p)) (+ 0.6 (nth 1 p)) (nth 2 p)))
	    '(210 0.0 0.0 1.0)
	    '(100 . "AcDbAttribute")
	    '(2 . "index")
	    '(70 . 2)
	    '(73 . 0)
	    '(74 . 2)
	    '(280 . 0)
      )     
    )
    (entmake '((0 . "seqend")))
  )
)
(princ)