;画块里面的圆点；
(defun draw_point_of_block(pt /)
  (setq basex (car pt))
  (setq basey (cadr pt))
  (setq pt1 (list (+ -0.05 basex) basey 0 ))
  (setq pt2 (list (+ 0.05 basex) basey 0))
  (entmake (append (list '(0 . "LWPOLYLINE")		
			 '(100 . "AcDbEntity")
			 '(67 . 0)
			 '(410 . "Model")
			 ;'(8 . "0")  ;layer name
			 '(6 . "Continuous")
			 '(100 . "AcDbPolyline")
			 '(90 . 2)
			 '(70 . 129)
			 '(43 . 0.1)
			 '(38 . 0.0) 
			 '(39 . 0.0)
			 '(10 0.05 0 0)  ;vertex
			 '(40 . 0.1) ;width of start point
			 '(41 . 0.1) ;width of end point
			 '(42 . 1.0) ;bugle
			 '(91 . 0)
			 '(10 -0.05 0 0) ;vertex
			 '(40 . 0.1)
			 '(41 . 0.1)
			 '(42 . 1.0)
			 '(91 . 0)
			 '(210 0.0 0.0 1.0)
		     )
	    ) ;append
  )
)

;;  =================================================
;;;;功能：插入一个块，块里面是一个点状多义线，宽度0.1，是一个圆形；
;;;;      块带一个属性文字，文字内容是"点"的z坐标，即"高程";文字高度为1;
;;;;提供个zd调用，zd是展点程序;
;;  =================================================
(defun makeInsertsWithHeight( pt /)
  (setq att_text(rtos (caddr pt))) ;;;获取高程;
 
  (ENTMAKE '((0 . "BLOCK")  ;;;block head;
	     (8 . "0")
	     (10 0.0 0.0 0.0)
	     (2 . "_NBT_cross_owin")
	     (70 . 2)	     
	     (1 . "")
	     )
  ) ;;;entmake;
  
 
  (draw_point_of_block pt) ;;;block content;
  
 
  (ENTMAKE '((0 . "Endblk"))) ;;;block end;

  
  (entmake (list '(0 . "INSERT")(cons 66 1) (cons 2 "_NBT_cross_owin") (cons 10 pt))) ;;;entmake insert;
  
  ;;;add attribution to block
  (entmake (list
	     '(0 . "ATTRIB")
	     '(100 . "AcDbEntity")
	     '(100 . "AcDbText")
	     (cons 10 pt)
	     (cons 40 1)  ;text height;
	     (cons 1  att_text)
	     '(100 . "AcDbAttribute")
	     (cons 2  "A")
	     ;(cons 3  "hello")
	     '(70 . 2);(70 . 1)属性字不可见  (70 . 0)属性字可见
	     )
	   )    
  (entmake '((0 . "SEQEND")))
)


;;;;test the command
(defun c:qqs()
  (setq pt (getpoint "\n Please pick the basePoint:"))
  (makeInsertsWithHeight pt)
)


