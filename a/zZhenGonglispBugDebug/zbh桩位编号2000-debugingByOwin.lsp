;����������ƶ�;
(defun sx9 (#k #heigth  /  %k i gr n pt)
 (setq %k T  ;ѭ������
       i nil ;��ʼ����
 )
 (while %k
  (setq  gr (grread t 4 0);;ȡ��������������
         n (car gr)       ;;������
         pt (cadr gr)     ;;�������
  )
  (if (= n 5);;û�в���
   (progn
    (if (/= i nil) (entdel i) );;��������־�ɾ��
    (entmake 
    (list 
    '(0 . "TEXT")      ;;����
    (cons 1 #k)        ;;��������
    (cons 40 #heigth)      ;;���ָ߶�
    (cons 41 0.7)      ;;������б��
    ;(cons 50 #rotate)
    (cons 10 pt)       ;;���ֲ����
    (cons 7 "standard");; ������ʽ
    (cons 8 "0")       ;;����ͼ��
    ));;������������
    (setq i (entlast));;�õ�����ͼԪ��
   )
  )
  (if (= n 3) (setq %k nil) );;3��ʾ���;����ѭ��
  (if (or (= n 2) (= n 25));;2��ʾ�ո�;25��ʾ�Ҽ�;����ѭ����ɾ������
   (progn
    (setq %k nil)
    (entdel i)
   )
  )
 )
)  ; end of defun��

;д׮��;
(setq bh_began 1)
(setq text_offset 3.0)
(setq Newtextsize (getvar "textsize"))
(setq texthome "")
(setq textend "")

(defun c:zbh (/ error_old textstyle_old zbh_ok bho dimzin_old TEXTSIZE_old
		old_lay netbeetmp1 netbeetmp2��pt pt1
	     )
  (prompt "׮λ�Զ���ű�ע����\n")

  ;; ��ʼ��
  (setini_bh)			       ; ��while���Ʊ�ע��ѭ��,�س��˳���ע
  (while (progn
	   (while (progn
		    (setq pt nil
			  netbeetmp1 nil
			  netbeetmp1 (strcat "\nѡ���ע��" " [ǰ׺(M):"
					     texthome "/��׺(E):" textend
					     "/�ָ�(H):" (rtos Newtextsize)
					     "/ƫ����(O):" (rtos text_offset)
					     "/���(N):" (itoa bh_began)
					     "/ѡ�񹤳�׮(Z)]:"
				     ) ; _����strcat

		    )		       ; _����setq
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
		      )		       ; _����cond

		      (not t)
		    )		       ; _����if

		  )		       ; _����progn
	   )			       ; _����while
	   (if (not pt)
	     (not t)
	     (not nil)
	   )			       ; _����if
	 )			       ; _����progn

    ;; �رղ�׽��ʽ
    (setq osmode_old (getvar "osmode"))
    (setvar "osmode" 0)
    (setq pt1 (list (+ (car pt) text_offset) (+ (cadr pt) text_offset))) ; _
				       ; ����setq

    ;; ����ż���ǰ��׺
    (setq zbh_ok (strcat texthome (itoa bh_began) textend))

    ;(command "_.text" "s" "ZBH" pt1 Newtextsize "0d" zbh_ok) ; (setq bho
				       ; (entlast))
				       ; (command "move" bho "" pt pt1)
    (sx9 zbh_ok Newtextsize ) ;д׮������;
    (setq bh_began (1+ bh_began))

    ;; �ָ���׽��ʽ
    (setvar "osmode" osmode_old)
  )				       ; _����while

  (setclose_bh)
)
;;; _����defun

;;; --------------------------------------------
;;; ��׮λ���
(defun zbh_zw (/ sszwd sszw sslen loop_i loop_j)
  (setq sszw (ssget '((0 . "CIRCLE")   ; (8 . "׮λ")
		    )
	     )			       ; _����ssget

  )				       ; _����setq
  (setq sslen (sslength sszw)
	loop_i 1.0
  )				       ; _����setq
  (if (< sslen 1000)
    (prompt "\n ���ڽ��м���,���Ե�...\n")
    (prompt "\n ���ݹ��࣬����ʱ�������Ҫ�����ӣ���ȴ�......\n")
  )				       ; ��ȡ����׮λ���굽��sszwd
  (while (<= loop_i sslen)
    (setq sszwd (append
		  sszwd
		  (list (cdr (assoc 10 (entget (ssname sszw (1- loop_i))))))
		)		       ; _����append

    )				       ; _����setq
    (setq loop_i (1+ loop_i))
  )				       ; _����while
				       ; ����sszwd���ݴ�����,���ϵ��µ�
				       ; ��������(���뷨).
  (zbh_px2)			       ; ��һ�ӱ�sszwd����ȡ������б�ע
  (setq osmode_old (getvar "osmode"))
  (setvar "osmode" 0)
  (setq loop_i 0)
  (while (<= loop_i (1- sslen))	       ; �رղ�׽��ʽ
    (setq pt (nth loop_i sszwd))
    (setq pt1 (list (+ (car pt) text_offset) (+ (cadr pt) text_offset))) ; _
				       ; ����setq

    ;; ����ż���ǰ��׺
    (setq zbh_ok (strcat texthome (itoa bh_began) textend))

    (command "_.text" "s" "ZBH" pt1 Newtextsize "0d" zbh_ok) ; (setq bho
				       ; (entlast))
				       ; (command "move" bho "" pt pt1)
    (setq bh_began (1+ bh_began))

    ;; �ָ���׽��ʽ
    (setq loop_i (1+ loop_i))
  )				       ; _����while
  (setvar "osmode" osmode_old)


)
;;; _����defun


;;; ----------------------------------------
;;; ��list_o�ڵ�I,��JԪ�ػ�
;;; (listIJ_JI list_o list_i list_j)
(defun listij_ji (list_o list_i list_j / ltmep ltemp1 ltemp2 loop_i loop_j)
  (setq loop_i 0
	loop_j 0
	listlen (length list_o)
	ltemp (list (getvar "CDATE"))
	ltemp1 (nth list_i list_o)
	ltemp2 (nth list_j list_o)


  )				       ; _����setq
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
;;; _����defun

;;; --------------------------------------------------------
;;; ׮λ��ų�ʼ��
(defun setini_bh ()
  (setq textstyle_old (getvar "textstyle"))
  (setvar "cmdecho" 0)
  (setq old_lay (getvar "clayer"))
  (setq dimzin_old (getvar "Dimzin"))
  (setvar "dimzin" 8)
  (setq error_old *error*)
  (gc)				       ; �ж�����ͼ��ZBH
  (if (= (tblobjname "LAYER" "ZBH") nil)
    (progn
      (entmake (list '(0 . "LAYER") '(100 . "AcDbSymbolTableRecord") '
		     (100 . "AcDbLayerTableRecord") '(6 . "CONTINUOUS") '
		     (62 . 3) '(70 . 0) (cons 2 "ZBH")
	       )		       ; _����list
      )				       ; _����entmake
    )				       ; _����progn
  )				       ; _����if
  (setvar "clayer" "ZBH")

  ;; �ж�����������ZBH
  (if (= (tblobjname "STYLE" "ZBH") nil)
    (progn
      (entmake (list '(0 . "STYLE") '(100 . "AcDbSymbolTableRecord") '
		     (100 . "AcDbTextStyleTableRecord") '(3 . "txt") '
		     (40 . 0) '(41 . 0.6) '(50 . 0) '(70 . 0) (cons 2 "ZBH")
	       )		       ; _����list
      )				       ; _����entmake
    )				       ; _����progn
  )				       ; _����if

)
;;; _����defun

;;; ------------------------------------------------------------
(defun textoffset ()
  (setq netbeetmp1 nil
	netbeetmp1 text_offset
  )				       ; _����setq
  (prompt "\n�������עƫ����<")
  (princ text_offset)
  (prompt ">:")
  (if (= (setq text_offset (getreal))
	 nil
      )
    (setq text_offset netbeetmp1)
  )				       ; _����if

)
;;; _����defun
;;; --------------------------------------------------------------
(defun settexthome ()
  (setq netbeetmp1 nil
	netbeetmp1 texthome
  )				       ; _����setq
  (prompt "\n��������ǰ׺(��ǰ׺������\"*\")<")
  (Princ texthome)
  (Prompt ">:")
  (if (= (setq texthome (getstring))
	 ""
      )
    (setq texthome netbeetmp1)
  )				       ; _����if
  (if (= texthome "*")
    (SETQ texthome "")
  )				       ; _����if

)
;;; _����defun

;;; --------------------------------------------------------
(defun settextend ()

  (setq netbeetmp1 nil
	netbeetmp1 textend
  )				       ; _����setq
  (prompt "\n�������ź�׺(�޺�׺������\"*\")<")
  (Princ textend)
  (Prompt ">:")
  (if (= (setq textend (getstring))
	 ""
      )
    (setq textend netbeetmp1)
  )				       ; _����if
  (if (= textend "*")
    (SETQ textend "")
  )				       ; _����if

)
;;; _����defun
;;; --------------------------------------------------------
(defun settextbegan ()
  (setq netbeetmp1 nil
	netbeetmp1 bh_began
  )				       ; _����setq
  (if (= (setq bh_began (getint (strcat "\n��ʼ���:1/2/3...<"
					(itoa bh_began) ">:"
				)
			)	       ; _����getint

	 )			       ; _����setq
	 nil
      )				       ; _����=
    (setq bh_began netbeetmp1)
  )				       ; _����if

)
;;; _����defun
;;; -------------------------------------------------------
;;; �ָ�����
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
;;; _����defun

;;; -----------------------------------------------------


;;; -------------------------------------------------------
;;; �����ָߺ���
(defun settexth ()
  (setq netbeetmp1 nil
	netbeetmp1 newtextsize
	netbeetmp2 nil
	netbeetmp2 (strcat "\n�����ָ�:<" (rtos Newtextsize) ">:")
  )				       ; _����setq
  (if (= (setq Newtextsize (getreal netbeetmp2))
	 nil
      )
    (setq Newtextsize netbeetmp1)
  )				       ; _����if


)
;;; _����defun
;;; -------------------------------------------------------
(defun *error* (msg)
  (setclose_bh)
)
;;; _����defun

;;; -------------------------------------------
;;; ����sszwd���ݴ�����,���ϵ��µĹ�������(���뷨).
(defun zbh_px2 (/ sstmp1 sstmp2 loop_i loop_j pt1x pt1y pt2x pt2y) ; �����ڲ�����
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
	     )			       ; _����setq
	     (cond		       ; ��ָ��i�Ѿ�����ָ��j��λ��������
	       ((= loop_i loop_j)
		 (not 1)
	       )		       ; ���ڶ����Y�ȵ�һ��Ĵ�,��ѵڶ���
	                               ;���뵽��һǰ
				       ; ������
	       ((< pt1y pt2y)
		 (listij_in)
		 (not 1)
	       )		       ; _����һ���Y��ڶ����Y���,�ҵڶ�
	                               ; ���X�ȵ�һ���С,
				       ; ��ѵڶ�����뵽��һ��ǰ��������
	       ((and
		  (= pt1y pt2y)
		  (> pt1x pt2x)
		)
		 (listij_in)
		 (not 1)
	       )

	       ;; ��������򲻽��л���
	       (t
		 (setq loop_i (1+ loop_i))
	       )
	     )			       ; _����cond
	   )
    )				       ; _����while
    (setq loop_j (1+ loop_j))
  )				       ; _����while
  sstmp
)


(Prompt "\n[������ͼϵͳ1.5] ��Ȩ���� �۷����幤���� 2000��")
(prompt "\n׮λ������zbh")