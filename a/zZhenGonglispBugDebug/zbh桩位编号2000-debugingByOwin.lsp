;文字随鼠标移动;
(defun sx9 (#k #heigth  /  %k i gr n pt)
 (setq %k T  ;循环条件
       i nil ;初始设置
 )
 (while %k
  (setq  gr (grread t 4 0);;取得鼠标操作及坐标
         n (car gr)       ;;鼠标操作
         pt (cadr gr)     ;;鼠标坐标
  )
  (if (= n 5);;没有操作
   (progn
    (if (/= i nil) (entdel i) );;如果有文字就删除
    (entmake 
    (list 
    '(0 . "TEXT")      ;;文字
    (cons 1 #k)        ;;文字内容
    (cons 40 #heigth)      ;;文字高度
    (cons 41 0.7)      ;;文字倾斜度
    ;(cons 50 #rotate)
    (cons 10 pt)       ;;文字插入点
    (cons 7 "standard");; 文字样式
    (cons 8 "0")       ;;文字图层
    ));;重新生成文字
    (setq i (entlast));;得到文字图元名
   )
  )
  (if (= n 3) (setq %k nil) );;3表示左键;结束循环
  (if (or (= n 2) (= n 25));;2表示空格;25表示右键;结束循环并删除文字
   (progn
    (setq %k nil)
    (entdel i)
   )
  )
 )
)  ; end of defun；

;写桩号;
(setq bh_began 1)
(setq text_offset 3.0)
(setq Newtextsize (getvar "textsize"))
(setq texthome "")
(setq textend "")

(defun c:zbh (/ error_old textstyle_old zbh_ok bho dimzin_old TEXTSIZE_old
		old_lay netbeetmp1 netbeetmp2　pt pt1
	     )
  (prompt "桩位自动编号标注程序\n")

  ;; 初始化
  (setini_bh)			       ; 用while控制标注的循环,回车退出标注
  (while (progn
	   (while (progn
		    (setq pt nil
			  netbeetmp1 nil
			  netbeetmp1 (strcat "\n选择标注点" " [前缀(M):"
					     texthome "/后缀(E):" textend
					     "/字高(H):" (rtos Newtextsize)
					     "/偏移量(O):" (rtos text_offset)
					     "/编号(N):" (itoa bh_began)
					     "/选择工程桩(Z)]:"
				     ) ; _结束strcat

		    )		       ; _结束setq
		    (initget "M E H O N Z")
		    (setq pt (getpoint netbeetmp1))
		    (if (= (type pt) 'STR)
		      (cond
			((= pt "M")
			  (settexthome)
			  (not nil)
			)
			((= pt "E")
			  (settextend)
			  (not nil)
			)
			((= pt "H")
			  (settexth)
			  (not nil)
			)
			((= pt "O")
			  (textoffset)
			  (not nil)
			)
			((= pt "N")
			  (settextbegan)
			  (not nil)
			)
			((= pt "Z")
			  (zbh_zw)
			  (not nil)
			)
			(t
			  t
			)
		      )		       ; _结束cond

		      (not t)
		    )		       ; _结束if

		  )		       ; _结束progn
	   )			       ; _结束while
	   (if (not pt)
	     (not t)
	     (not nil)
	   )			       ; _结束if
	 )			       ; _结束progn

    ;; 关闭捕捉方式
    (setq osmode_old (getvar "osmode"))
    (setvar "osmode" 0)
    (setq pt1 (list (+ (car pt) text_offset) (+ (cadr pt) text_offset))) ; _
				       ; 结束setq

    ;; 将编号加上前后缀
    (setq zbh_ok (strcat texthome (itoa bh_began) textend))

    ;(command "_.text" "s" "ZBH" pt1 Newtextsize "0d" zbh_ok) ; (setq bho
				       ; (entlast))
				       ; (command "move" bho "" pt pt1)
    (sx9 zbh_ok Newtextsize ) ;写桩号文字;
    (setq bh_began (1+ bh_began))

    ;; 恢复捕捉方式
    (setvar "osmode" osmode_old)
  )				       ; _结束while

  (setclose_bh)
)
;;; _结束defun

;;; --------------------------------------------
;;; 按桩位编号
(defun zbh_zw (/ sszwd sszw sslen loop_i loop_j)
  (setq sszw (ssget '((0 . "CIRCLE")   ; (8 . "桩位")
		    )
	     )			       ; _结束ssget

  )				       ; _结束setq
  (setq sslen (sslength sszw)
	loop_i 1.0
  )				       ; _结束setq
  (if (< sslen 1000)
    (prompt "\n 正在进行计算,请稍等...\n")
    (prompt "\n 数据过多，计算时间可能需要几分钟，请等待......\n")
  )				       ; 提取所有桩位坐标到表sszwd
  (while (<= loop_i sslen)
    (setq sszwd (append
		  sszwd
		  (list (cdr (assoc 10 (entget (ssname sszw (1- loop_i))))))
		)		       ; _结束append

    )				       ; _结束setq
    (setq loop_i (1+ loop_i))
  )				       ; _结束while
				       ; 给表sszwd根据从左到右,从上到下的
				       ; 规则排序(插入法).
  (zbh_px2)			       ; 逐一从表sszwd中提取坐标进行标注
  (setq osmode_old (getvar "osmode"))
  (setvar "osmode" 0)
  (setq loop_i 0)
  (while (<= loop_i (1- sslen))	       ; 关闭捕捉方式
    (setq pt (nth loop_i sszwd))
    (setq pt1 (list (+ (car pt) text_offset) (+ (cadr pt) text_offset))) ; _
				       ; 结束setq

    ;; 将编号加上前后缀
    (setq zbh_ok (strcat texthome (itoa bh_began) textend))

    (command "_.text" "s" "ZBH" pt1 Newtextsize "0d" zbh_ok) ; (setq bho
				       ; (entlast))
				       ; (command "move" bho "" pt pt1)
    (setq bh_began (1+ bh_began))

    ;; 恢复捕捉方式
    (setq loop_i (1+ loop_i))
  )				       ; _结束while
  (setvar "osmode" osmode_old)


)
;;; _结束defun


;;; ----------------------------------------
;;; 表list_o内第I,第J元素换
;;; (listIJ_JI list_o list_i list_j)
(defun listij_ji (list_o list_i list_j / ltmep ltemp1 ltemp2 loop_i loop_j)
  (setq loop_i 0
	loop_j 0
	listlen (length list_o)
	ltemp (list (getvar "CDATE"))
	ltemp1 (nth list_i list_o)
	ltemp2 (nth list_j list_o)


  )				       ; _结束setq
  (setq list_o (subst
		 ltemp
		 ltemp1
		 list_o
	       )
  )
  (setq list_o (subst
		 ltemp1
		 ltemp2
		 list_o
	       )
  )
  (setq list_o (subst
		 ltemp2
		 ltemp
		 list_o
	       )
  )
  (eval 'list_o)


)
;;; _结束defun

;;; --------------------------------------------------------
;;; 桩位编号初始化
(defun setini_bh ()
  (setq textstyle_old (getvar "textstyle"))
  (setvar "cmdecho" 0)
  (setq old_lay (getvar "clayer"))
  (setq dimzin_old (getvar "Dimzin"))
  (setvar "dimzin" 8)
  (setq error_old *error*)
  (gc)				       ; 判断有无图层ZBH
  (if (= (tblobjname "LAYER" "ZBH") nil)
    (progn
      (entmake (list '(0 . "LAYER") '(100 . "AcDbSymbolTableRecord") '
		     (100 . "AcDbLayerTableRecord") '(6 . "CONTINUOUS") '
		     (62 . 3) '(70 . 0) (cons 2 "ZBH")
	       )		       ; _结束list
      )				       ; _结束entmake
    )				       ; _结束progn
  )				       ; _结束if
  (setvar "clayer" "ZBH")

  ;; 判断有无字类型ZBH
  (if (= (tblobjname "STYLE" "ZBH") nil)
    (progn
      (entmake (list '(0 . "STYLE") '(100 . "AcDbSymbolTableRecord") '
		     (100 . "AcDbTextStyleTableRecord") '(3 . "txt") '
		     (40 . 0) '(41 . 0.6) '(50 . 0) '(70 . 0) (cons 2 "ZBH")
	       )		       ; _结束list
      )				       ; _结束entmake
    )				       ; _结束progn
  )				       ; _结束if

)
;;; _结束defun

;;; ------------------------------------------------------------
(defun textoffset ()
  (setq netbeetmp1 nil
	netbeetmp1 text_offset
  )				       ; _结束setq
  (prompt "\n请输入标注偏移量<")
  (princ text_offset)
  (prompt ">:")
  (if (= (setq text_offset (getreal))
	 nil
      )
    (setq text_offset netbeetmp1)
  )				       ; _结束if

)
;;; _结束defun
;;; --------------------------------------------------------------
(defun settexthome ()
  (setq netbeetmp1 nil
	netbeetmp1 texthome
  )				       ; _结束setq
  (prompt "\n请输入编号前缀(无前缀请输入\"*\")<")
  (Princ texthome)
  (Prompt ">:")
  (if (= (setq texthome (getstring))
	 ""
      )
    (setq texthome netbeetmp1)
  )				       ; _结束if
  (if (= texthome "*")
    (SETQ texthome "")
  )				       ; _结束if

)
;;; _结束defun

;;; --------------------------------------------------------
(defun settextend ()

  (setq netbeetmp1 nil
	netbeetmp1 textend
  )				       ; _结束setq
  (prompt "\n请输入编号后缀(无后缀请输入\"*\")<")
  (Princ textend)
  (Prompt ">:")
  (if (= (setq textend (getstring))
	 ""
      )
    (setq textend netbeetmp1)
  )				       ; _结束if
  (if (= textend "*")
    (SETQ textend "")
  )				       ; _结束if

)
;;; _结束defun
;;; --------------------------------------------------------
(defun settextbegan ()
  (setq netbeetmp1 nil
	netbeetmp1 bh_began
  )				       ; _结束setq
  (if (= (setq bh_began (getint (strcat "\n起始编号:1/2/3...<"
					(itoa bh_began) ">:"
				)
			)	       ; _结束getint

	 )			       ; _结束setq
	 nil
      )				       ; _结束=
    (setq bh_began netbeetmp1)
  )				       ; _结束if

)
;;; _结束defun
;;; -------------------------------------------------------
;;; 恢复环境
(defun setclose_bh ()
  (setvar "textstyle" textstyle_old)
  (setvar "cmdecho" 1)
  (setvar "clayer" old_lay)
  (setvar "dimzin" dimzin_old)

  (defun *merrmsg* (msg)
    (princ msg)
    (setq *error* m:err
	  m:err nil
    )
    (princ)
  )
  (Princ)
)
;;; _结束defun

;;; -----------------------------------------------------


;;; -------------------------------------------------------
;;; 设置字高函数
(defun settexth ()
  (setq netbeetmp1 nil
	netbeetmp1 newtextsize
	netbeetmp2 nil
	netbeetmp2 (strcat "\n输入字高:<" (rtos Newtextsize) ">:")
  )				       ; _结束setq
  (if (= (setq Newtextsize (getreal netbeetmp2))
	 nil
      )
    (setq Newtextsize netbeetmp1)
  )				       ; _结束if


)
;;; _结束defun
;;; -------------------------------------------------------
(defun *error* (msg)
  (setclose_bh)
)
;;; _结束defun

;;; -------------------------------------------
;;; 给表sszwd根据从左到右,从上到下的规则排序(插入法).
(defun zbh_px2 (/ sstmp1 sstmp2 loop_i loop_j pt1x pt1y pt2x pt2y) ; 定义内部函数
  (defun listij_in ()
    (setq sszwd (append
		  (reverse (cdr (member sstmp1 (reverse sszwd))))
		  (list sstmp2)
		  (member sstmp1 (reverse (cdr (member sstmp2
						       (reverse sszwd)
					       )
					  )
				 )
		  )
		  (cdr (member sstmp2 sszwd))
		)
    )
  )

  (setq loop_j 1)
  (while (< loop_j sslen)
    (setq sstmp2 (nth loop_j sszwd)
	  pt2x (car sstmp2)
	  pt2y (cadr sstmp2)
	  loop_i 0
    )
    (while (progn
	     (setq sstmp1 (nth loop_i sszwd)
		   pt1x (car sstmp1)
		   pt1y (cadr sstmp1)
	     )			       ; _结束setq
	     (cond		       ; 若指针i已经到了指针j的位置则跳出
	       ((= loop_i loop_j)
		 (not 1)
	       )		       ; 若第二点的Y比第一点的大,则把第二点
	                               ;插入到第一前
				       ; 并跳出
	       ((< pt1y pt2y)
		 (listij_in)
		 (not 1)
	       )		       ; _若第一点的Y与第二点的Y相等,且第二
	                               ; 点的X比第一点的小,
				       ; 则把第二点插入到第一点前，并跳出
	       ((and
		  (= pt1y pt2y)
		  (> pt1x pt2x)
		)
		 (listij_in)
		 (not 1)
	       )

	       ;; 其他情况则不进行互换
	       (t
		 (setq loop_i (1+ loop_i))
	       )
	     )			       ; _结束cond
	   )
    )				       ; _结束while
    (setq loop_j (1+ loop_j))
  )				       ; _结束while
  sstmp
)


(Prompt "\n[基础绘图系统1.5] 版权所有 蜜蜂软体工作室 2000年")
(prompt "\n桩位编号命令：zbh")