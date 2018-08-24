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
	     (2 . "_NBT_cross")
	     (70 . 2)	     
	     (1 . "")
	     )
  ) ;;;entmake;
  
 
  (draw_point_of_block pt) ;;;block content;
  
 
  (ENTMAKE '((0 . "Endblk"))) ;;;block end;

  
  (entmake (list '(0 . "INSERT")(cons 66 1) (cons 2 "_NBT_cross") (cons 10 pt))) ;;;entmake insert;
  
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
			  (cons 10 pt1)  ;vertex
			 '(40 . 0.1) ;width of start point
			 '(41 . 0.1) ;width of end point
			 '(42 . 1.0) ;bugle
			 '(91 . 0)
			  (cons 10 pt2) ;vertex
			 '(40 . 0.1)
			 '(41 . 0.1)
			 '(42 . 1.0)
			 '(91 . 0)
			 '(210 0.0 0.0 1.0)
		     )
	    ) ;append
  )
)



;a sample,just to be reference；
;how to make a insert with attribution text? read it;
(defun c:qqs()
	(PROGN
	  (ENTMAKE '((0 . "BLOCK")
		     (8 . "0")
		     (10 0.0 0.0 0.0)
		     (2 . "_NBT_cross")
		     (70 . 2)
		     (1 . "")
		    )
	  )
	  (ENTMAKE '((0 . "LINE")
		     (8 . "0")
		     (67 . 0)
		     (10 0.353553 -0.353553 0.0)
		     (11 -0.353553 0.353553 0.0)
		     (210 0.0 0.0 1.0)
		    )
	  )
	  (ENTMAKE '((0 . "LINE")
		     (8 . "0")
		     (10 -0.353553 -0.353553 0.0)
		     (11 0.353553 0.353553 0.0)
		    )
	  )
	  (ENTMAKE
	    '((0 . "LINE") (8 . "0") (10 0.0 0.0 0.0) (11 -1.0 0.0 0.0))
	  )
;;;	  (ENTMAKE (list '(0 . "ATTDEF")
;;;		 (cons 10 '(1 0 0))
;;;		 (cons 11 '(1 0 0))
;;;		 (cons 40 1)		 
;;;		 (cons 1 "0.1")
;;;		 (cons 3 "0.2")
;;;		 (cons 2  "heigth") ;height
;;;		 '(74 . 2)
;;;		 '(70 . 2))
;;;	  )
	  (ENTMAKE '((0 . "Endblk")))
	)

  	(setq pt(getpoint "\n Please input the insert point:"))
        (entmake (list '(0 . "INSERT")(cons 66 1) (cons 2 "_NBT_cross") (cons 10 pt)))
  
	  (entmake (list
	               '(0 . "ATTRIB")
	               '(100 . "AcDbEntity")
	               '(100 . "AcDbText")
	                (cons 10 pt)
	                (cons 40 1)
	                (cons 1  "height_val")  
	               '(100 . "AcDbAttribute")
	                (cons 2  "A")
		        ;(cons 3  "hello")
	               '(70 . 2);(70 . 1)属性字不可见  (70 . 0)属性字可见
	           )
	   )    
	  (entmake '((0 . "SEQEND")))
)


;;;  =================================================
;;;   entmake制作&插入属性块 v1.0
;;;   作者：Mr.Meng      命令:(list-count lst)    日期:2015年5月11号
;;;   功能：制作&插入属性块
;;;   由图元制作块——>制作属性块：已制作的块+属性字图元(可以连续加入多组) 错误顺序： 图元+属性字图元——>属性块 这种顺序插入的块没有属性字的
;;;   制作图元的时候，可以加入颜色组码，不然生成的图元颜色都是bylayer
;;;   需要研究坐标系trans，以免属性字和图块分离很远
;;;  =================================================
;(entmake (list '(0 . "LINE") (cons 10 pt1) (cons 11 pt2)))
;(entmake (list '(0 . "CIRCLE") (cons 10 pt) (cons 40 r)))
(defun qqt()
	(entmake (list 
	    '(0 . "block")           
	     (cons 2 "$$$$myequip$$$$0001") ;图块名
	    '(70 . 2)                 ;块具有非固定属性定义
	    '(10 0.0 0.0 0.0))        ;基点
	);block定义
	(entmake (list 
	    '(0 . "LINE") 
	    '(10 0.353553 0.353553 0.0) 
	    '(11 -0.353553 -0.353553 0.0))
	);block图元直线
	(entmake (list 
	    '(0 . "LINE") 
	    '(10 -0.353553 0.353553 0.0) 
	    '(11 0.353553 -0.353553 0.0))
	);block图元直线
	(entmake (list 
	    '(0 . "CIRCLE")
	    '(10 0.0 0.0 0.0) 
	    '(40 . 0.5))
	);block图元圆
	(entmake '((0 . "ENDBLK")));结束标志

	(entmake (list '(0 . "INSERT") '(66 . 1) (cons 2 "$$$$myequip$$$$0001") '(10 0.0 0.0 0.0) 
	                '(41 . 500.0) '(42 . 500.0) '(43 . 500.0) ))
	   (entmake (list
	               '(0 . "ATTRIB")
	               '(100 . "AcDbEntity")
	               '(100 . "AcDbText")
	               '(10 0.0 0.0 0.0)
	                (cons 40 150)
	                (cons 1 "吸顶荧光灯")
	               '(100 . "AcDbAttribute")
	                (cons 2 "A")
	               '(70 . 2);(70 . 1)属性字不可见  (70 . 0)属性字可见
	                  )
	   )    
	  (entmake '((0 . "SEQEND")))
	  (entlast)
  )
