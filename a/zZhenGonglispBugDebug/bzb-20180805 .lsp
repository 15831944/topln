(vl-load-com)
(defun bf-app-bzb-excel (lst / i)
  (setq excelobj (BF-Excel-New t))
  (BF-Excel-MergeRange excelobj '(1 1 1 4))
  (BF-Excel-setRangeValue excelobj '(1 1) "�����")
  (BF-Excel-setRangeValue
    excelobj
    '(2 1 2 4)
    '("���" "X" "Y" "Z")
  )
  (setq i 3)
  (foreach item lst
    (BF-Excel-setRangeValue
      excelobj
      (list i 1 i 4)
      item
      ;(cons (- i 2) item)
    )
    (setq i (1+ i))
  )
)

(defun BF-APP-bzb-table (pt lst / i j len tab)
  (setq len (length lst))
  (setq  tab (vla-AddTable
							 (BF-model-space)
							 (vlax-3D-point pt)
							 (+ 2 len)
							 4
							 15
							 50
						 )
  )
	
  (setq i 0)
  (if vla-put-RegenerateTableSuppressed
    (vla-put-RegenerateTableSuppressed tab :vlax-false)
  )
  (vla-SetText tab 0 0 "�����")
  (mapcar '(lambda (x) (vla-SetText tab 1 i x) (setq i (1+ i)))
    '("���" "X" "Y" "Z")
  )
  (setq i 2)
  (foreach item  lst
    (setq j 0)
    (mapcar
      '(lambda (a)
				 (if (< j 4)
					 (vla-settext tab i j a)
				 )
				 (setq j (1+ j))
			 )
      item
      ;(cons (- i 1) item)
    )
    (setq i (1+ i))
  )
	
  (if vla-put-RegenerateTableSuppressed
    (vla-put-RegenerateTableSuppressed tab :vlax-true)
  )
)

(defun bf-app-bzb-drawdcl (/ baseang basept bf-app-bzb-getvalue dcl-id  tmp what_next)
  (setq  dcl-id
    (BF-dcl-Init
      "�����ע����"
      "celiang-coord"
      (list
        (BF-dcl-setLayout
          "row"
          nil
          (list
			      (BF-dcl-setLayout
			        "boxed_row"
			        '((label . "��������"))
			        (list
			          (BF-dcl-setLayout
			            "column"
			            nil
			            (list
						        (BF-dcl-addItem
						          "radio_button"
						          '((key . "math-coord") (label . "��ѧ����"))
						        )
						        (BF-dcl-addItem
						          "radio_button"
						          '((key . "celiang-coord") (label . "��������"))
						        )
			            )
			          )
			          (BF-dcl-setLayout
			            "column"
			            nil
			            (list
						        (BF-dcl-addItem
						          "text"
						          '((key . "coord-type-up")
												 (label . "A=Y")
												 (alignment . centered)
											 )
						        )
						        (BF-dcl-addItem
						          "text"
						          '((label . "----") (alignment . centered))
						        )
						        (BF-dcl-addItem
						          "text"
						          '((key . "coord-type-down")
												 (label . "B=X")
												 (alignment . centered)
											 )
						        )
			            )
			          )
			        )
			      )
			      (BF-dcl-setLayout
			        "boxed_radio_column"
			        '((label . "����ȡֵ"))
			        (list
			          (BF-dcl-addItem
			            "radio_button"
			            '((key . "world-coord") (label . "��������"))
			          )
			          (BF-dcl-addItem
			            "radio_button"
			            '((key . "user-coord") (label . "�û�����"))
			          )
			        )
			      )
			      (BF-dcl-setLayout
			        "boxed_column"
			        '((label . "��ǰ��������") (width . 30))
			        (list
			          (BF-dcl-addItem
			            "text"
			            '((key . "basetext")
										 (alignment . centered)
										 (label . "(X=0.000,Y=0.000)")
									 )
			          )
			          (BF-dcl-setLayout
			            "row"
			            nil
			            (list
						        (BF-dcl-addItem
						          "toggle"
						          '((key . "tog") (label . "���¶���"))
						        )
						        (BF-dcl-addItem
						          "button"
						          '((key . "basebtn") (label . "ָ������"))
						        )
			            )
			          )
			        )
			      )
          )
        )
        (BF-dcl-setLayout
          "row"
          nil
          (list
			      (BF-dcl-setLayout
			        "boxed_row"
			        '((label . "�������"))
			        (list
			          (BF-dcl-setLayout
			            "column"
			            nil
			            (list
						        (BF-dcl-addItem
						          "edit_box"
						          '((key . "theight")
												 (label . "�����ָ�:")
												 (edit_width . 11)
												 (alignment . left)
											 )
						        )
						        (BF-dcl-addItem
						          "edit_box"
						          '((key . "twidth")
												 (label . "�����ֿ�:")
												 (edit_width . 11)
												 (alignment . left)
											 )
						        )
						        (BF-dcl-addItem
						          "edit_box"
						          '((key . "prefix-x")
												 (label . "���ǰ׺:")
												 (edit_width . 11)
												 (alignment . left)
											 )
						        )
						        (BF-dcl-addItem
						          "edit_box"
						          '((key . "prefix-y")
												 (label . "�ݱ�ǰ׺:")
												 (edit_width . 11)
												 (alignment . left)
											 )
						        )
										(BF-dcl-addItem
						          "edit_box"
						          '((key . "prefix-h")
												 (label . "�߳�ǰ׺")
												 (edit_width . 11)
												 (alignment . left)
											 )
						        )
			            )
			          )
			          (BF-dcl-setLayout
			            "column"
			            nil
			            (list
						        (BF-dcl-addItem
						          "popup_list"
						          '((key . "tstyle")
												 (label . "������ʽ:")
												 (edit_width . 10)
												 (alignment . right)
											 )
						        )
						        (BF-dcl-addItem
						          "popup_list"
						          '((key . "tlayer")
												 (label . "����ͼ��:")
												 (edit_width . 10)
												 (alignment . right)
											 )
						        )
						        (BF-dcl-addItem
						          "popup_list"
						          '((key . "arrow")
												 (label . "��ͷ��ʽ:")
												 (edit_width . 10)
												 (alignment . right)
											 )
						        )
						        (BF-dcl-addItem
						          "popup_list"
						          '((key . "jingdu")
												 (label . "��ע����:")
												 (edit_width . 10)
												 (alignment . right)
											 )
						        )
										(BF-dcl-addItem
						          "toggle"
						          '((key . "xsgc") (label . "��ʾ�߳�"))
						        )
			            )
			          )
			        )
			      )
			      (BF-dcl-setLayout
			        "boxed_column"
			        '((label . "�Զ�������ϵ"))
			        (list
			          (BF-dcl-addItem
			            "edit_box"
			            '((key . "base-x")
										 (label . "�����������:")
										 (edit_width . 11)
									 )
			          )
			          (BF-dcl-addItem
			            "edit_box"
			            '((key . "base-y")
										 (label . "������������:")
										 (edit_width . 11)
									 )
			          )
			          (BF-dcl-addItem
			            "edit_box"
			            '((key . "coord-ro")
										 (label . "��������ת��:")
										 (edit_width . 11)
									 )
			          )
			          (BF-dcl-addItem
			            "button"
			            '((key . "pick-ang") (label . "ʰȡ�Ƕ�"))
			          )
			        )
			      )
          )
        )
        (list "ok_cancel;")
      )
    )
  )
  (defun bf-app-bzb-getvalue (/ keys)
    (setq keys '("math-coord"  "celiang-coord"
									"world-coord"  "user-coord"   "tog"
									"theight"  "twidth"       "prefix-x"
									"prefix-y"  "prefix-h" "tstyle"       "tlayer"
									"arrow"  "jingdu"    "xsgc"   "base-x"
									"base-y"  "coord-ro"
								)
    )
    (mapcar 'cons keys (BF-dcl-getValues keys))
  )
  (setq result option)
  ;(if (= nil result) (setq result '()))
  (setq what_next 2)
  (while (>= what_next 2)    ;Begin display loop.
    (setq what_next
      (BF-dcl-Start
        dcl-id
        (list
          '("set"
						 ("jingdu"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "jingdu"))
								 tmp
								 "3"
							 )
						 )
						 ("arrow"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "arrow"))
								 tmp
								 "1"
							 )
						 )
						 ("prefix-x"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "prefix-x"))
								 tmp
								 "X="
								 ;"Y="
							 )
						 )
						 ("prefix-y"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "prefix-y"))
								 tmp
								 "Y="
								 ;"X="
							 )
						 )
						 ("prefix-h"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "prefix-h"))
								 tmp
								 "H="
								 ;"X="
							 )
						 )
						 ("theight"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "theight"))
								 tmp
								 "3"
							 )
						 )
						 ("twidth"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "twidth"))
								 tmp
								 "0.8"
							 )
						 )
						 ("tog"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "tog"))
								 tmp
								 "0"
							 )
						 )
						 ("xsgc"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "xsgc"))
								 tmp
								 "0"
							 )
						 )
						 ("tlayer"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "tlayer"))
								 tmp
								 "0"
							 )
						 )
						 ("tstyle"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "tstyle"))
								 tmp
								 "0"
							 )
						 )
						 ;("math-coord" . (if (setq tmp (BF-AssocList-Key result "math-coord")) tmp "0"))
						 ("celiang-coord"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "celiang-coord"))
								 tmp
								 "1"
							 )
						 )
						 ;("world-coord" . (if (setq tmp (BF-AssocList-Key result "world-coord")) tmp "0"))
						 ("user-coord"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "user-coord"))
								 tmp
								 "1"
							 )
						 )
						 ("base-x"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "base-x"))
								 tmp
								 ""
							 )
						 )
						 ("base-y"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "base-y"))
								 tmp
								 ""
							 )
						 )
						 ("coord-ro"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "pick-ang"))
								 (BF-math-rtos tmp 3)
								 ""
							 )
						 )
						 ("basetext"
							 .
							 (if
								 (setq tmp (BF-AssocList-Key result "basebtn"))
								 (strcat
									 ;"(X="
									 "(Y="
									 (BF-math-rtos (car tmp) 3)
									 ;",Y="
									 ",X="
									 (BF-math-rtos (cadr tmp) 3)
									 ")"
								 )
								 "(X=0.000,Y=0.000)"
							 )
						 )
					 )
          '("mode"
						 ("base-x"
							 .
							 (if
								 (= "1" (BF-AssocList-Key result "tog"))
								 0
								 1
							 )
						 )
						 ("base-y"
							 .
							 (if
								 (= "1" (BF-AssocList-Key result "tog"))
								 0
								 1
							 )
						 )
						 ("coord-ro"
							 .
							 (if
								 (= "1" (BF-AssocList-Key result "tog"))
								 0
								 1
							 )
						 )
						 ("pick-ang"
							 .
							 (if
								 (= "1" (BF-AssocList-Key result "tog"))
								 0
								 1
							 )
						 )
						 ("basebtn"
							 .
							 (if
								 (= "1" (BF-AssocList-Key result "tog"))
								 0
								 1
							 )
						 )
					 )
          '("list"
						 ("jingdu" ("[0]0" "[1]0.0" "[2]0.00" "[3]0.000"))
						 ("arrow" ("��" "ʮ��" "��ͷ" "Բ��"))
						 ("tlayer" (BF-ent-Layers))
						 ("tstyle" (BF-Ent-TextStyles))
					 )
          '("action"
						 ("world-coord"
							 (BF-dcl-setModes
								 '(("base-x" . 1)
										("base-y" . 1)
										("coord-ro" . 1)
										("pick-ang" . 1)
										("basebtn" . 1)
										("tog" . 1)
									)
							 )
						 )
						 ("user-coord"
							 (mode_tile "tog" 0)
							 (if
								 (= "1" (get_tile "tog"))
								 (BF-dcl-setModes
									 '(("base-x" . 0)
											("base-y" . 0)
											("coord-ro" . 0)
											("pick-ang" . 0)
											("basebtn" . 0)
										)
								 )
								 (BF-dcl-setModes
									 '(("base-x" . 1)
											("base-y" . 1)
											("coord-ro" . 1)
											("pick-ang" . 1)
											("basebtn" . 1)
										)
								 )
							 )
						 )
						 ("tog"
							 (if
								 (= "1" (get_tile "tog"))
								 (BF-dcl-setModes
									 '(("base-x" . 0)
											("base-y" . 0)
											("coord-ro" . 0)
											("pick-ang" . 0)
											("basebtn" . 0)
										)
								 )
								 (BF-dcl-setModes
									 '(("base-x" . 1)
											("base-y" . 1)
											("coord-ro" . 1)
											("pick-ang" . 1)
											("basebtn" . 1)
										)
								 )
							 )
						 )
						 ("math-coord"
							 (BF-dcl-setValues
								 '(("coord-type-up" . "X=X") ("coord-type-down" . "Y=Y"))
							 )
						 )
						 ("celiang-coord"
							 (BF-dcl-setValues
								 '(("coord-type-up" . "A=Y") ("coord-type-down" . "B=X"))
							 )
						 )
						 ("basebtn"
							 (setq
								 result
								 (BF-AssocList-AppendList result (bf-app-bzb-getvalue))
							 )
							 (done_dialog 4)
						 )
						 ("pick-ang"
							 (setq
								 result
								 (BF-AssocList-AppendList result (bf-app-bzb-getvalue))
							 )
							 (done_dialog 5)
						 )
						 ("accept"
							 (setq
								 result
								 (BF-AssocList-AppendList result (bf-app-bzb-getvalue))
							 )
							 (done_dialog 1)
						 )
					 )
        )
      )
    )          ;Display dialog box.
    (cond
      ((= what_next 4)      ;Prompt user to
        (setq basept (getpoint "\npick a point")) ;pick pt.
        (setq result
	        (BF-AssocList-AppendItem result (cons "basebtn" basept))
        )
      )
      ((= what_next 5)      ;Prompt user to
        (setq baseang (getorient "\npick a angle")) ;pick angle.
        (setq result (BF-AssocList-AppendItem
											 result
											 (cons "pick-ang" baseang)
										 )
        )
      )
      ((= what_next 0)
        (prompt "\nuser cancelled dialog")
      )
    )
  )
  (BF-dcl-End dcl-id)
  result
)

;ԭ�����ע���� wzg356 2014.9.12
;�������ע���� vicwjb 2015.5.21
(defun c:bzb (/      *error*     *olderror*  accaction
							 ang    basepoint   dcl-id    hzt
							 layout1    newerr      pickaction  pickbutton
							 startpoint  sysvarlst   x-edit    y-edit
							 z-edit mr mw ms mf
						 )
	;�Զ����µĳ�����
  (defun newerr  (msg)
		(print "�ҵ��Զ��庯��")
		(if (/= nil mr) 
		(progn
			(close mr)
			(setq mr nil)
		)
	)
	(if (/= nil mw) 
		(progn
			(prin1 option mw)
			(close mw)
			(setq mw nil)
		)
		
	)
    (mapcar 'eval sysvarlst)    ;�ָ���������
    (if  *olderror*
      (setq *error*    *olderror*
	      *olderror* nil
      )
    ) ;_ �ָ�*error*����
    ;(setq shuju nil)      ;�������
    (if (not (member msg '(nil "������ȡ��" ";����:quit / exit abort")))
      (princ (strcat ";����:" msg))
    )
  )
  ;;ϵͳ����
  (BF-startundo (BF-active-document))
  ;;������鿪ʼ
  (setq  sysvarlst
    (mapcar (function (lambda (n) (list 'setvar n (getvar n))))
      '("osmode"    "cmdecho"   "OSNAPCOORD"
				 "dimzin"    "plinewid"   "TEXTSIZE"
				 "textstyle"
			 )
    )
  )          ;����ϵͳ����
  (setq *olderror* *error*)    ;���������
  (setq *error* newerr)      ;�����Զ��������  
  (setvar "cmdecho" 0)
  ;;�ر�������Ӧ
  (setvar "OSNAPCOORD" 1)
  ;;�����������ȼ���Ϊ�����������������׽����
  ;(setvar "OSMODE" 679)
  ;;�ı䲶׽ģʽ
  (setvar "dimzin" 0)
  ;;��������λֵ�����㴦��
  (if (not shuju)(setq shuju '()))
  ;;���ô洢���ݵı���
  ;=========================
  ;===========================
  ;��ʼ��ע
	(setq mf "d:/xxnote-bzb.txt")
	(setq mr (open mf "r"))
	(if (= nil mr) 
		(setq mw (open mf "w"))
		;(setq option (read (read-line mr)))
		(progn
			(setq ms (read-line mr))
			(if (and (= nil option) (/= nil ms)) (setq option (read ms)))
		)
	)
	(if (= nil mw) (setq mw (open mf "w")))
  (bf-app-bzb-sub)
	(if (/= nil mr) 
		(progn
			(close mr)
			(setq mr nil)
		)
	)
	(if (/= nil mw) 
		(progn
			(prin1 option)
			(close mw)
			(setq mw nil)
		)
		
	)
  ;;�ָ�����
  (BF-endundo (BF-active-document))
  ;;��������
  (mapcar 'eval sysvarlst)    ;�ָ���������
  (setq *error* *olderror*)
  ;;�ָ�������
  (princ)
)

;;��������:bf-app-bzb-addtext
;;����˵��:
;;��    ��:textstr:
;;��    ��:insertpt:
;;��    ��:height:
;;��    ��:property:��������ֵ����((Layer . "0") (StyleName . "songti") (ScaleFactor . 0.8) (TextAlignmentPoint 1 1 0) (Alignment . 2))
;;�� �� ֵ:
;;ʾ    ��:(bf-app-bzb-addtext textstr insertpt height property)
(defun bf-app-bzb-addtext (textstr insertpt height property / textobj)
  (setq  textobj  (vla-AddText
										(BF-model-space)
										textstr
										(vlax-3D-point insertpt)
										height
									)
  )
  (foreach item  property
    ((eval (read (strcat "vla-put-" (vl-symbol-name (car item))))
		 )
      textobj
      (cdr item)
    )
  )
  textobj
)
(defun bf-app-bzb-shizi  (pt size)
  (list
    (BF-ent-line (polar pt0 0 size) (polar pt0 pi size))
    (BF-ent-line
      (polar pt0 (* pi 0.5) size)
      (polar pt0 (* pi -0.5) size)
    )
  )
)

;;��������:bf-app-bzb-arrow
;;����˵��:
;;��    ��:start:
;;��    ��:end:
;;�� �� ֵ:
;;ʾ    ��:(bf-app-bzb-arrow (getpoint) (getpoint))
(defun bf-app-bzb-arrow  (start end size / arrobj len mid)
  (setq len (BF-Math-Length start end))
  (if (< size len)
    (setq mid (polar start (angle start end) size))
    (setq mid end)
  )
  (setq
    arrobj (vla-AddLightweightPolyline
						 (BF-model-space)
						 (BF-vla-List->Array
							 (apply 'append
								 (mapcar 'BF-point-3d->2d (list start mid end))
							 )
							 5
						 )
					 )
  )
  (vla-SetWidth arrobj 0 0 (/ size 3))
  arrobj
)

;;��������:bf-app-bzb-dot
;;����˵��:��ʵ��Բ��
;;��    ��:pt:Բ��
;;�� �� ֵ:Բ�����
;;ʾ    ��:(bf-app-bzb-dot (getpoint))
(defun bf-app-bzb-dot (pt size / end start yuanobj)
  (setq size (/ size 6))
  (setq start (polar pt 0 size))
  (setq end (polar pt pi size))
  (setq  yuanobj  (vla-AddLightweightPolyline
										(BF-model-space)
										(BF-vla-List->Array
											(apply 'append
												(mapcar 'BF-point-3d->2d (list start end))
											)
											5
										)
									)
  )
  (vla-put-Closed yuanobj :vlax-true)
  (vla-SetBulge yuanobj 0 1)
  (vla-SetBulge yuanobj 1 1)
  (vla-SetWidth yuanobj 0 size size)
  (vla-SetWidth yuanobj 1 size size)
  (vla-put-ConstantWidth yuanobj (* 2 size))
  yuanobj
)
(defun bf-app-bzb-sub (/    arrow       basebtn  base-x
												base-y    celiang-coord    coord-ro
												gr    gr-model   gr-value  hengxian
												jingdu    p1       p2    pick-ang
												prefix-x    prefix-y  prefix-h pt0  pt2
												shangtext  shizi       theight  tlayer
												tpt    tstyle     twidth  user-coord
												xianchang  xiatext    xiexian  xpt xstmp
												ypt xsgc sjgc sxgcf ph gctext;xsgc�Ƿ���ʾ�߳�   sjgcʵ�ʸ߳�ֵ ph �߳��������� gctext �߳�text
											)
	;(print xsgc)
  (initget "S T E  ")      ;���ո�Ĺؼ���
  (setq pt0 (getpoint "\n��ȡ��ע���[����(S)/����CAD���(T)/�������Excel(E)]:")) ;�������ڲ������ָ�ʱֱ�ӽ����ע
  (cond
    ((= pt0 "") nil)      ;�Ҽ����ո��˳�
    ((= pt0 "S")
			;(initget 1 "Yes No")
			;(setq xstemp (getkword "�Ƿ���ʾ�߳�(��Y/��N):"))
			;(if (/= nil xstemp) 
			;(cond 
			;	((= xstemp "Yes") (setq xsgc t))
			;	((= xstemp "No") (setq xsgc nil))
			;)
			
			;)
			
      (setq option (bf-app-bzb-drawdcl))
      (bf-app-bzb-sub)      ;�������ѭ����ȡ��ע�㣬�еݹ��Ӱ��
    )
    ((= pt0 "T")
      (BF-APP-bzb-table
        (getpoint "\nָ���������:")
        shuju ;(reverse shuju)
      )
      (bf-app-bzb-sub)
    )          ;��������
    ((= pt0 "E")
      ;(bf-app-bzb-excel (reverse shuju))
      (bf-app-bzb-excel shuju)
      (bf-app-bzb-sub)
    )          ;��������
    ((= 'list (type pt0))
      ;;���û�������
      (if option
        (progn
			    (setq
			      celiang-coord (BF-AssocList-Key option "celiang-coord")
			      user-coord   (BF-AssocList-Key option "user-coord")
			      tog     (BF-AssocList-Key option "tog")
			      theight   (atof (BF-AssocList-Key option "theight"))
			      twidth   (atof (BF-AssocList-Key option "twidth"))
			      prefix-x   (BF-AssocList-Key option "prefix-x")
			      prefix-y   (BF-AssocList-Key option "prefix-y")
						prefix-h   (BF-AssocList-Key option "prefix-h")
						xsgc     (BF-AssocList-Key option "xsgc")
			      tstyle   (nth (atoi (BF-AssocList-Key option "tstyle"))
											 (BF-Ent-TextStyles)
										 )
			      tlayer   (nth (atoi (BF-AssocList-Key option "tlayer"))
											 (BF-ent-Layers)
										 )
			      arrow   (BF-AssocList-Key option "arrow")
			      jingdu   (atoi (BF-AssocList-Key option "jingdu"))
			      base-x   (if
											 (= ""
												 (setq base-x (BF-AssocList-Key option "base-x"))
											 )
											 (car pt0)
											 (atof base-x)
										 )
			      base-y   (if
											 (= ""
												 (setq base-y (BF-AssocList-Key option "base-y"))
											 )
											 (cadr pt0)
											 (atof base-y)
										 )
			      pick-ang   (if (= "0" tog)
												 0.0
												 (BF-AssocList-Key option "pick-ang")
											 )
			      basebtn   (cond
												((and (= "0" celiang-coord) (= "0" user-coord))
													pt0
												)    ;��ѧ������
												((and (= "0" celiang-coord) (= "1" user-coord))
													(if  (= "0" tog)
														pt0
														(BF-AssocList-Key option "basebtn")
													)
												)    ;��ѧ���û�
												((and (= "1" celiang-coord) (= "0" user-coord))
													pt0
												)    ;����������
												((and (= "1" celiang-coord) (= "1" user-coord))
													(if  (= "0" tog)
														pt0
														(BF-AssocList-Key option "basebtn")
													)
												)    ;�������û�
											)
			    )
        )
        (setq
			    celiang-coord "0"
			    user-coord    "0"
			    tog         "0"
					xsgc 				"0"
			    theight       3
			    twidth         0.8
			    ;prefix-x      "X="
			    prefix-x      "Y="
			    ;prefix-y      "Y="
			    prefix-y      "X="
					prefix-h      "H="
			    tstyle         (vla-get-name
													 (vla-get-ActiveTextStyle (BF-active-document))
												 )
			    tlayer         (vla-get-name
													 (vla-get-ActiveLayer (BF-active-document))
												 )
			    arrow         "1"
			    jingdu         3
			    base-x         (car pt0)
			    base-y         (cadr pt0)
			    pick-ang      0.0
			    basebtn       pt0
        )
      )
      (setq tpt (BF-math-TransPt
									basebtn
									(list base-x base-y 0.0)
									pt0
									pick-ang
								)
      )     
      ;�����û�����
			
			(print xsgc)
			(if (= xsgc "1")
				(setq xsgcf t)
				(setq xsgcf nil)
			)
      ;(print (nth 2 pt0))
			(setq sjgc (nth 2 pt0))
			
      ;(setq xsgc t sjgc (nth 2 pt0))
      (if xsgcf;Ҫ��ʾ�߳�
				(progn
				  (if (zerop sjgc);�߳�Ϊ0
				    (progn
							;(initget 1)
			      	(setq sjgc (getreal "��ǰ�߳�Ϊ0�����ֶ�����߳�ֵ<0>��"))
							(if (= nil sjgc) (setq sjgc 0))
				    )
				  )
				)
      )
			
      
			
      ;;�������ֲ����
      (setq p1 (polar pt0 (* pi 0.5) (+ theight (/ theight 7.7)))) ;������λ��
      (setq p2 (polar pt0 (* pi -0.5) (+ theight (/ theight 7.7))))
      (setq p3 (polar p1  (* pi 0.5) (+ theight (/ theight 7.7))))
      (if xsgcf
				(setq ph (polar p2  (* pi -0.5) (+ theight (/ theight 7.7))))
      )
			(print ph)
			;������λ��
      (if (null xuhao)
        (setq xuhao(getint "\n��������ʼ���:"))
        (setq xuhao(1+ xuhao))
      )
			
      (if (= "0" celiang-coord)
        (setq
          xpt (strcat prefix-x (rtos (car tpt) 2 jingdu)) ;�������� 
          ypt (strcat prefix-y (rtos (cadr tpt) 2 jingdu))
        )
        (setq
          xpt (strcat prefix-x (rtos (cadr tpt) 2 jingdu)) ;�������� 
          ypt (strcat prefix-y (rtos (car tpt) 2 jingdu))
					;sjgc 
        )
      )
      ;(setvar "osmode" 0)
      
      (setq        ;��������
        shangtext (bf-app-bzb-addtext
										xpt
										p1
										theight
										(list
											(cons 'Layer tlayer)
											(cons 'StyleName tstyle)
											(cons 'ScaleFactor twidth)
										)
									)
        xiatext   (bf-app-bzb-addtext
										ypt
										p2
										theight
										(list
											(cons 'Layer tlayer)
											(cons 'StyleName tstyle)
											(cons 'ScaleFactor twidth)
										)
									)
        xuhaotext (bf-app-bzb-addtext
										(itoa xuhao)
										p3
										theight
										(list
											(cons 'Layer tlayer)
											(cons 'StyleName tstyle)
											(cons 'ScaleFactor twidth)
										)
									)
				
				
        
      )
			
			(if xsgcf 
				(setq gctext (bf-app-bzb-addtext
											 (strcat prefix-h (rtos sjgc 2 jingdu))
											 ph
											 theight
											 (list
												 (cons 'Layer tlayer)
												 (cons 'StyleName tstyle)
												 (cons 'ScaleFactor twidth)
											 )
										 ))
			)
      ;;�������ֵİ�Χ��
      (vla-GetBoundingBox shangtext 'p3 'p4)
      (vla-GetBoundingBox xiatext 'p5 'p6)
      ;;�������ֵ���󳤶�
      (setq xianchang
	      (-
	        (BF-math-maxlist
				    (mapcar
				      'car
				      (mapcar 'vlax-safearray->list (list p3 p4 p5 p6))
				    )
	        )
	        (BF-math-minlist
				    (mapcar
				      'car
				      (mapcar 'vlax-safearray->list (list p3 p4 p5 p6))
				    )
	        )
	      )
      )
			
      (setq pt2 (polar pt0 0 xianchang))  ;���߶˵� 
      (setq hengxian (BF-ent-line pt0 pt2)) ;������
			
      (cond        ;���ݼ�ͷ��ʽ����ͷ,������
        ((= "0" arrow) (setq xiexian (BF-Ent-Line pt0 pt0))) ;�޼�ͷ
        ((= "1" arrow)
				  (setq shizi (bf-app-bzb-shizi pt0 theight))
				  (setq xiexian (BF-Ent-Line pt0 pt0))
        )        ;ʮ��
        ((= "2" arrow)
				  (setq xiexian (bf-app-bzb-arrow pt0 pt0 theight))
        )        ;��ͷ
        ((= "3" arrow)
				  (setq shizi (bf-app-bzb-dot pt0 theight))
				  (setq xiexian (BF-Ent-Line pt0 pt0))
        )        ;Բ��
      )
      (setq gr 0
	      gr-model 0
	      gr-value 0
      )
      ;;gr-model�������
      (while (/= gr-model 3)    ;��������ȷ�ϱ�עλ�ã��˳�ѭ��
        (setq gr        (grread T 8)
	        gr-model (car gr)
	        gr-value (cadr gr)    ;���λ��
        )
        (if (and gr (= gr-model 5))  ;����ƶ�
			    (progn
						
			      (if (>= (car gr-value) (car pt0)) ;������ֵ���������ұ�
			        (progn
								(setq
				          pt2 (polar gr-value 0 xianchang) ;���߶˵�
				          p1  (polar gr-value (* pi 0.5) (/ theight 7.7))
				          ;������λ��
				          p2  (polar gr-value (* pi -0.5) (+ theight (/ theight 7.7)))
				          ;p3  (polar gr-value (* pi 0.5)  (+ theight (/ theight 10)))
									p3 (polar p1  (* pi 0.5) (+ theight (/ theight 7.7)))
									
				        )        ;������λ��
								(if xsgcf
									(setq ph (polar p2  (* pi -0.5) (+ theight (/ theight 7.7))))
				      	)
								
							)
							
							(progn
								(setq
				          pt2 (polar gr-value pi xianchang)
				          p1  (polar pt2 (* pi 0.5) (/ theight 7.7))
				          p2  (polar pt2 (* pi -0.5) (+ theight (/ theight 7.7)))
				          ;p3  (polar pt2 (* pi 0.5) (+ theight (/ theight 10)))
									p3 (polar p1  (* pi 0.5) (+ theight (/ theight 7.7)))
									
				        )        ;���ֵ�����������
								(if xsgcf
									(setq ph (polar p2  (* pi -0.5) (+ theight (/ theight 7.7))))
					      )
							)
			        
			      )
						
			      ;(print pt0)
			      ;(print p1)
			      ;(print p2)
			      ;(print "p3")
			      ;(print p3)
						
			      (vla-put-StartPoint hengxian (vlax-3D-point gr-value))
			      (vla-put-EndPoint hengxian (vlax-3D-point pt2))
			      (vla-put-InsertionPoint shangtext (vlax-3D-point p1))
			      (vla-put-InsertionPoint xiatext (vlax-3D-point p2))
			      
			      ;;xuhaotext
			      (vla-put-InsertionPoint xuhaotext (vlax-3D-point p3))
			      (if xsgcf
							(vla-put-InsertionPoint gctext (vlax-3D-point ph))
			      )
			      (if (/= "2" arrow)
			        (vla-put-EndPoint xiexian (vlax-3D-point gr-value))
			        (progn
			          (vla-put-Coordinates
						      xiexian
						      (BF-vla-List->Array
						        (apply
						          'append
						          (mapcar
						            'BF-point-3d->2d
						            (list
									        pt0
									        (if (< theight (BF-Math-Length pt0 gr-value))
									          (polar pt0 (angle pt0 gr-value) theight)
									          gr-value
									        )
									        gr-value
						            )
						          )
						        )
						        5
						      )
			          )
			        )
			      )
			    )
        )
      )
			;(BF-Ent-Group 
			;  (append 
			;    (list 
			;      xiexian 
			;      (BF-Ent-Block (list shangtext xiatext hengxian) "*U" gr-value)  
			;    )
			;    (if (and shizi (vl-consp shizi))
			;      shizi
			;      (list shizi)
			;    )
			;  )
			;  "*"
			;)
      ;(setq shuju (cons (list (substr xpt 3) (substr ypt 3) "0.0") shuju) )
      (setq shuju (append shuju (list (list (itoa xuhao) (substr xpt 3) (substr ypt 3) (rtos sjgc 2 jingdu))) ))
      ;(setvar "OSMODE" 679)
      ;;�ı䲶׽ģʽΪ�´�ѭ����׼��
      (bf-app-bzb-sub)
    )
  )
)



;;======================================
;;===========����Ϊ�ڿ㲿��=============
;;======================================
(defun BF-dcl-Init (title focus dclstr / dclfile)
  (setq dclfile (vl-filename-mktemp "DclTemp.dcl"))
  (BF-dcl-PrintDcl
    (BF-dcl-setDialog
      (strcat (vl-filename-base dclfile) ":dialog")
      (list (cons 'label title)
	      (cons 'initial_focus focus)
      )
      dclstr
    )
    dclfile
  )
  (BF-return (cons dclfile (BF-dcl-getId dclfile)))
)

(defun BF-dcl-setLayout  (layoutname layoutlst lst)
  (list  (strcat ":" layoutname)
	  (append (BF-dcl-listsplit layoutlst) (apply 'append lst))
  )
)

(defun BF-dcl-addItem (itemname lst)
  (list (strcat ":" itemname) (BF-dcl-listsplit lst))
)

(defun BF-dcl-getValues  (lst)
  (cond
    ((listp lst) (mapcar 'get_tile lst))
    ((= (type lst) 'str) (get_tile lst))
    (t nil)
  )
)

(defun BF-dcl-Start (dcl-id actionlist)
  (if (not
				(new_dialog (vl-filename-base (car dcl-id)) (cdr dcl-id))
			)
    (exit)
  )
  (if (listp actionlist)
    (mapcar
      '(lambda (x / tmpx tmpy)
				 (setq tmpx (car x))
				 (setq tmpy (cdr x))
				 (cond
					 ((and tmpx (= tmpx "set") tmpy) (BF-dcl-setValues tmpy))
					 ((and tmpx (= tmpx "mode") tmpy) (BF-dcl-setModes tmpy))
					 ((and tmpx (= tmpx "action") tmpy) (BF-dcl-setAction tmpy))
					 ((and tmpx (= tmpx "list") tmpy)
						 (mapcar '(lambda (y) (apply 'BF-dcl-addlist y)) tmpy)
					 )
					 ((and tmpx (= tmpx "image") tmpy)
						 (mapcar '(lambda (y) (apply 'BF-dcl-loadsld y)) tmpy)
					 )
				 )
			 )
      actionlist
    )
  )
  (BF-return (start_dialog))
)

(defun BF-math-rtos (real prec / dimzin result)
  (setq dimzin (getvar 'dimzin))
  (setvar 'dimzin 0)
  (setq result (vl-catch-all-apply 'rtos (list real 2 prec)))
  (setvar 'dimzin dimzin)
  (if (not (vl-catch-all-error-p result))
    result
  )
)

(defun BF-ent-Layers ()
	; (setq layers-obj (vla-get-Layers doc))
	; (setq layer-list '())
	;(vlax-for i layers-obj
	;  (setq layer-list (append layer-list (list (vla-get-Name i))))
	;)
	;layer-list
  (BF-Ent-ListCollection (BF-Layers))
)

(defun BF-Ent-TextStyles ()
  (BF-Ent-ListCollection (BF-TextStyles))
)

(defun BF-dcl-setModes (lst)
  (mapcar 'mode_tile
    (mapcar 'car lst)
    (mapcar
      '(lambda (x)
				 (if (and (listp x) (= 'sym (type (car x))))
					 (eval x)
					 x
				 )
			 )
      (mapcar 'cdr lst)
    )
  )
)

(defun BF-dcl-setValues  (lst)
  (mapcar
    'set_tile
    (mapcar 'car lst)
    (mapcar
      '(lambda (x / tmp)
				 (if (and (listp x) (= 'sym (type (car x))))
					 (setq tmp (eval x))
					 x
				 )
			 )
      (mapcar 'cdr lst)
    )
  )
)

(defun BF-AssocList-AppendList (lst value)
  (if (listp value)
    (foreach item value
      (setq lst (BF-AssocList-AppendItem lst item))
    )
  )
)

(defun BF-AssocList-AppendItem (lst value)
  (if (assoc (car value) lst)
    (setq lst (BF-AssocList-Remove lst (car value)))
  )
  (cons value lst)
)

(defun BF-dcl-End (dclid)
  (unload_dialog (cdr dclid))
  (vl-file-delete (car dclid))
  (setq  *user-dclfile-dclid-list*
    (vl-remove dclid
			*user-dclfile-dclid-list*
    )
  )
  (princ)
)
(defun BF-startundo (doc)
  (BF-endundo doc)
  (vla-startundomark doc)
)

;;��������:BF-AssocList-Key
;;����˵��:���ع�������key��Ӧ��value
;;��    ��:lst:������
;;��    ��:key:�ؼ���
;;�� �� ֵ:key��Ӧ��value
;;ʾ    ��:(BF-AssocList-Key lst key)
(defun BF-AssocList-Key  (lst key)
  (cdr (assoc key lst))
)

(defun BF-active-document nil
  (eval  (list 'defun
					 'BF-active-document
					 'nil
					 (vla-get-activedocument (vlax-get-acad-object))
				 )
  )
  (BF-active-document)
)

(defun BF-endundo (doc)
  (while (= 8 (logand 8 (getvar 'undoctl)))
    (vla-endundomark doc)
  )
)

(defun BF-model-space nil
  (eval
    (list 'defun
	    'BF-model-space
	    'nil
	    (vla-get-modelspace
	      (vla-get-activedocument (vlax-get-acad-object))
	    )
    )
  )
  (BF-model-space)
)

(defun BF-Ent-Line (start end)
  (vla-AddLine
    (BF-model-space)
    (vlax-3D-point start)
    (vlax-3D-point end)
  )
)

(defun BF-Math-Length (start end)
  (BF-Vec-Norm (mapcar '- end start))
)

(defun BF-vla-List->Array (nList arraytype)
  (vlax-SafeArray-Fill
    (vlax-Make-SafeArray
      arraytype
      (cons 0 (1- (length nList)))
    )
    nList
  )
)

(defun BF-point-3d->2d (3dpt)
  (if (listp 3dpt)
    (list (float (car 3dpt)) (float (cadr 3dpt)))
  )
)

(defun BF-math-TransPt (base usrpt transpt ang)
  (car (BF-Mat-RotateByMatrix
				 (BF-Mat-TranslateByMatrix (list transpt) base usrpt)
				 usrpt
				 (- ang)
			 )
  )
)

(defun BF-math-maxlist (lst)
  (if (atom lst)
    lst
    (apply 'max lst)
  )
)

(defun BF-math-minlist (lst)
  (if (atom lst)
    lst
    (apply 'min lst)
  )
)

(defun BF-dcl-PrintDcl (x f / _PrintItem _PrintList _Main file)
	
  (defun _PrintItem (_PrintMethod item indents)
    (cond
      (item
        (princ "\n" file)
        (repeat indents (princ "    " file))
        (_PrintMethod item file)
      )
      ((princ " { }" file))
    )
    (princ)
  )
	
  (defun _PrintList (_PrintMethod lst indents)
    (if  (< -1 indents)
      (_PrintItem _PrintMethod "{" indents)
    )
    ((lambda (i) (foreach x lst (_Main x i))) (1+ indents))
    (if  (< -1 indents)
      (_PrintItem _PrintMethod "}" indents)
      (princ)
    )
  )
	
  (defun _Main (x indents)
    (if  (vl-consp x)
      (if ((lambda (x) (and x (atom x))) (cdr x))
			  (_PrintItem princ x indents)
			  (_PrintList princ x indents)
      )
      (_PrintItem princ x indents)
    )
  )
	
  (cond
    ((setq file (open f "w"))
      (_Main x -1)
      (close file)
			;(startapp "notepad" f)
    )
  )
)

(defun BF-dcl-setDialog  (dialogname layoutlst lst)
  (list  dialogname
	  (append (BF-dcl-listsplit layoutlst) (apply 'append lst))
  )
)

(defun BF-return (value) value)

(defun BF-dcl-getId (dcl_file / dcl_handle)
  (cond
    ;;����Ѿ���dcl�ļ����ͷ���dcl�ļ����
    ((BF-return (cdr (assoc dcl_file *user-dclfile-dclid-list*)))
    )
    ;; ���û���ҵ�dcl�ļ����͵����Ի��򣬲�����nil
    ((not (findfile dcl_file))
      (alert
        (strcat
			    "δ�ҵ��Ի������ļ� "
			    dcl_file
			    ".dcl\n����֧��Ŀ¼��"
        )
      )
      (BF-return nil)
    )
		
    ;; �ҵ��ļ���������ز��ɹ��������Ի��򣬷���nil
    ((or (not (setq dcl_handle (load_dialog dcl_file)))
			 (> 1 dcl_handle)
		 )
      (alert
        (strcat
			    "�޷����ضԻ�������ļ� "
			    dcl_file
			    "\n����֧��Ŀ¼��"
        )
      )
      (BF-return nil)
    )
		
    ;; �ҵ��ļ������Ҽ��سɹ������ļ����;����ɵ�Ա����ڳ����У������ؾ��
    (t
      (setq *user-dclfile-dclid-list*
	      (BF-AssocList-AddItem
	        *user-dclfile-dclid-list*
	        (cons dcl_file dcl_handle)
	      )
      )
      (BF-return dcl_handle)
    )
  )
)

(defun BF-dcl-listsplit  (lst)
  (if (listp lst)
    (mapcar
      '(lambda (x / tmp)
				 (strcat
					 (strcase (vl-princ-to-string (car x)) t)
					 " = "
					 (if (BF-stringp (setq tmp (cdr x)))
						 (vl-prin1-to-string tmp)
						 (strcase (vl-prin1-to-string tmp) t)
					 )
					 ";"
				 )
			 )
      lst
    )
  )
)

(defun BF-dcl-setAction  (lst)
  (if (listp lst)
		;(mapcar 'action_tile (mapcar 'car lst) (mapcar 'cadr lst))
    (mapcar
      '(lambda (x)
				 (BF-CatchApply
					 'action_tile
					 (cons
						 (car x)
						 (list
							 (apply
								 'strcat
								 (mapcar 'vl-prin1-to-string (cdr x))
							 )
						 )
					 )
				 )
			 )
      lst
    )
  )
)

(defun BF-dcl-addlist (key lst)
	;(cond
	;  ((and (null operation) (null index))
	;    (start_list key)
	;  )
	;  ((and (null index) operation)
	;    (start_list key operation)
	;  )
	;  (t (start_list key operation index))
	;)
	
  (if (= 'sym (type (car lst)))
    (setq lst (eval lst))
  )
	
  (start_list key)
  (mapcar 'add_list (mapcar 'vl-princ-to-string lst))
  (end_list)
)

(defun BF-dcl-loadsld (key sld / x y)
  (setq x (dimx_tile key))
  (setq y (dimy_tile key))
  (start_image key)
  (cond  ((numberp sld) (fill_image 0 0 x y sld))
	  (t
	    (fill_image 0 0 x y -2)
	    (slide_image 0 0 x y sld)
	  )
  )
  (end_image)
)

(defun BF-Ent-ListCollection (collection / out)
  (vlax-for each collection
    (setq out (cons (vla-get-Name each) out))
  )
  (reverse out)
)

(defun BF-Layers nil
  (eval
    (list 'defun
	    'BF-Layers
	    'nil
	    (vla-get-Layers
	      (vla-get-activedocument (vlax-get-acad-object))
	    )
    )
  )
  (BF-Layers)
)

(defun BF-TextStyles ()
  (eval
    (list 'defun
	    'BF-TextStyles
	    'nil
	    (vla-get-TextStyles
	      (vla-get-activedocument (vlax-get-acad-object))
	    )
    )
  )
  (BF-TextStyles)
)

(defun BF-AssocList-Remove (lst key)
  (vl-remove-if '(lambda (x) (equal (car x) key)) lst)
)

(defun BF-Vec-Norm (v)
  (sqrt (apply '+ (mapcar '* v v)))
)

(defun BF-Mat-RotateByMatrix (target p1 ang / m)
  (BF-Mat-ApplyMatrixTransformation
    target
    (setq m
      (list
        (list (cos ang) (- (sin ang)) 0.)
        (list (sin ang) (cos ang) 0.)
        (list 0. 0. 1.)
      )
    )
    (mapcar '- p1 (BF-Mat-MxV m p1))
  )
)

(defun BF-Mat-TranslateByMatrix  (target p1 p2)
  (BF-Mat-ApplyMatrixTransformation
    target
    (list
      (list 1. 0. 0.)
      (list 0. 1. 0.)
      (list 0. 0. 1.)
    )
    (mapcar '- p2 p1)
  )
)

(defun BF-AssocList-AddItem (lst value)
  (if (assoc (car value) lst)
    lst
    (cons value lst)
  )
)

(defun BF-stringp (arg) (equal (type arg) 'str))

(defun BF-CatchApply (fun args / result)
  (if
    (not
      (vl-catch-all-error-p
			  (setq result
          (vl-catch-all-apply
			      (if (= 'SYM (type fun))
			        fun
			        (function fun)
			      )
			      args
          )
			  )
      )
    )
		result
  )
)

(defun BF-Mat-ApplyMatrixTransformation  (target matrix vector)
  (cond
    ((eq 'VLA-OBJECT (type target))
      (vla-TransformBy
        target
        (vlax-tMatrix
			    (append (mapcar (function (lambda (x v) (append x (list v))))
										matrix
										vector
									)
			      '((0. 0. 0. 1.))
			    )
        )
      )
    )
    ((listp target)
      (mapcar
        (function
			    (lambda (point)
			      (mapcar '+ (BF-Mat-MxV matrix point) vector)
			    )
        )
        target
      )
    )
  )
)

(defun BF-Mat-MxV (m v)
  (mapcar (function (lambda (r) (apply '+ (mapcar '* r v))))
    m
  )
)

(Defun BF-Excel-New (ishide / Rtn)
  (if (setq Rtn (vlax-get-or-create-object "Excel.Application"))
    (progn
      (vlax-invoke
			  (vlax-get-property Rtn 'WorkBooks)
			  'Add
      )
      (if ishide
			  (vla-put-visible Rtn 1)
			  (vla-put-visible Rtn 0)
      )
    )
  )
  Rtn
)

(defun BF-Excel-MergeRange (xlapp index / range)
  (vlax-invoke-method
    (BF-Excel-getRange
      xlapp
      (BF-Excel-Utils-index-cells->range index)
    )
    'Merge
  )
  (BF-Excel-getRange XLApp index)
)

(Defun BF-Excel-setRangeValue (XLApp index value / range)
  (setq range (BF-Excel-getRange XLApp index))
  (if (= 'list (type value))
    (progn
      (vlax-for  it range
			  (vlax-put-property it 'value2 (car value))
			  (setq value (cdr value))
      )
    )
    (progn
      (vlax-for  it range
			  (vlax-put-property it 'value2 value)
      )
    )
  )
)

(defun BF-Excel-getRange (xlapp index)
  (vlax-get-property
    (vlax-get-property
      (vlax-get-property xlapp 'ActiveWorkbook)
      'ActiveSheet
    )
    'range
    (BF-Excel-Utils-index-cells->range index)
  )
)

(defun BF-Excel-Utils-index-cells->range (lst / num->col)
  (defun num->col (n)      ;����תΪ�У�leemac
    (if  (< n 27)
      (chr (+ 64 n))
      (strcat (num->col (/ (1- n) 26))
        (num->col (1+ (rem (1- n) 26)))
      )
    )
  )
  (if (= 'list (type lst))
    (cond
      ((= 2 (length lst))
        (strcat (num->col (cadr lst)) (itoa (car lst)))
      )
      ((= 4 (length lst))
        (strcat (num->col (cadr lst))
          (itoa (car lst))
          ":"
          (num->col (last lst))
          (itoa (caddr lst))
        )
      )
      (t
        "A1"
      )
    )
    lst
  )
)

(print "��ӭʹ�������ע���ߣ����bzb")
(princ)