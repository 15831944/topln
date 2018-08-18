(VL-LOAD-COM)
(if (NOT (setq V_LANGUAGE
		(VL-REGISTRY-READ
		  "HKEY_CURRENT_USER\\Software\\NBSoft\\NBDimCoord"
		  "Language"
		)
	 )
    )
  (PROGN (setq V_LANGUAGE "zh-cn"))
)
(DEFUN NBTF_GETLANGUAGESTR (STRLST)
  (COND	((= V_LANGUAGE "zh-cn") (CAR STRLST))
	((= V_LANGUAGE "en") (CADR STRLST))
	((= V_LANGUAGE "zh-tw")
	 (if (CADDR STRLST)
	   (PROGN (CADDR STRLST))
	   (PROGN (CAR STRLST))
	 )
	)
	(T (CAR STRLST))
  )
)
(DEFUN NBTF_LST_SUBSTNTH (INDEX NEW LST / N RE ITEM)
  (setq N 0)
  (FOREACH ITEM	LST
    (if	(= N INDEX)
      (PROGN (setq RE (APPEND RE (LIST NEW))))
      (PROGN (setq RE (APPEND RE (LIST ITEM))))
    )
    (setq N (1+ N))
  )
  RE
)
(DEFUN NBTF_VXACADAPPOBJECT ()
  (COND	(*NBTV-ACAD-OBJECT*)
	(T (setq *NBTV-ACAD-OBJECT* (vlax-get-acad-object)))
  )
)
(DEFUN NBTF_VXACTIVEDOCUMENT ()
  (if *NBTV-ACTIVE-DOCUMENT*
    (PROGN (vlax-release-object *NBTV-ACTIVE-DOCUMENT*)
	   (setq *NBTV-ACTIVE-DOCUMENT* nil)
    )
  )
  (setq	*NBTV-ACTIVE-DOCUMENT*
	 (vla-get-ActiveDocument
	   (NBTF_VXACADAPPOBJECT)
	 )
  )
)
(DEFUN NBTF_VXMODELSPACE ()
  (if *NBTV-MODEL-SPACE*
    (PROGN (vlax-release-object *NBTV-MODEL-SPACE*)
	   (setq *NBTV-MODEL-SPACE* nil)
    )
  )
  (setq *NBTV-MODEL-SPACE* (vla-get-ModelSpace (NBTF_VXACTIVEDOCUMENT)))
)
(DEFUN NBTF_VXACTIVESPACE (/ STAT)
  (setq STAT (vla-get-ActiveSpace (NBTF_VXACTIVEDOCUMENT)))
  (COND	((= STAT acModelSpace) (setq STAT (NBTF_VXMODELSPACE)))
	((= STAT acPaperSpace) (setq STAT (NBTF_VXPAPERSPACE)))
  )
  STAT
)
(DEFUN NBTF_VXPAPERSPACE ()
  (if *NBTV-PAPER-SPACE*
    (PROGN (vlax-release-object *NBT-PAPER-SPACE*)
	   (setq *NBTV-PAPER-SPACE* nil)
    )
  )
  (setq *NBTV-PAPER-SPACE* (vla-get-PaperSpace (NBTF_VXACTIVEDOCUMENT)))
)
(DEFUN NBTF_DXF1 (ST SS)
  (if (= (TYPE SS) 'ENAME)
    (PROGN (CDR (ASSOC ST (ENTGET SS '("*")))))
    (PROGN (CDR (ASSOC ST SS)))
  )
)
(DEFUN NBTF_DXF	(ST SS)
  (if (= (TYPE SS) 'ENAME)
    (PROGN (CDR (ASSOC ST (ENTGET SS '("*")))))
    (PROGN (CDR (ASSOC ST SS)))
  )
)
(DEFUN NBTF_ENTSEL (MSG KEYWORD FILTER / KEYINI EL SS PT)
  (INITGET KEYWORD)
  (setq EL (ENTSEL (STRCAT MSG "/<退出选择>:")))
  (while (and (NOT (COND ((AND (= nil EL) (= 52 (GETVAR "errno")))
			  (NOT (setq EL nil))
			 )
			 ((= 'STR (TYPE EL)) EL)
			 ((= 'LIST (TYPE EL))
			  (command "_.select")
			  (command (CAR EL))
			  (command "")
			  (SSGET "P" FILTER)
			 )
		   )
	      )
	 )
    (PRINC "\n未选中有效实体!")
  )
  EL
)
(DEFUN NBTF_VXLAYERSID ()
  (setq *NBTV-LAYERS* (vla-get-Layers (NBTF_VXACTIVEDOCUMENT)))
)
(DEFUN NBTF_VXACTIVEDOCUMENT ()
  (if *NBTV-ACTIVE-DOCUMENT*
    (PROGN (vlax-release-object *NBTV-ACTIVE-DOCUMENT*)
	   (setq *NBTV-ACTIVE-DOCUMENT* nil)
    )
  )
  (setq	*NBTV-ACTIVE-DOCUMENT*
	 (vla-get-ActiveDocument
	   (NBTF_VXACADAPPOBJECT)
	 )
  )
)
(DEFUN NBTF_VXACADAPPOBJECT ()
  (COND	(*NBTV-ACAD-OBJECT*)
	(T (setq *NBTV-ACAD-OBJECT* (vlax-get-acad-object)))
  )
)
(DEFUN NBTF_VXADDMYXDATA (OBJ LL / OBJID XLIST BOOL XL)
  (REGAPP "NBT_OBJID")
  (setq OBJID (CAR LL))
  (setq LL (CDR LL))
  (if LL
    (PROGN
      (FOREACH A LL
	(REGAPP (CAR A))
	(setq XLIST
	       (COND ((= (TYPE (CADR A)) 'STR) (CONS 1000 (CADR A)))
		     ((= (TYPE (CADR A)) 'INT) (CONS 1070 (CADR A)))
		     ((= (TYPE (CADR A)) 'ENAME) (CONS 1005 (CADR A)))
		     ((= (TYPE (CADR A)) 'REAL) (CONS 1040 (CADR A)))
		     ((AND (= (TYPE (CADR A)) 'LIST)
			   (OR (= (LENGTH A) 2) (= (LENGTH A) 3))
		      )
		      (CONS 1010 (CADR A))
		     )
		     (T (CONS 1000 "ERROR"))
	       )
	)
	(setq XLIST (CONS (CONS 1001 (CAR A)) (LIST XLIST)))
	(setq XL (CONS XLIST XL))
      )
    )
  )
  (if OBJID
    (PROGN
      (setq XL (CONS (LIST '(1001 . "NBT_OBJID") (CONS 1000 OBJID)) XL))
    )
  )
  (FOREACH A XL (NBTF_VXPUTXDATA OBJ A))
)
(DEFUN NBTF_VXPUTXDATA (EOBJ XDATA)
  (setq EOBJ (vlax-ename->vla-object EOBJ))
  (setq	XDATA
	 (NBTF_VXBUILDFILTER
	   (MAPCAR
	     '(LAMBDA (ITEM / KEY)
		(SETQ KEY (CAR ITEM))
		(IF (<= 1010 KEY 1033)
		  (CONS	KEY
			(vlax-variant-value (vlax-3d-point (CDR ITEM)))
		  )
		  ITEM
		)
	      )
	     XDATA
	   )
	 )
  )
  (vla-SetXData EOBJ (CAR XDATA) (CADR XDATA))
)
(DEFUN NBTF_VXBUILDFILTER (FILTER)
  (MAPCAR '(LAMBDA (LST TYP)
	     (vlax-make-variant
	       (vlax-safearray-fill
		 (vlax-make-safearray TYP (CONS 0 (1- (LENGTH LST))))
		 LST
	       )
	     )
	   )
	  (LIST (MAPCAR 'CAR FILTER) (MAPCAR 'CDR FILTER))
	  (LIST vlax-vbInteger vlax-vbVariant)
  )
)
(DEFUN NBTF_VXGETMYXDATA (OBJ APPID / XLIST XL L1 NA A)
  (setq XLIST (NBTF_VXGETXDATA OBJ APPID))
  (setq NA "ERROR")
  (while (and (setq A (CAR XLIST)))
    (if	(AND (= (CAR A) 1001) (= (CDR A) "NBT_OBJID"))
      (PROGN (setq NA (CDADR XLIST)) (setq XLIST (CDDR XLIST)))
      (PROGN (if (= (CAR A) 1001)
	       (PROGN (setq L1 (CDR A)))
	       (PROGN (setq L1 (CONS L1 (CONS (CDR A) nil)))
		      (setq XL (CONS L1 XL))
	       )
	     )
	     (setq XLIST (CDR XLIST))
      )
    )
  )
  (CONS NA XL)
)
(DEFUN NBTF_VXGETXDATA (VLAOBJ APPID / XTYPE XDATA)
  (setq VLAOBJ (vlax-ename->vla-object VLAOBJ))
  (vla-GetXData VLAOBJ APPID 'XTYPE 'XDATA)
  (if (AND XTYPE XDATA)
    (PROGN
      (MAPCAR
	'(LAMBDA (KEY VAL)
	   (CONS
	     KEY
	     (IF (= (TYPE (vlax-variant-value VAL)) (QUOTE safearray))
	       (vlax-safearray->list (vlax-variant-value VAL))
	       (vlax-variant-value VAL)
	     )
	   )
	 )
	(vlax-safearray->list XTYPE)
	(vlax-safearray->list XDATA)
      )
    )
  )
)
(DEFUN NBTF_SS_REDRAW (SS A / E I)
  (setq I 0)
  (while (and (setq E (SSNAME SS I)))
    (REDRAW E A)
    (setq I (1+ I))
  )
)
(DEFUN NBTF_DCL_VERIFY_DATA (A / STR STRN LP)
  (setq STR (GET_TILE A))
  (setq LP 0)
  (setq STRN 0)
  (if (= (SUBSTR STR 1 1) ".")
    (PROGN (setq STR (STRCAT "0" STR)))
  )
  (while (and (setq LP (VL-STRING-POSITION (ASCII ".") STR LP)))
    (setq STRN (1+ STRN))
    (setq LP (1+ LP))
  )
  (if (AND (OR (= STRN 1) (= STRN 0)) (NUMBERP (READ STR)))
    (PROGN (SET (READ A) (READ STR)))
    (PROGN (MODE_TILE A 2)
	   (VLR-beep-reaction)
	   (SET_TILE "error" "数据错误!--本编辑框只能是数字!!!")
	   (NOT T)
    )
  )
)
(DEFUN NBTF_DCL_UPDATE_LIST (KEY STR_LIST K)
  (START_LIST KEY K)
  (MAPCAR '(LAMBDA (X)
	     (IF (= (TYPE X) (QUOTE STR))
	       (ADD_LIST X)
	       (ADD_LIST (VL-PRIN1-TO-STRING X))
	     )
	   )
	  STR_LIST
  )
  (END_LIST)
)
(DEFUN NBTF_DCL_LIST_GET (LKEY / L1)
  (setq L1 (EVAL (READ LKEY)))
  (if (AND L1 (LISTP L1))
    (PROGN (NTH	(ATOI (if (GET_TILE LKEY)
			(PROGN (GET_TILE LKEY))
			(PROGN "0")
		      )
		)
		L1
	   )
    )
  )
)
(DEFUN NBTF_GETPLPTL (ENAME / PLIST PP N I)
  (setq OBJ (vlax-ename->vla-object ENAME))
  (setq	PLIST (vlax-safearray->list
		(vlax-variant-value (vla-get-Coordinates OBJ))
	      )
  )
  (setq N 0)
  (if (= "AcDbPolyline" (vla-get-ObjectName OBJ))
    (PROGN (setq I 2))
    (PROGN (setq I 3))
  )
  (REPEAT (/ (LENGTH PLIST) I)
    (setq PP (APPEND PP (LIST (LIST (NTH N PLIST) (NTH (1+ N) PLIST)))))
    (setq N (+ N I))
  )
  PP
)
(DEFUN NBTF_GETLDATA (LDATA / VARS)
  (COND
    ((= (TYPE LDATA) 'STR)
     (setq VARS (vlax-ldata-get "NBTF_var" LDATA))
     (SET (READ LDATA) VARS)
    )
    ((AND (= (TYPE LDATA) 'LIST) (= (TYPE (CAR LDATA)) 'STR))
     (setq VARS (vlax-ldata-get "NBTF_var" (CAR LDATA)))
     (if (/= (TYPE VARS) (TYPE (CADR LDATA)))
       (PROGN (setq VARS (CADR LDATA))
	      (NBTF_SETLDATA (CAR LDATA) (CADR LDATA))
       )
     )
     (SET (READ (CAR LDATA)) VARS)
    )
    (T
     (PROMPT
       "\n 参数错误：请用(NBTF_GetLdata varnawe)或(NBTF_GetLdata '(varname var))格式！"
     )
    )
  )
  VARS
)
(DEFUN NBTF_SETLDATA (LDATA VARS)
  (SET (READ LDATA) VARS)
  (vlax-ldata-put "NBTF_var" LDATA VARS)
)
(DEFUN NBTF_GETTEXTSTYLEWS (STYLENAME / TSTYLE TSTYLELST)
  (OR (setq TSTYLE (TBLOBJNAME "style" STYLENAME))
      (setq TSTYLE (TBLOBJNAME "style" (GETVAR "TEXTSTYLE")))
  )
  (setq TSTYLELST (ENTGET TSTYLE))
  (CDR (ASSOC 41 TSTYLELST))
)
(DEFUN NBTF_STRPARSE
       (STR DELIMITER / SEARCHSTR STRINGLEN RETURN N CHAR)
  (setq SEARCHSTR STR)
  (setq STRINGLEN (STRLEN SEARCHSTR))
  (setq RETURN nil)
  (while (and (> STRINGLEN 0))
    (setq N 1)
    (setq CHAR (SUBSTR SEARCHSTR 1 1))
    (while (and (AND (/= CHAR DELIMITER) (/= CHAR "")))
      (setq N (1+ N))
      (setq CHAR (SUBSTR SEARCHSTR N 1))
    )
    (setq RETURN (CONS (SUBSTR SEARCHSTR 1 (1- N)) RETURN))
    (setq SEARCHSTR (SUBSTR SEARCHSTR (1+ N) STRINGLEN))
    (setq STRINGLEN (STRLEN SEARCHSTR))
  )
  (REVERSE RETURN)
)
(DEFUN NBTF_STRUNPARSE (LST DELIMITER / RETURN)
  (setq RETURN "")
  (FOREACH STR LST
    (setq RETURN (STRCAT RETURN DELIMITER STR))
  )
  (SUBSTR RETURN 2)
)
(DEFUN NBTF_SET_SCREEN_CEN (C03 / C08 C04 C05 C07 C06 C09 C01 C02)
  (setq C03 (TRANS C03 1 2))
  (setq C08 (GETVAR "viewsize"))
  (setq C04 (GETVAR "screensize"))
  (setq C07 (CAR C04))
  (setq C06 (CADR C04))
  (setq C09 (/ (* C08 C07) C06))
  (setq C01 (LIST (- (CAR C03) (* 0.5 C09)) (- (CADR C03) (* 0.5 C08))))
  (setq C02 (LIST (+ (CAR C03) (* 0.5 C09)) (+ (CADR C03) (* 0.5 C08))))
  (setq C01 (TRANS C01 2 1))
  (setq C02 (TRANS C02 2 1))
  (VL-CMDF "_.zoom" "W" C01 C02)
)
(DEFUN NBTF_PT_AB->XY (PTU_XY PTU_AB ANG1 PT)
  (LIST	(+ (* (- (CAR PT) (CAR PTU_AB)) (COS ANG1))
	   (- 0.0 (* (- (CADR PT) (CADR PTU_AB)) (SIN ANG1)))
	   (CAR PTU_XY)
	)
	(+ (* (- (CADR PT) (CADR PTU_AB)) (COS ANG1))
	   (* (- (CAR PT) (CAR PTU_AB)) (SIN ANG1))
	   (CADR PTU_XY)
	)
	(if (CADDR PT)
	  (PROGN (CADDR PT))
	  (PROGN 0)
	)
  )
)
(DEFUN NBTF_BOX_DTEXT (ENT   /	   I	 ENAME ELIST LST   LST2	 TB
		       TB1   TB2   TLEN	 THI   LL    LR	   UR	 UL
		       ANG   ANGG  UCSANG      LLX   LRX   URX	 ULX
		       LLY   LRY   URY	 ULY   ALL   AVG   ERR	 CNTLST
		       DXF
		      )
  (setq	UCSANG (ANGLE (TRANS '(0.0 0.0 0.0) 1 0)
		      (TRANS '(1.0 0.0 0.0) 1 0)
	       )
  )
  (setq ELIST (ENTGET ENT))
  (setq ANG (CDR (ASSOC 50 ELIST)))
  (setq TB (TEXTBOX ELIST))
  (setq TB1 (CAR TB))
  (setq TB2 (CADR TB))
  (setq TLEN (- (CAR TB2) (CAR TB1)))
  (setq THI (- (CADR TB2) (CADR TB1)))
  (setq	LL (POLAR (TRANS (CDR (ASSOC 10 ELIST)) 0 1)
		  (+ ANG (/ PI 2))
		  (CADR TB1)
	   )
  )
  (setq ANG (- ANG UCSANG))
  (setq LR (POLAR LL ANG TLEN))
  (setq UR (POLAR LL (+ ANG (ANGLE TB1 TB2)) (DISTANCE TB1 TB2)))
  (setq UL (POLAR LL (+ ANG (/ PI 2)) THI))
  (LIST LL LR UR UL)
)
(DEFUN NBTF_BOXMTEXT
       (ENT / ELST TB LL UR UL LR EL PT APT TYP WDT HGT ANG ANG90)
  (AND (LISTP ENT) (setq ENT (CAR ENT)))
  (setq ELST (ENTGET ENT))
  (setq ANG90 (/ PI 2))
  (COND
    ((= (CDR (ASSOC 0 ELST)) "MTEXT")
     (setq APT (CDR (ASSOC 71 ELST)))
     (setq WDT (CDR (ASSOC 42 ELST)))
     (setq HGT (CDR (ASSOC 43 ELST)))
     (setq UL (TRANS (CDR (ASSOC 10 ELST)) 0 1))
     (setq ANG (CDR (ASSOC 50 ELST)))
     (COND ((= APT 1))
	   ((= APT 2) (setq UL (POLAR UL (+ ANG PI) (/ WDT 2.0))))
	   ((= APT 3) (setq UL (POLAR UL (+ ANG PI) WDT)))
	   ((= APT 4) (setq UL (POLAR UL (+ ANG ANG90) (/ HGT 2.0))))
	   ((= APT 5)
	    (setq UL (POLAR UL (+ ANG ANG90) (/ HGT 2.0)))
	    (setq UL (POLAR UL (+ ANG PI) (/ WDT 2.0)))
	   )
	   ((= APT 6)
	    (setq UL (POLAR UL (+ ANG ANG90) (/ HGT 2.0)))
	    (setq UL (POLAR UL (+ ANG PI) WDT))
	   )
	   ((= APT 7) (setq UL (POLAR UL (+ ANG ANG90) HGT)))
	   ((= APT 8)
	    (setq UL (POLAR UL (+ ANG ANG90) HGT))
	    (setq UL (POLAR UL (+ ANG PI) (/ WDT 2.0)))
	   )
	   ((= APT 9)
	    (setq UL (POLAR UL (+ ANG ANG90) HGT))
	    (setq UL (POLAR UL (+ ANG PI) WDT))
	   )
     )
     (setq UR (POLAR UL ANG WDT))
     (setq LR (POLAR UR (+ ANG (* PI 1.5)) HGT))
     (setq LL (POLAR LR (+ ANG PI) WDT))
    )
    (T (PROMPT "\n选择对象不是MTEXT."))
  )
  (if LL
    (PROGN (LIST LL LR UR UL))
  )
)
(DEFUN NBTF_XPKBOXLST (PT SC TF / PKHIGH PKRAD)
  (setq PKHIGH (* (GETVAR "viewsize") SC))
  (setq PKRAD (* 1.4144 PKHIGH))
  (if (> TF 0)
    (PROGN (LIST (POLAR PT (* 0.75 PI) PKRAD)
		 (POLAR PT (* 1.75 PI) PKRAD)
	   )
    )
    (PROGN (LIST (POLAR PT (* -0.75 PI) PKRAD)
		 (POLAR PT (* -1.75 PI) PKRAD)
	   )
    )
  )
)
(DEFUN NBTF_SETCLAYER (SLIST) (NBTF_VXSETLAYER SLIST))
(DEFUN NBTF_VXSETLAYER (LL / CTMP LAYNAME COR LSTY TMP LPRINT)
  (setq	CTMP
	 (STRCAT
	   "\n(NBTF_VXSetlayer '(层名 颜色 [线型] [是否打印(为真不打)]))！"
	   "\n(NBTF_VXSetlayer \"轴线\")设置当前图层"
	   "\n(NBTF_VXSetlayer '())恢复前一图层"
	 )
  )
  (COND	((= (TYPE LL) 'LIST)
	 (setq LAYNAME (CAR LL))
	 (setq COR (CADR LL))
	 (setq LSTY (CADDR LL))
	 (setq LPRINT (NTH 3 LL))
	 (if (NOT LSTY)
	   (PROGN (setq LSTY "CONTINUOUS"))
	 )
	)
	((= (TYPE LL) 'STR) (setq LAYNAME LL))
  )
  (COND
    ((= LL '?) (ALERT CTMP))
    ((NOT LL)
     (if NB_VCLAYER_OLD
       (PROGN (SETVAR "clayer" NB_VCLAYER_OLD))
       (PROGN (SETVAR "clayer" "0"))
     )
    )
    ((AND LAYNAME
	  COR
	  (/= LAYNAME "")
	  (= (TYPE LAYNAME) 'STR)
	  (= (TYPE COR) 'INT)
     )
     (setq NB_VCLAYER_OLD (GETVAR "clayer"))
     (if (setq TMP (TBLOBJNAME "LAYER" LAYNAME))
       (PROGN (setq TMP (vlax-ename->vla-object TMP))
	      (if (TBLOBJNAME "Ltype" LSTY)
		(PROGN (vla-put-Linetype TMP LSTY))
	      )
	      (if LPRINT
		(PROGN (vla-put-Plottable
			 TMP
			 (vlax-make-variant nil vlax-vbBoolean)
		       )
		)
	      )
	      (vla-put-Color TMP COR)
       )
       (PROGN (setq TMP (vlax-invoke-method (NBTF_VXLAYERSID) 'ADD LAYNAME))
	      (if (TBLOBJNAME "Ltype" LSTY)
		(PROGN (vla-put-Linetype TMP LSTY))
	      )
	      (if LPRINT
		(PROGN (vla-put-Plottable
			 TMP
			 (vlax-make-variant nil vlax-vbBoolean)
		       )
		)
	      )
	      (vla-put-Color TMP COR)
       )
     )
     (SETVAR "clayer" LAYNAME)
    )
    ((AND (/= LAYNAME "") (= (TYPE LAYNAME) 'STR))
     (setq NB_VCLAYER_OLD (GETVAR "clayer"))
     (if (TBLOBJNAME "LAYER" LAYNAME)
       (PROGN (SETVAR "clayer" LAYNAME))
       (PROGN (vlax-invoke-method (NBTF_VXLAYERSID) 'ADD LAYNAME))
     )
     (SETVAR "clayer" LAYNAME)
    )
    (T
     (ALERT (STRCAT "\n数据错误:"
		    CTMP
		    "\n现在的参数为:"
		    (VL-PRIN1-TO-STRING LL)
	    )
     )
    )
  )
  (PRINC)
)
(DEFUN NBTF_SETTEXTSTYLE (SLIST / TLIST TEXTN TEXTF CTMP TF)
  (setq	CTMP
	 (STRCAT
	   "\n(NBTF_setTextstyle '((2. \"宋体\")(3 . \"SIMSUN.TTF\")(40 . 0)(41 . 0.8)...)！"
	   "\n(NBTF_setTextstyle \"宋体\")设置当前字体"
	   "\n各参数意义如下:"
	   "\n2\t样式名称"
	   "\n40\t固定文字高度；如果高度不固定则值为 0"
	   "\n41\t宽度因子"
	   "\n50\t倾斜角"
	   "\n71\t文字生成标志:"
	   "\n        2 = 文字反向（X 轴方向镜像）"
	   "\n        4 = 文字倒置（Y 轴方向镜像）"
	   "\n42\t最后使用的高度"
	   "\n3\t主字体文件名"
	   "\n4\t大字体文件名；如果没有则为空"
	  )
  )
  (setq TF nil)
  (if (= (TYPE SLIST) 'LIST)
    (PROGN (setq TEXTN (if (ASSOC 2 SLIST)
			 (PROGN (CDR (ASSOC 2 SLIST)))
		       )
	   )
	   (setq TEXTF (if (ASSOC 3 SLIST)
			 (PROGN (CDR (ASSOC 3 SLIST)))
		       )
	   )
	   (setq SLIST
		  (if (NOT (ASSOC 3 SLIST))
		    (PROGN (setq TEXTF "txt") (APPEND SLIST '((3 . "txt"))))
		    (PROGN SLIST)
		  )
	   )
	   (setq SLIST (if (NOT (ASSOC 40 SLIST))
			 (PROGN (APPEND SLIST '((40 . 0.0))))
			 (PROGN SLIST)
		       )
	   )
	   (setq SLIST (if (NOT (ASSOC 41 SLIST))
			 (PROGN (APPEND SLIST '((41 . 1.0))))
			 (PROGN SLIST)
		       )
	   )
	   (setq SLIST (if (NOT (ASSOC 50 SLIST))
			 (PROGN (APPEND SLIST '((50 . 0.0))))
			 (PROGN SLIST)
		       )
	   )
	   (setq SLIST (APPEND
			 '((0 . "STYLE"))
			 '((100 . "AcDbSymbolTableRecord"))
			 '((100 . "AcDbTextStyleTableRecord"))
			 '((70 . 0))
			 SLIST
			)
	   )
    )
  )
  (if (= (TYPE SLIST) 'STR)
    (PROGN (setq TEXTN SLIST))
  )
  (COND	((= SLIST '?) (ALERT CTMP))
	((NOT SLIST)
	 (if STYLE_OLD
	   (PROGN (SETVAR "TEXTSTYLE" CLAYER_OLD))
	   (PROGN (SETVAR "TEXTSTYLE" "standard"))
	 )
	 (setq TF T)
	)
	((AND TEXTN TEXTF (= (TYPE SLIST) 'LIST))
	 (if (TBLOBJNAME "STYLE" TEXTN)
	   (PROGN (setq	SLIST
			 (APPEND
			   (LIST (ASSOC -1 (ENTGET (TBLOBJNAME "style" TEXTN))))
			   SLIST
			 )
		  )
		  (setq TEST SLIST)
		  (ENTMOD SLIST)
		  (setq TF T)
	   )
	   (PROGN (ENTMAKE SLIST) (setq TF T))
	 )
	 (setq STYLE_OLD (GETVAR "TEXTSTYLE"))
	 (SETVAR "TEXTSTYLE" TEXTN)
	)
	((AND (= (TYPE SLIST) 'STR))
	 (if (TBLOBJNAME "STYLE" TEXTN)
	   (PROGN (SETVAR "TEXTSTYLE" TEXTN)
		  (setq TF T)
		  (setq STYLE_OLD (GETVAR "TEXTSTYLE"))
		  (SETVAR "TEXTSTYLE" TEXTN)
	   )
	 )
	)
	(T)
  )
  TF
)
(DEFUN NBTF_POINTXY->AB
       (PT0 ANG SCALE PT1 / PIX2 I PT MATRIX X Y Z X1 Y1 Z1)
  (setq PIX2 (* PI 2))
  (setq X1 (- (CAR PT1) (CAR PT0)))
  (setq Y1 (- (CADR PT1) (CADR PT0)))
  (setq Z1 0)
  (setq	MATRIX
	 (LIST
	   (LIST (* SCALE (COS ANG)) (* SCALE (SIN ANG)) 0 (CAR PT0))
	   (LIST (* SCALE (SIN (- PIX2 ANG)))
		 (* SCALE (COS (- PIX2 ANG)))
		 0
		 (CADR PT0)
	   )
	   (LIST 0 0 1 0)
	 )
  )
  (setq I 0)
  (setq PT nil)
  (while (and (< I 3))
    (setq PT (APPEND PT
		     (LIST (+ (* (NTH 0 (NTH I MATRIX)) X1)
			      (* (NTH 1 (NTH I MATRIX)) Y1)
			      (* (NTH 2 (NTH I MATRIX)) Z1)
			   )
		     )
	     )
    )
    (setq I (1+ I))
  )
  PT
)
(DEFUN NBTF_SETCUSCOORD	(MODE	  /	   PT1	    PT2	     PT3
			 UCSANG	  PT0	   PT1_XY   PT1_AB   PT2_XY
			 PT2_AB	  FUN_GET2P	    MYGETN   MYGETE
			 UCSX	  UCSY	   MYSETN   MYSETE   DIMZIN_OLD
			)
  (DEFUN FUN_GET2P (/		DCL_ID	    WHAT_NEXT	DO_GETP1OLD
		    P1OLD	P1OLDN	    P1OLDE	P2OLD
		    P2OLDN	P2OLDE	    P1NEW	P1NEWN
		    P1NEWE	P2NEW	    P2NEWN	P2NEWE
		    FUN_SETINI	UCSSC	    TRUEUCSSC	DO_GETP1OLD
		    FUN_SETOK	FUN_SETPT   SCALESET	DO_SCSET
		    DDIST	DO_TRUEUCSSC		FUN_GETDIST
		    DIST0	DIST1
		   )
    (DEFUN FUN_GETDIST (P1 P2 / DIST P1X P1Y P2X P2Y LEN)
      (setq P1X	(VL-STRING-POSITION
		  (ASCII ".")
		  (VL-LIST->STRING
		    (REVERSE (VL-STRING->LIST (RTOS (CAR P1) 2 10)))
		  )
		)
      )
      (setq P1Y	(VL-STRING-POSITION
		  (ASCII ".")
		  (VL-LIST->STRING
		    (REVERSE (VL-STRING->LIST (RTOS (CADR P1) 2 10)))
		  )
		)
      )
      (setq P2X	(VL-STRING-POSITION
		  (ASCII ".")
		  (VL-LIST->STRING
		    (REVERSE (VL-STRING->LIST (RTOS (CAR P2) 2 10)))
		  )
		)
      )
      (setq P2Y	(VL-STRING-POSITION
		  (ASCII ".")
		  (VL-LIST->STRING
		    (REVERSE (VL-STRING->LIST (RTOS (CADR P2) 2 10)))
		  )
		)
      )
      (if (NULL P1X)
	(PROGN (setq P1X 0))
      )
      (if (NULL P1Y)
	(PROGN (setq P1Y 0))
      )
      (if (NULL P2X)
	(PROGN (setq P2X 0))
      )
      (if (NULL P2Y)
	(PROGN (setq P2Y 0))
      )
      (setq LEN (MAX P1X P1Y P2X P2Y))
      (setq DIST (DISTANCE P2 P1))
      (setq DIST
	     (/ (FIX (+ (* DIST (EXPT 10 LEN)) 0.5)) 1.0 (EXPT 10 LEN))
      )
      DIST
    )
    (DEFUN FUN_SETINI ()
      (if P1OLD
	(PROGN (SET_TILE "p1oldn" (RTOS (MYGETN P1OLD) 2 10))
	       (SET_TILE "p1olde" (RTOS (MYGETE P1OLD) 2 10))
	)
      )
      (if P2OLD
	(PROGN (SET_TILE "p2oldn" (RTOS (MYGETN P2OLD) 2 10))
	       (SET_TILE "p2olde" (RTOS (MYGETE P2OLD) 2 10))
	)
      )
      (if P1NEW
	(PROGN (SET_TILE "p1newn" (RTOS (MYGETN P1NEW) 2 10))
	       (SET_TILE "p1newe" (RTOS (MYGETE P1NEW) 2 10))
	)
      )
      (if P2NEW
	(PROGN (SET_TILE "p2newn" (RTOS (MYGETN P2NEW) 2 10))
	       (SET_TILE "p2newe" (RTOS (MYGETE P2NEW) 2 10))
	)
      )
      (if (AND P1OLD
	       P2OLD
	       P1NEW
	       P2NEW
	       (NOT (EQUAL P1OLD P2OLD))
	       (NOT (EQUAL P1NEW P2NEW))
	  )
	(PROGN (setq DIST0 (DISTANCE P2OLD P1OLD))
	       (setq DIST1 (FUN_GETDIST P2NEW P1NEW))
	       (setq UCSSC (/ DIST0 DIST1))
	)
      )
      (if UCSSC
	(PROGN (SET_TILE "calucssc" (RTOS UCSSC 2 10)))
      )
      (if TRUEUCSSC
	(PROGN (SET_TILE "trueucssc" (RTOS TRUEUCSSC 2 10))
	       (SET_TILE
		 "ddist"
		 (RTOS (setq DDIST (- (* TRUEUCSSC (FUN_GETDIST P2NEW P1NEW))
				      (DISTANCE P2OLD P1OLD)
				   )
		       )
		       2
		       10
		 )
	       )
	)
      )
    )
    (DEFUN FUN_SETOK ()
      (if (AND P1OLD P2OLD P1NEW P2OLD TRUEUCSSC)
	(PROGN (MODE_TILE "accept" 0))
	(PROGN (MODE_TILE "accept" 1))
      )
    )
    (DEFUN DO_GETP1OLD ()
      (if (AND (setq P1OLD
		      (GETPOINT
			(NBTF_GETLANGUAGESTR
			  '("\n 请确定已知点Pt1在源坐标系的位置:"
			    "\n Please determine the position of the known point Pt1 in the original coordinate system:"
			    "\n 請確定已知點Pt1在源坐標系的位置:"
			   )
			)
		      )
	       )
	       (setq P2OLD
		      (GETPOINT
			P1OLD
			(NBTF_GETLANGUAGESTR
			  '("\n 请确定已知点Pt2在源坐标系的位置:"
			    "\n Please determine the position of the known point Pt2 in the original coordinate system:"
			    "\n 請確定已知點Pt2在源坐標系的位置:"
			   )
			)
		      )
	       )
	  )
	(PROGN (setq P1OLDN (CADR P1OLD))
	       (setq P1OLDE (CAR P1OLD))
	       (setq P2OLDN (CADR P2OLD))
	       (setq P2OLDE (CAR P2OLD))
	)
      )
    )
    (DEFUN DO_GETP2OLD ()
      (if P1OLD
	(PROGN (setq P2OLD
		      (GETPOINT
			P1OLD
			(NBTF_GETLANGUAGESTR
			  '("\n 请确定已知点Pt2在源坐标系的位置:"
			    "\n Please determine the position of the known point Pt2 in the original coordinate system:"
			    "\n 請確定已知點Pt2在源坐標系的位置:"
			   )
			)
		      )
	       )
	       (setq P2OLDN (CADR P2OLD))
	       (setq P2OLDE (CAR P2OLD))
	)
	(PROGN (setq P2OLD
		      (GETPOINT
			(NBTF_GETLANGUAGESTR
			  '("\n 请确定已知点Pt2在源坐标系的位置:"
			    "\n Please determine the position of the known point Pt2 in the original coordinate system:"
			    "\n 請確定已知點Pt2在源坐標系的位置:"
			   )
			)
		      )
	       )
	       (setq P2OLDN (CADR P2OLD))
	       (setq P2OLDE (CAR P2OLD))
	)
      )
    )
    (DEFUN FUN_SETPT ()
      (COND
	((= $KEY "p1oldn")
	 (if (NOT
	       (EQUAL P1OLDN (READ (RTOS (MYGETN P1OLD) 2 10)) 1.0e-10)
	     )
	   (PROGN (setq P1OLD (MYSETN P1OLDN P1OLD)))
	 )
	)
	((= $KEY "p1olde")
	 (if (NOT
	       (EQUAL P1OLDE (READ (RTOS (MYGETE P1OLD) 2 10)) 1.0e-10)
	     )
	   (PROGN (setq P1OLD (MYSETE P1OLDE P1OLD)))
	 )
	)
	((= $KEY "p2oldn")
	 (if (NOT
	       (EQUAL P2OLDN (READ (RTOS (MYGETN P2OLD) 2 10)) 1.0e-10)
	     )
	   (PROGN (setq P2OLD (MYSETN P2OLDN P2OLD)))
	 )
	)
	((= $KEY "p2olde")
	 (if (NOT
	       (EQUAL P2OLDE (READ (RTOS (MYGETE P2OLD) 2 10)) 1.0e-10)
	     )
	   (PROGN (setq P2OLD (MYSETE P2OLDE P2OLD)))
	 )
	)
	((= $KEY "p1newn") (setq P1NEW (MYSETN P1NEWN P1NEW)))
	((= $KEY "p1newe") (setq P1NEW (MYSETE P1NEWE P1NEW)))
	((= $KEY "p2newn") (setq P2NEW (MYSETN P2NEWN P2NEW)))
	((= $KEY "p2newe") (setq P2NEW (MYSETE P2NEWE P2NEW)))
      )
      (FUN_SETINI)
    )
    (DEFUN DO_SCSET ()
      (if UCSSC
	(PROGN (SET_TILE "trueucssc" (RTOS (setq TRUEUCSSC UCSSC) 2 10))
	       (DO_TRUEUCSSC)
	)
      )
    )
    (DEFUN DO_TRUEUCSSC	()
      (SET_TILE	"ddist"
		(RTOS (setq DDIST (- (* TRUEUCSSC (FUN_GETDIST P2NEW P1NEW))
				     (DISTANCE P2OLD P1OLD)
				  )
		      )
		      2
		      10
		)
      )
    )
    (setq P1OLD (NBTF_GETLDATA '("NBTV_DimZB_UCS_p1old" (0 0))))
    (setq P2OLD (NBTF_GETLDATA '("NBTV_DimZB_UCS_p2old" (0 0))))
    (setq P1NEW (NBTF_GETLDATA '("NBTV_DimZB_UCS_p1new" (0 0))))
    (setq P2NEW (NBTF_GETLDATA '("NBTV_DimZB_UCS_p2new" (0 0))))
    (setq TRUEUCSSC (NBTF_GETLDATA '("NBTV_DimZB_UCS_trueucssc" 1.0)))
    (if	(< (setq DCL_ID	(LOAD_DIALOG
			  (STRCAT ""
				  (NBTF_GETLANGUAGESTR
				    '("get2pucs.DCL"
				      "get2pucs_en.DCL"
				      "get2pucs_tw.DCL"
				     )
				  )
			  )
			)
	   )
	   0
	)
      (PROGN (EXIT))
    )
    (setq WHAT_NEXT 8)
    (while (and (< 2 WHAT_NEXT))
      (if (NOT (NEW_DIALOG "get2pucs" DCL_ID))
	(PROGN (EXIT))
      )
      (FUN_SETINI)
      (ACTION_TILE
	"p1oldn"
	"(NBtf_Dcl_Verify_Data $key)(fun_setpt)"
      )
      (ACTION_TILE
	"p1olde"
	"(NBtf_Dcl_Verify_Data $key)(fun_setpt)"
      )
      (ACTION_TILE
	"p2oldn"
	"(NBtf_Dcl_Verify_Data $key)(fun_setpt)"
      )
      (ACTION_TILE
	"p2olde"
	"(NBtf_Dcl_Verify_Data $key)(fun_setpt)"
      )
      (ACTION_TILE
	"p1newn"
	"(NBtf_Dcl_Verify_Data $key)(fun_setpt)"
      )
      (ACTION_TILE
	"p1newe"
	"(NBtf_Dcl_Verify_Data $key)(fun_setpt)"
      )
      (ACTION_TILE
	"p2newn"
	"(NBtf_Dcl_Verify_Data $key)(fun_setpt)"
      )
      (ACTION_TILE
	"p2newe"
	"(NBtf_Dcl_Verify_Data $key)(fun_setpt)"
      )
      (ACTION_TILE "scaleset" "(do_scset)")
      (ACTION_TILE
	"trueucssc"
	"(NBtf_Dcl_Verify_Data $key)(do_trueucssc)"
      )
      (ACTION_TILE "getp1old" "(done_dialog 3)")
      (ACTION_TILE "getp2old" "(done_dialog 4)")
      (ACTION_TILE "getp1new" "(done_dialog 5)")
      (ACTION_TILE "getp2new" "(done_dialog 6)")
      (setq WHAT_NEXT (START_DIALOG))
      (COND ((= WHAT_NEXT 3) (DO_GETP1OLD))
	    ((= WHAT_NEXT 4) (DO_GETP2OLD))
	    ((= WHAT_NEXT 5) (DO_GETP1NEW))
	    ((= WHAT_NEXT 6) (DO_GETP2NEW))
      )
    )
    (if	(= WHAT_NEXT 1)
      (PROGN (NBTF_SETLDATA "NBTV_DimZB_UCS_p1old" P1OLD)
	     (NBTF_SETLDATA "NBTV_DimZB_UCS_p2old" P2OLD)
	     (NBTF_SETLDATA "NBTV_DimZB_UCS_p1new" P1NEW)
	     (NBTF_SETLDATA "NBTV_DimZB_UCS_p2new" P2NEW)
	     (NBTF_SETLDATA "NBTV_DimZB_UCS_trueucssc" (* 1.0 TRUEUCSSC))
	     (NBTF_SETLDATA "NBTV_DimZB_UCS_sc" (/ 1.0 TRUEUCSSC))
	     (setq UCSANG (- (ANGLE P2OLD P1OLD) (ANGLE P2NEW P1NEW)))
	     (setq P1NEW (MAPCAR '(LAMBDA (PTXY) (* PTXY TRUEUCSSC)) P1NEW))
	     (setq P2NEW (MAPCAR '(LAMBDA (PTXY) (* PTXY TRUEUCSSC)) P2NEW))
	     (setq UCSANG (if (< UCSANG 0)
			    (PROGN (+ PI PI UCSANG))
			    (PROGN UCSANG)
			  )
	     )
	     (setq PT1 (NBTF_PT_AB->XY P1OLD P1NEW UCSANG '(0 0 0)))
	     (setq PT2 (POLAR PT1 UCSANG 100000.0))
	     (setq UCSANG (ANGLE (TRANS PT1 1 0) (TRANS PT2 1 0)))
	     (NBTF_SETLDATA "NBTV_DimZB_UCS_pt0" (TRANS PT1 1 0))
	     (NBTF_SETLDATA "NBTV_DimZB_UCS_ang" UCSANG)
      )
    )
  )
  (setq DIMZIN_OLD (GETVAR "DIMZIN"))
  (SETVAR "DIMZIN" 8)
  (setq UCSX (GETVAR "ucsxdir"))
  (setq UCSY (GETVAR "ucsydir"))
  (if
    (AND
      (OR (AND (EQUAL UCSX '(1.0 0.0 0.0))
	       (EQUAL UCSY '(0.0 1.0 0.0))
	  )
	  (AND (= (CAR UCSX) (CADR UCSY)) (/= (CADR UCSX) (CAR UCSY)))
      )
    )
     (PROGN (setq NBTV_UCSCFG 0)
	    (setq MYGETN CADR)
	    (setq MYGETE CAR)
	    (setq MYSETN (lambda (PTN PT) (LIST (CAR PT) PTN)))
	    (setq MYSETE (lambda (PTE PT) (LIST PTE (CADR PT))))
     )
     (PROGN (setq NBTV_UCSCFG 1)
	    (setq MYGETN CAR)
	    (setq MYGETE CADR)
	    (setq MYSETN (lambda (PTN PT) (LIST PTN (CADR PT))))
	    (setq MYSETE (lambda (PTE PT) (LIST (CAR PT) PTE)))
     )
  )
  (NBTF_SHOWFREEINFO)
  (COND
    ((= MODE 1)
     (if (setq PT1
		(GETPOINT
		  (NBTF_GETLANGUAGESTR
		    '("\n 请确定自定义坐标系的基点坐标:"
		      "\n Please determine the base point coordinates of the custom coordinate system:"
		      "\n 請確定自定義坐標系的基點座標:"
		     )
		  )
		)
	 )
       (PROGN
	 (if (setq UCSANG
		    (GETANGLE
		      PT1
		      (NBTF_GETLANGUAGESTR
			'("\n 请确定自定义坐标系的X方向在当前坐标系中的方位角:"
			  "\n Please determine the azimuth of the X direction of the custom coordinate system in the current coordinate system:"
			  "\n 請確定自定義坐標系的X方向在當前坐標系中的方位角:"
			 )
		      )
		    )
	     )
	   (PROGN (setq PT2 (POLAR PT1 UCSANG 1000000000.0))
		  (setq UCSANG (ANGLE (TRANS PT1 1 0) (TRANS PT2 1 0)))
		  (NBTF_SETLDATA "NBTV_DimZB_UCS_pt0" (TRANS PT1 1 0))
		  (NBTF_SETLDATA "NBTV_DimZB_UCS_ang" UCSANG)
		  (NBTF_SETLDATA "NBTV_DimZB_UCS_sc" 1.0)
	   )
	 )
       )
     )
    )
    ((= MODE 2) (FUN_GET2P))
  )
  (SETVAR "DIMZIN" DIMZIN_OLD)
)


(DEFUN NBTF_SHOWFREEINFO ()
  (ALERT
    (NBTF_GETLANGUAGESTR
      (LIST
	(STRCAT
	  "[网蜂工具]提示：\n\n"
	  "    您运行的是网络免费版，你的支持是我最好的动力，请打赏!\n\n"
	  "    http://netbee.ys168.com\n\n"
	  "                      云南建投 杨明峰\n"
	  "                      QQ:14997-84427\n"
	  "                      ymf5340@163.com \n"
	 )
	(STRCAT
	  "[NetBee Tool]Prompt：\n\n"
	  "    You are running a free version of the network, your support is my best motivation, please Bonus!\n\n"
	  "    http://netbee.ys168.com\n\n"
	  "            China Yunnan Construction Investment Holding Group Co., Ltd."
	  "                        YangMingFeng\n"
	  "                      QQ:14997-84427\n"
	  "                      ymf5340@163.com \n")
	(STRCAT
	  "[網蜂工具]提示：\n\n"
	  "    您運行的是網絡免費版，你的支持是我最好的動力，請打賞!\n\n"
	  "    http://netbee.ys168.com\n\n"
	  "                      雲南建投集團 楊明峰\n"
	  "                      QQ:14997-84427\n"
	  "                      ymf5340@163.com \n"
	 )
      )
    )
  )
)


(DEFUN NBTF_GETLAYERS (/ LAY LAYS)
  (setq LAYS (CONS (CDR (ASSOC 2 (TBLNEXT "layer" T))) LAYS))
  (while (and (setq LAY (TBLNEXT "layer")))
    (setq LAYS (CONS (CDR (ASSOC 2 LAY)) LAYS))
  )
  (REVERSE LAYS)
)


(DEFUN NBTF_GETTEXTSTYLE (/ LAY LAYS)
  (setq LAYS (CONS (CDR (ASSOC 2 (TBLNEXT "Style" T))) LAYS))
  (while (and (setq LAY (TBLNEXT "Style")))
    (setq LAYS (CONS (CDR (ASSOC 2 LAY)) LAYS))
  )
  (REVERSE LAYS)
)


(DEFUN NBTF_SUBSTNTH (INDEX NEW LST / N RE ITEM)
  (setq N 0)
  (FOREACH ITEM	LST
    (if	(= N INDEX)
      (PROGN (setq RE (APPEND RE (LIST NEW))))
      (PROGN (setq RE (APPEND RE (LIST ITEM))))
    )
    (setq N (1+ N))
  )
  RE
)


(DEFUN NBTF_REGEXP (PAT STR KEY / REGEX S)
  (setq REGEX (vlax-create-object "Vbscript.RegExp"))
  (vlax-put-property REGEX "IgnoreCase" 0)
  (vlax-put-property REGEX "Global" 1)
  (setq S MTEXTSTRING)
  (vlax-put-property REGEX "Pattern" "\\\\\\\\")
  (setq S (vlax-invoke-method REGEX "Replace" S (CHR 1)))
  (vlax-put-property REGEX "Pattern" "\\\\{")
  (setq S (vlax-invoke-method REGEX "Replace" S (CHR 2)))
  (vlax-put-property REGEX "Pattern" "\\\\}")
  (setq S (vlax-invoke-method REGEX "Replace" S (CHR 3)))
  (vlax-put-property REGEX "Pattern" "\\\\pi(.[^;]*);")
  (setq S (vlax-invoke-method REGEX "Replace" S ""))
  (vlax-put-property REGEX "Pattern" "\\\\pt(.[^;]*);")
  (setq S (vlax-invoke-method REGEX "Replace" S ""))
  (vlax-put-property
    REGEX
    "Pattern"
    "\\\\S(.[^;]*)(\\^|#|\\\\)(.[^;]*);"
  )
  (setq S (vlax-invoke-method REGEX "Replace" S ""))
  (vlax-put-property
    REGEX
    "Pattern"
    "(\\\\F|\\\\f|\\\\C|\\\\H|\\\\T|\\\\Q|\\\\W|\\\\A)(.[^;]*);"
  )
  (setq S (vlax-invoke-method REGEX "Replace" S ""))
  (vlax-put-property
    REGEX
    "Pattern"
    "(\\\\L|\\\\O|\\\\l|\\\\o)"
  )
  (setq S (vlax-invoke-method REGEX "Replace" S ""))
  (vlax-put-property REGEX "Pattern" "\\\\~")
  (setq S (vlax-invoke-method REGEX "Replace" S ""))
  (vlax-put-property REGEX "Pattern" "\\\\P")
  (setq S (vlax-invoke-method REGEX "Replace" S ""))
  (vlax-put-property REGEX "Pattern" "\n")
  (setq S (vlax-invoke-method REGEX "Replace" S ""))
  (vlax-put-property REGEX "Pattern" "({|})")
  (setq S (vlax-invoke-method REGEX "Replace" S ""))
  (vlax-put-property REGEX "Pattern" "\\x01")
  (setq S (vlax-invoke-method REGEX "Replace" S "\\"))
  (vlax-put-property REGEX "Pattern" "\\x02")
  (setq S (vlax-invoke-method REGEX "Replace" S "{"))
  (vlax-put-property REGEX "Pattern" "\\x03")
  (setq S (vlax-invoke-method REGEX "Replace" S "}"))
  (vlax-release-object REGEX)
  S
)


(NBTF_GETLDATA '("NBTV_Dimzb_sc" 1.0))
(NBTF_GETLDATA '("NBTV_bzhome" "Xy"))
(NBTF_GETLDATA
  (LIST "NBTV_Dimzb_tsize" (GETVAR "textsize"))
)
(NBTF_GETLDATA '("NBTV_Dimzb_xs" 3))
(NBTF_GETLDATA '("NBTV_Dimzb_ang" 0.0))
(NBTF_GETLDATA '("NBTV_PICKSTYLE" 1))
(NBTF_GETLDATA '("NBTV_DimZB_CFG" 0))
(NBTF_GETLDATA '("NBTV_DimZB_clayer" "*默认*"))
(NBTF_GETLDATA '("NBTV_DimZB_Textstyle" "*默认*"))
(NBTF_GETLDATA '("NBTV_DimZB_pointstyle" "_None"))
(NBTF_GETLDATA
  (LIST "NBTV_DimZB_pointsize" (GETVAR "DIMASZ"))
)
(NBTF_GETLDATA '("NBTV_DimZB_UCS" 0))
(NBTF_GETLDATA '("NBTV_DimZB_UCS_pt0" (0.0 0.0 0.0)))
(NBTF_GETLDATA '("NBTV_DimZB_UCS_ang" 0.0))
(NBTF_GETLDATA '("NBTV_DimZB_UCS_sc" 1.0))
(NBTF_GETLDATA '("NBTV_DimZB_mode_LR" 1))
(NBTF_GETLDATA '("NBTV_DimZB_Num" 1))
(NBTF_GETLDATA '("NBT_DimCoord_layer" "DimCoord"))
(NBTF_GETLDATA '("NBTV_DimZB_BHHome" "ZB"))
(NBTF_GETLDATA '("NBTV_DimZB_BH" 1))
(NBTF_GETLDATA '("NBTV_DimZB_BHincrement" 1))
(NBTF_GETLDATA '("NBTV_DimZB_BHEnd" ""))
(NBTF_GETLDATA '("NBTV_dimzb_ShowBH" 0))
(NBTF_GETLDATA '("NBTV_DimZB_AutoOrie" 1))
(if (> (SUBSTR (GETVAR "ACADVER") 1 4) "16.0")
  (PROGN (NBTF_GETLDATA '("NBTV_DimZB_wipeout" 1)))
  (PROGN (NBTF_GETLDATA '("NBTV_DimZB_wipeout" 0)))
)


(vl-ACAD-defun
  (DEFUN C:NBT-DIMCOORD	(/		 TMP
			 DO_WIPEOUT	 DIMZIN_OLD
			 OSMODE_OLD	 OSMODE_OLD0
			 TEXTEVAL_OLD	 TEXTFILL_OLD
			 FUN_DRAWLDR	 TEXTQLTY_OLD
			 TEXTSIZE_OLD	 ERR_NEW
			 ERR_OLD	 OSMODE_OLD0
			 OSMODE_OLD	 PTX1
			 PTY1		 PTZ1
			 FUN_DRAWIMAGE	 FUN_GETTXTLEN
			 FUN_CREATEZB	 FUN_SETTH
			 OP1		 OP2
			 OP3		 DOWNTEMP
			 PT1		 PT2
			 PT3		 BZANG
			 UCSX		 UCSY
			 NBTV_UCSCFG	 ZBBZ_LIBERTY
			 ZBBZ_FIXUP	 SETINI_ZB
			 SETHOME	 SETCLOSE_ZB
			 ERR_NEW	 SET_XS
			 SET_UCS	 SSTR1
			 SET_OTHER	 SSTR2
			 SETTEXTH	 SETGDANG
			 DRAWTXT	 SS
			 CTSTYLE	 ORTHOMODE_OLD
			 PRG_BZPOINT	 DO_DANG
			 DO_ZB_C	 DO_NBTF_PSTYLE
			 DO_DIM_BL_SL	 FUN_DO_HELP
			 DO_GETANGLE	 DO_GET_UCS1
			 DO_GET_UCS2	 SET_ANG
			 PRG_UCS1_2	 IMG_X
			 IMG_Y		 DO_ZB_UPDATE
			 DO_PDIM	 DO_PUTFILE
			 FUN-SS-DRAG-MOVE
			 ELST		 PTDEF
			 DIMZB_LAYER	 DIMBLK_ID
			 DIMZB_TSTYLE	 DIMPOITSTYLELST
			 DO_ZB_SHOWBH	 DIMPOITSTYLE
			 DIMPOITSTYLECN	 DO_DIM_BL_SL
			 DO_DIM_BL	 DO_DIMPOINTSTYLE
			 DO_DIMZB_TSTYLE FUN_SETNUM
			 FUN_PX		 DO_NBTF_AUTOORIE
			 OLD_DIM_ORIE	 DIMPT
			 FUN_SETENT	 FUN_GETEPT
			 FUN_SETDIMZBANG DIMZB_SC
			 DIMLDRBLK_OLD	 NEWUCSPT0X
			 NEWUCSPT0Y	 NEWUCSANG
			 NEWUCSSC	 NBTF_VECTOR_TEXT
			 NBTF_GETFONTLIST
			 NBTF_LOGO	 NBTF_VECTOR_ARC
			 NBTF_FINDREPLACE
			 NBTF_DRAWFGW	 BFGW
			 FUN_GETBHSTR	 FUN_READREG
			 FUN_WRITEREG	 V_LANGUAGE
			 FUN_GETLANGUAGE DO_LANGUAGELIST
			 V_LANGUAGELIST	 V_STR_DEFAULT
			 V_STR_ACTIVE	 FUN_SHOWBONUS
			)
    (DEFUN FUN_READREG (NAME)
      (VL-REGISTRY-READ
	"HKEY_CURRENT_USER\\Software\\NBSoft\\NBDimCoord"
	NAME
      )
    )
    (DEFUN FUN_WRITEREG	(NAME VAR)
      (VL-REGISTRY-WRITE
	"HKEY_CURRENT_USER\\Software\\NBSoft\\NBDimCoord"
	NAME
	VAR
      )
    )
    (DEFUN FUN_GETLANGUAGE ()
      (COND ((= V_LANGUAGE "zh-cn") (CAR V_LANGUAGELIST))
	    ((= V_LANGUAGE "en") (CADR V_LANGUAGELIST))
	    ((= V_LANGUAGE "zh-tw") (CADDR V_LANGUAGELIST))
	    (T (CAR V_LANGUAGELIST))
      )
    )
    (DEFUN NBTF_VECTOR_ARC (IMAGEKEY  COLOR#	PTCEN#	  R#
			    STARTANG# ENDANG#	/	  I
			    PTNEW     PTOLD	PTX1LST	  PTY1LST
			    PTX2LST   PTY2LST	COLLST
			   )
      (START_IMAGE IMAGEKEY)
      (setq PTOLD (POLAR PTCEN# STARTANG# R#))
      (setq I (+ STARTANG# 0.2))
      (while (and (< I ENDANG#))
	(setq PTNEW (POLAR PTCEN# I R#))
	(setq PTX1LST (CONS (FIX (CAR PTOLD)) PTX1LST))
	(setq PTY1LST (CONS (FIX (CADR PTOLD)) PTY1LST))
	(setq PTX2LST (CONS (FIX (CAR PTNEW)) PTX2LST))
	(setq PTY2LST (CONS (FIX (CADR PTNEW)) PTY2LST))
	(setq COLLST (CONS COLOR# COLLST))
	(setq PTOLD PTNEW)
	(setq I (+ I 0.1))
      )
      (setq PTNEW (POLAR PTCEN# ENDANG# R#))
      (setq PTX1LST (CONS (FIX (CAR PTOLD)) PTX1LST))
      (setq PTY1LST (CONS (FIX (CADR PTOLD)) PTY1LST))
      (setq PTX2LST (CONS (FIX (CAR PTNEW)) PTX2LST))
      (setq PTY2LST (CONS (FIX (CADR PTNEW)) PTY2LST))
      (setq COLLST (CONS COLOR# COLLST))
      (MAPCAR 'VECTOR_IMAGE PTX1LST PTY1LST PTX2LST PTY2LST COLLST)
      (END_IMAGE)
    )
    (DEFUN NBTF_LOGO (IMAGEKEY PT0 / IMG_X IMG_Y LOGOSTR)
      (setq IMG_X (DIMX_TILE IMAGEKEY))
      (setq IMG_Y (DIMY_TILE IMAGEKEY))
      (setq LOGOSTR "By NetBee")
      (if (NULL PT0)
	(PROGN (setq PT0 (LIST (- IMG_X 5) (- IMG_Y 5))))
      )
      (NBTF_VECTOR_TEXT
	IMAGEKEY
	"R"
	(CAR PT0)
	(CADR PT0)
	0
	5
	252
	LOGOSTR
      )
      (NBTF_VECTOR_TEXT
	IMAGEKEY
	"R"
	(- (CAR PT0) 1)
	(CADR PT0)
	0
	5
	252
	LOGOSTR
      )
      (NBTF_VECTOR_TEXT
	IMAGEKEY
	"R"
	(- (CAR PT0) 1)
	(- (CADR PT0) 1)
	0
	5
	4
	LOGOSTR
      )
      (NBTF_VECTOR_TEXT
	IMAGEKEY
	"R"
	(CAR PT0)
	(- (CADR PT0) 1)
	0
	5
	4
	LOGOSTR
      )
    )
    (DEFUN NBTF_FINDREPLACE (STR$      FIND$	 REPLACE$  /
			     CNT#      FINDLEN#	 LOOP	   MID$
			     NEWSTR$   REPLACELEN#
			    )
      (setq LOOP T)
      (setq CNT# 1)
      (setq NEWSTR$ STR$)
      (setq FINDLEN# (STRLEN FIND$))
      (setq REPLACELEN# (STRLEN REPLACE$))
      (while (and LOOP)
	(setq MID$ (SUBSTR NEWSTR$ CNT# FINDLEN#))
	(if (= MID$ FIND$)
	  (PROGN (setq NEWSTR$ (STRCAT (SUBSTR NEWSTR$ 1 (1- CNT#))
				       REPLACE$
				       (SUBSTR NEWSTR$ (+ CNT# FINDLEN#))
			       )
		 )
		 (setq CNT# (+ CNT# REPLACELEN#))
	  )
	  (PROGN (setq CNT# (1+ CNT#)))
	)
	(if (= MID$ "")
	  (PROGN (setq LOOP nil))
	)
      )
      NEWSTR$
    )
    (DEFUN NBTF_GETFONTLIST
	   (STR$ / C$ CHRLIST@ FONTLIST@ NUM# PERCENTPERCENT:)
      (DEFUN PERCENTPERCENT: (STR$)
	(if (WCMATCH STR$ "*%%D*")
	  (PROGN (setq STR$ (NBTF_FINDREPLACE STR$ "%%D" (CHR 176))))
	)
	(if (WCMATCH STR$ "*%%d*")
	  (PROGN (setq STR$ (NBTF_FINDREPLACE STR$ "%%d" (CHR 176))))
	)
	(if (WCMATCH STR$ "*%%P*")
	  (PROGN (setq STR$ (NBTF_FINDREPLACE STR$ "%%P" (CHR 177))))
	)
	(if (WCMATCH STR$ "*%%p*")
	  (PROGN (setq STR$ (NBTF_FINDREPLACE STR$ "%%p" (CHR 177))))
	)
	(if (WCMATCH STR$ "*%%C*")
	  (PROGN (setq STR$ (NBTF_FINDREPLACE STR$ "%%C" (CHR 248))))
	)
	(if (WCMATCH STR$ "*%%c*")
	  (PROGN (setq STR$ (NBTF_FINDREPLACE STR$ "%%c" (CHR 248))))
	)
	(if (WCMATCH STR$ "*?")
	  (PROGN
	    (setq STR$ (NBTF_FINDREPLACE STR$ (CHR 216) (CHR 248)))
	  )
	)
	STR$
      )
      (setq STR$ (PERCENTPERCENT: STR$))
      (setq NUM# 1)
      (REPEAT (STRLEN STR$)
	(setq C$ (SUBSTR STR$ NUM# 1))
	(setq CHRLIST@
	       (COND
		 ((= C$ " ") '(3 -1 -1 -1 -1))
		 ((= C$ "!") '(3 1 7 1 1 1 9 1 9))
		 ((= C$ "\"") '(5 1 1 1 3 4 1 4 3))
		 ((= C$ "#") '(7 1 7 6 7 1 3 6 3 2 1 2 9 5 1 5 9))
		 ((= C$ "$")
		  '(6 3	1 3 10 1 8 1 8 2 9 4 9 1 3 1 4 4 6 2 5 4 2 2 2 5
		    8 5	7 5 3 5	3)
		 )
		 ((= C$ "%")
		  '(8  1  8  7	2  4  8	 4  8  1  2  1	2  2  3	 3  3
		    2  1  3  1	4  2  4	 2  5  7  6  7	5  9  6	 9  7
		    8  7  8
		   )
		 )
		 ((= C$ "&")
		  '(6  1  6  1	8  2  9	 3  9  4  8  4	7  1  2	 1  3
		    2  4  2  5	3  6  3	 6  2  1  2  1	3  2  3	 3  5
		    9  5  9  5	6  5  6
		   )
		 )
		 ((= C$ "'") '(2 1 1 1 3))
		 ((= C$ "(") '(3 1 2 1 10 2 11 2 11 2 1 2 1))
		 ((= C$ ")") '(3 2 2 2 10 1 11 1 11 1 1 1 1))
		 ((= C$ "*") '(4 1 4 3 2 1 2 3 4))
		 ((= C$ "+") '(6 1 6 5 6 3 4 3 8))
		 ((= C$ ",") '(3 1 10 2 9))
		 ((= C$ "-") '(3 1 6 2 6))
		 ((= C$ ".") '(3 1 9 1 9))
		 ((= C$ "/") '(5 1 9 4 2 4 1 4 1))
		 ((= C$ "0") '(6 1 2 1 8 2 9 4 9 2 1 4 1 5 2 5 8))
		 ((= C$ "1") '(6 3 1 3 9 1 2 2 2))
		 ((= C$ "2")
		  '(6 5 9 1 9 1 8 4 5 1 2 1 2 2 1 4 1 5 2 5 4)
		 )
		 ((= C$ "3")
		  '(6 1	8 1 8 2	9 4 9 1	2 1 2 4	5 3 5 2	1 4 1 5	8 5 6 5
		    2 5	4)
		 )
		 ((= C$ "4") '(6 5 7 1 7 4 1 4 9 3 2 1 7))
		 ((= C$ "5")
		  '(6 5	1 1 1 1	8 1 8 4	9 2 9 1	1 1 5 2	4 4 4 5	5 5 8)
		 )
		 ((= C$ "6")
		  '(6 1	8 1 2 2	9 4 9 4	5 2 5 2	1 4 1 5	8 5 6 5	2 5 2)
		 )
		 ((= C$ "7") '(6 5 1 2 8 1 1 5 1 2 9 2 9))
		 ((= C$ "8")
		  '(6 1	6 1 8 2	9 4 9 1	2 1 4 2	5 4 5 2	1 4 1 5	6 5 8 5
		    2 5	4)
		 )
		 ((= C$ "9")
		  '(6 1	8 1 8 2	9 4 9 1	2 1 4 2	5 4 5 2	1 4 1 5	2 5 8)
		 )
		 ((= C$ ":") '(3 1 9 1 9 1 4 1 4))
		 ((= C$ ";") '(3 1 10 2 9 2 4 2 4))
		 ((= C$ "<") '(6 1 6 4 9 1 6 4 3))
		 ((= C$ "=") '(6 1 7 5 7 1 5 5 5))
		 ((= C$ ">") '(6 4 6 1 9 1 3 4 6))
		 ((= C$ "?")
		  '(6 3	6 3 7 3	9 3 9 1	2 1 2 4	5 4 5 2	1 4 1 5	2 5 4)
		 )
		 ((= C$ "@")
		  '(11 4  10 8	10 4  1	 7  1  1  4  1	7  2  9	 3  9
		    2  8  2  9	4  5  4	 6  3  2  2  2	2  2  2	 3  8
		    7  10 7  5	7  6  7	 7  4  5  4  7	4  7  6	 8  2
		    9  2  9  2	9  3  10 4  10 7
		   )
		 )
		 ((= C$ "A")
		  '(7 3 1 5 6 1 7 5 7 0 9 0 8 1 6 3 1 6 8 6 9)
		 )
		 ((= C$ "B")
		  '(7 1	1 1 9 1	9 4 9 1	5 4 5 1	1 4 1 5	6 5 8 5	2 5 4)
		 )
		 ((= C$ "C")
		  '(7 2 9 5 9 2 1 5 1 1 2 1 8 6 8 6 8 6 2 6 2)
		 )
		 ((= C$ "D")
		  '(8 1	1 1 9 1	9 4 9 1	1 4 1 6	3 6 7 5	8 5 8 5	2 5 2)
		 )
		 ((= C$ "E") '(7 1 9 5 9 1 1 5 1 1 1 1 9 1 5 4 5))
		 ((= C$ "F") '(6 1 1 5 1 1 1 1 9 1 5 4 5))
		 ((= C$ "G")
		  '(8 4	5 6 5 2	1 5 1 1	2 1 8 4	9 2 9 6	5 6 9 5	8 5 8 6
		    2 6	2)
		 )
		 ((= C$ "H") '(8 1 5 6 5 1 1 1 9 6 1 6 9))
		 ((= C$ "I") '(3 1 1 1 9))
		 ((= C$ "J") '(5 3 8 3 1 1 9 2 9 0 7 0 8))
		 ((= C$ "K") '(7 2 5 6 9 5 1 2 4 1 1 1 9))
		 ((= C$ "L") '(6 1 9 5 9 1 1 1 9))
		 ((= C$ "M") '(9 4 8 6 3 1 9 1 1 4 8 2 3 7 1 7 9))
		 ((= C$ "N")
		  '(8 1 1 1 9 2 2 3 5 4 6 4 6 6 9 6 1 5 7 5 8)
		 )
		 ((= C$ "O") '(8 2 9 5 9 2 1 5 1 1 2 1 8 6 2 6 8))
		 ((= C$ "P") '(7 5 5 1 5 1 1 5 1 1 9 1 1 6 2 6 4))
		 ((= C$ "Q")
		  '(8 5	8 4 7 2	9 5 9 2	1 5 1 1	2 1 8 6	2 6 8 6	10 6 10)
		 )
		 ((= C$ "R")
		  '(8 1 1 5 1 5 5 1 5 1 9 1 1 6 6 6 9 6 2 6 4)
		 )
		 ((= C$ "S")
		  '(7 1	8 1 8 2	9 4 9 1	2 1 4 2	5 4 5 2	1 4 1 5	6 5 8 5
		    2 5	2)
		 )
		 ((= C$ "T") '(7 1 1 5 1 3 1 3 9))
		 ((= C$ "U") '(8 2 9 5 9 1 1 1 8 6 1 6 8))
		 ((= C$ "V") '(7 5 4 3 9 1 4 3 9 0 1 1 4 6 1 5 4))
		 ((= C$ "W")
		  '(11 5 4 3 9 1 4 3 9 0 1 1 4 10 1 9 4	9 4 7 9	5 4 7 9
		    5 3	5 3)
		 )
		 ((= C$ "X")
		  '(7 1	3 5 7 1	7 5 3 0	9 0 8 0	1 0 2 6	8 6 9 6	1 6 2)
		 )
		 ((= C$ "Y")
		  '(7 3 5 5 3 3 6 3 9 1 3 3 5 0 1 0 2 6 1 6 2)
		 )
		 ((= C$ "Z") '(7 0 8 6 2 0 9 6 9 0 1 6 1))
		 ((= C$ "[") '(3 1 1 1 11 2 11 2 11 2 1 2 1))
		 ((= C$ "\\") '(5 1 2 4 9 1 1 1 1))
		 ((= C$ "]") '(3 2 1 2 11 1 11 1 11 1 1 1 1))
		 ((= C$ "^") '(6 1 2 3 0 3 0 5 2))
		 ((= C$ "_") '(6 0 11 5 11))
		 ((= C$ "`") '(3 1 1 2 2))
		 ((= C$ "a")
		  '(6 2 6 5 6 5 9 2 9 1 8 1 7 4 4 2 4 5 9 5 5)
		 )
		 ((= C$ "b") '(6 1 1 1 9 1 9 4 9 1 4 4 4 5 5 5 8))
		 ((= C$ "c")
		  '(6 1 5 1 8 2 9 4 9 2 4 4 4 5 8 5 8 5 5 5 5)
		 )
		 ((= C$ "d") '(6 2 4 5 4 2 9 5 9 1 5 1 8 5 9 5 1))
		 ((= C$ "e")
		  '(6 1	6 5 6 1	5 1 8 2	9 4 9 2	4 4 4 5	8 5 8 5	5 5 5)
		 )
		 ((= C$ "f") '(3 1 2 1 9 2 4 2 4 2 1 2 1))
		 ((= C$ "g")
		  '(6 2 4 5 4 2 9 5 9 1 5 1 8 4 11 1 11 5 4 5 10)
		 )
		 ((= C$ "h") '(6 1 1 1 9 2 5 2 5 3 4 4 4 5 5 5 9))
		 ((= C$ "i") '(2 1 9 1 4 1 1 1 1))
		 ((= C$ "j") '(2 1 4 1 11 1 1 1 1))
		 ((= C$ "k") '(6 5 9 2 6 1 1 1 9 2 6 4 4))
		 ((= C$ "l") '(2 1 1 1 9))
		 ((= C$ "m")
		  '(8 1 9 1 4 4 9 4 5 3 4 2 4 7 9 7 5 5 4 6 4)
		 )
		 ((= C$ "n") '(6 1 9 1 4 2 5 2 5 3 4 4 4 5 5 5 9))
		 ((= C$ "o") '(6 1 5 1 8 2 9 4 9 2 4 4 4 5 8 5 5))
		 ((= C$ "p") '(6 1 4 1 11 1 9 4 9 1 4 4 4 5 8 5 5))
		 ((= C$ "q") '(6 5 9 2 9 5 4 2 4 1 8 1 5 5 4 5 11))
		 ((= C$ "r") '(3 1 9 1 4 2 4 2 4))
		 ((= C$ "s")
		  '(5 1 5 4 8 1 8 1 8 2 9 3 9 2 4 3 4 4 5 4 5)
		 )
		 ((= C$ "t") '(3 1 1 1 8 2 9 2 9 2 4 2 4))
		 ((= C$ "u") '(6 1 4 1 8 2 9 3 9 4 8 4 8 5 9 5 4))
		 ((= C$ "v") '(6 3 9 5 4 1 4 3 9))
		 ((= C$ "w") '(8 6 9 4 4 1 4 1 7 2 9 4 4 7 4 7 7))
		 ((= C$ "x")
		  '(5 1 4 2 7 3 6 4 9 1 9 1 8 2 7 2 7 4 4 4 5)
		 )
		 ((= C$ "y")
		  '(5 4 4 4 7 1 4 1 7 1 11 0 11 2 8 2 10 3 8 3 8)
		 )
		 ((= C$ "z") '(5 1 8 4 5 1 9 4 9 1 4 4 4))
		 ((= C$ "{")
		  '(4 2 6 2 9 3 10 3 10 1 5 1 5 2 4 2 1 3 0 3 0)
		 )
		 ((= C$ "|") '(2 1 1 1 10))
		 ((= C$ "}")
		  '(4 2 6 2 9 1 10 1 10 2 4 2 1 3 5 3 5 1 0 1 0)
		 )
		 ((= C$ "~") '(7 2 2 5 3 1 3 1 3 6 2 6 2))
		 ((= C$ (CHR 176)) '(4 1 2 2 3 2 1 3 2))
		 ((= C$ (CHR 177)) '(6 1 9 5 9 1 5 5 5 3 3 3 7))
		 ((= C$ (CHR 248))
		  '(6 3	5 5 3 1	8 3 6 4	8 3 8 1	4 1 6 3	3 2 3 5	5 5 7)
		 )
	       )
	)
	(setq FONTLIST@ (APPEND FONTLIST@ (LIST CHRLIST@)))
	(setq NUM# (1+ NUM#))
      )
      (if FONTLIST@
	(PROGN FONTLIST@)
	(PROGN (LIST '(3 -1 -1 -1 -1)))
      )
    )
    (DEFUN NBTF_VECTOR_TEXT (IMAGEKEY  JUSTIFY$	 X#	   Y#
			     ROTATION# TEXTHIGHT#	   COLOR#
			     TEXTSTR$  /	 CHRLIST@  FONTLIST@
			     TMP       ITEM	 NUM#	   WIDTH#
			     XY#       X1#	 X2#	   Y1#
			     Y2#       WID
			    )
      (START_IMAGE IMAGEKEY)
      (setq JUSTIFY$ (STRCASE JUSTIFY$))
      (if (NOT (MEMBER JUSTIFY$
		       (LIST "L" "ML" "TL" "C" "M" "TC" "R" "MR" "TR")
	       )
	  )
	(PROGN
	  (PRINC
	    "\nvector_text syntax error:\nJustification options are L, ML, TL, C, M, TC, R, MR, and TR."
	  )
	  (EXIT)
	)
      )
      (if (AND (< COLOR# 0) (> COLOR# 255))
	(PROGN
	  (PRINC
	    "\nvector_text syntax error:\nColor value ranges between 0 and 255."
	  )
	  (EXIT)
	)
      )
      (if (OR (< X# 0) (< Y# 0))
	(PROGN
	  (PRINC
	    "\nvector_text syntax error:\nValues for X and Y must be a positive number."
	  )
	  (EXIT)
	)
      )
      (if (NOT (OR (= ROTATION# 0) (= ROTATION# 90)))
	(PROGN
	  (PRINC
	    "\nvector_text syntax error:\nRotation angle can only be 0 or 90 degrees."
	  )
	  (EXIT)
	)
      )
      (setq FONTLIST@ (NBTF_GETFONTLIST TEXTSTR$))
      (FOREACH CHRLIST@	FONTLIST@
	(setq WID (FIX (* (/ (CAR CHRLIST@) 5.0) TEXTHIGHT#)))
	(setq CHRLIST@
	       (MAPCAR '(LAMBDA (X) (FIX (* (/ X 5.0) TEXTHIGHT#)))
		       (CDR CHRLIST@)
	       )
	)
	(setq TMP (CONS (CONS WID CHRLIST@) TMP))
      )
      (setq FONTLIST@ (REVERSE TMP))
      (setq WIDTH# 0)
      (if (= ROTATION# 0)
	(PROGN
	  (if (MEMBER JUSTIFY$ (LIST "TL" "TC" "TR"))
	    (PROGN (setq Y# (1- Y#)))
	  )
	  (if (MEMBER JUSTIFY$ (LIST "ML" "M" "MR"))
	    (PROGN (setq Y# (- Y# (FIX (* (/ 1 5.0) TEXTHIGHT#)))))
	  )
	  (if (MEMBER JUSTIFY$ (LIST "L" "C" "R"))
	    (PROGN (setq Y# (- Y# (FIX (* (/ 1 5.0) 9.0 TEXTHIGHT#)))))
	  )
	  (FOREACH ITEM	FONTLIST@
	    (setq WIDTH# (+ WIDTH# (NTH 0 ITEM)))
	  )
	  (if (MEMBER JUSTIFY$ (LIST "C" "M" "TC"))
	    (PROGN (setq X# (- X# (FIX (+ (/ WIDTH# 2.0) 0.5)))))
	  )
	  (if (MEMBER JUSTIFY$ (LIST "R" "MR" "TR"))
	    (PROGN (setq X# (- X# WIDTH#)))
	  )
	  (FOREACH CHRLIST@ FONTLIST@
	    (setq NUM# 1)
	    (while (and (< NUM# (LENGTH CHRLIST@)))
	      (setq XY# (NTH NUM# CHRLIST@))
	      (setq X1# (+ X# (NTH NUM# CHRLIST@)))
	      (setq Y1# (+ Y# (NTH (1+ NUM#) CHRLIST@)))
	      (setq X2# (+ X# (NTH (+ NUM# 2) CHRLIST@)))
	      (setq Y2# (+ Y# (NTH (+ NUM# 3) CHRLIST@)))
	      (if (AND (/= XY# -1)
		       (> X1# -1)
		       (> Y1# -1)
		       (> X2# -1)
		       (> Y2# -1)
		  )
		(PROGN (VECTOR_IMAGE X1# Y1# X2# Y2# COLOR#))
	      )
	      (setq NUM# (+ NUM# 4))
	    )
	    (setq X# (+ X# (NTH 0 CHRLIST@)))
	  )
	)
	(PROGN (if (MEMBER JUSTIFY$ (LIST "TL" "TC" "TR"))
		 (PROGN (setq X# (1- X#)))
	       )
	       (if (MEMBER JUSTIFY$ (LIST "ML" "M" "MR"))
		 (PROGN (setq X# (- X# 5)))
	       )
	       (if (MEMBER JUSTIFY$ (LIST "L" "C" "R"))
		 (PROGN (setq X# (- X# 9)))
	       )
	       (FOREACH	ITEM FONTLIST@
		 (setq WIDTH# (+ WIDTH# (NTH 0 ITEM)))
	       )
	       (if (MEMBER JUSTIFY$ (LIST "C" "M" "TC"))
		 (PROGN (setq Y# (+ Y# (FIX (+ (/ WIDTH# 2.0) 0.5)))))
	       )
	       (if (MEMBER JUSTIFY$ (LIST "R" "MR" "TR"))
		 (PROGN (setq Y# (+ Y# WIDTH#)))
	       )
	       (FOREACH	CHRLIST@ FONTLIST@
		 (setq NUM# 1)
		 (while	(and (< NUM# (LENGTH CHRLIST@)))
		   (setq XY# (NTH NUM# CHRLIST@))
		   (setq X1# (+ X# (NTH (1+ NUM#) CHRLIST@)))
		   (setq Y1# (- Y# (NTH NUM# CHRLIST@)))
		   (setq X2# (+ X# (NTH (+ NUM# 3) CHRLIST@)))
		   (setq Y2# (- Y# (NTH (+ NUM# 2) CHRLIST@)))
		   (if (AND (/= XY# -1)
			    (> X1# -1)
			    (> Y1# -1)
			    (> X2# -1)
			    (> Y2# -1)
		       )
		     (PROGN (VECTOR_IMAGE X1# Y1# X2# Y2# COLOR#))
		   )
		   (setq NUM# (+ NUM# 4))
		 )
		 (setq Y# (- Y# (NTH 0 CHRLIST@)))
	       )
	)
      )
      (END_IMAGE)
    )
    (DEFUN SETINI_ZB ()
      (setq V_LANGUAGELIST '("简体中文" "English" "繁體中文"))
      (if (NOT (setq V_LANGUAGE (FUN_READREG "Language")))
	(PROGN (setq V_LANGUAGE "zh-cn")
	       (FUN_WRITEREG "Language" V_LANGUAGE)
	)
      )
      (setq TEXTSTYLE_OLD (GETVAR "textstyle"))
      (setq TEXTEVAL_OLD (GETVAR "TEXTEVAL"))
      (setq TEXTFILL_OLD (GETVAR "TEXTFILL"))
      (setq TEXTQLTY_OLD (GETVAR "TEXTQLTY"))
      (setq TEXTSIZE_OLD (GETVAR "TEXTSIZE"))
      (setq DIMZIN_OLD (GETVAR "DIMZIN"))
      (setq DIMASZ_OLD (GETVAR "DIMASZ"))
      (setq OLD_LAY (GETVAR "clayer"))
      (setq DIMASZ_OLD (GETVAR "DIMASZ"))
      (setq OSMODE_OLD0 (GETVAR "OSMODE"))
      (setq DIMLDRBLK_OLD (GETVAR "DIMLDRBLK"))
      (setq UCSX (GETVAR "ucsxdir"))
      (setq UCSY (GETVAR "ucsydir"))
      (setq ERR_OLD *ERROR*)
      (setq *ERROR* ERR_NEW)
      (OR (TBLOBJNAME "style" "Dim_Coord")
	  (NBTF_SETTEXTSTYLE
	    (LIST '(3 . "txt")
		  '(41 . 0.75)
		  '(71 . 0)
		  (CONS 2 "Dim_Coord")
	    )
	  )
      )
      (setq V_STR_DEFAULT
	     (NBTF_GETLANGUAGESTR
	       '("*默认*" "*Default*" "*默認*")
	     )
      )
      (setq V_STR_ACTIVE
	     (NBTF_GETLANGUAGESTR
	       '("*当前*" "*Active*" "*當前*")
	     )
      )
      (if (AND (/= NBTV_DIMZB_TEXTSTYLE V_STR_DEFAULT)
	       (/= NBTV_DIMZB_TEXTSTYLE V_STR_ACTIVE)
	       (NULL (TBLOBJNAME "style" NBTV_DIMZB_TEXTSTYLE))
	  )
	(PROGN (NBTF_SETLDATA "NBTv_DimZB_Textstyle" V_STR_DEFAULT))
      )
      (if (AND (/= NBTV_DIMZB_CLAYER V_STR_DEFAULT)
	       (/= NBTV_DIMZB_CLAYER V_STR_ACTIVE)
	       (NULL (TBLOBJNAME "Layer" NBTV_DIMZB_CLAYER))
	  )
	(PROGN (NBTF_SETLDATA "NBTv_DimZB_clayer" V_STR_DEFAULT))
      )
      (if (NOT (TBLOBJNAME "block" "_NBT_cross"))
	(PROGN
	  (ENTMAKE '((0 . "BLOCK")
		     (8 . "0")
		     (10 0.0 0.0 0.0)
		     (2 . "_NBT_cross")
		     (70 . 0)
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
	  (ENTMAKE '((0 . "Endblk")))
	)
      )
      (setq ELST (ENTGET (TBLOBJNAME "layer" "0")))
      (setq ELST (SUBST '(70 . 0) (ASSOC 70 ELST) ELST))
      (setq ELST (SUBST	(CONS 62 (ABS (CDR (ASSOC 62 ELST))))
			(ASSOC 62 ELST)
			ELST
		 )
      )
      (ENTMOD ELST)
      (setq PTDEF '(867895.254849 2587565.18754))
      (setq DIMZB_LAYER
	     (CONS V_STR_DEFAULT
		   (CONS V_STR_ACTIVE (NBTF_GETLAYERS))
	     )
      )
      (setq DIMZB_TSTYLE
	     (CONS V_STR_DEFAULT
		   (CONS V_STR_ACTIVE (NBTF_GETTEXTSTYLE))
	     )
      )
      (setq DIMPOITSTYLELST
	     (NBTF_GETLANGUAGESTR
	       '((("None" . "_None")
		  ("Cross" . "_NBT_cross")
		  ("ClosedSolid" . ".")
		  ("ClosedFilled" . "_ClosedBlank")
		  ("Closed" . "_Closed")
		  ("Dot" . "_Dot")
		  ("ArchTick" . "_ArchTick")
		  ("Oblique" . "_Oblique")
		  ("Open" . "_Open")
		  ("Origin" . "_Origin")
		  ("Origin2" . "_Origin2")
		  ("Open90" . "_Open90")
		  ("Open30" . "_Open30")
		  ("DotSmall" . "_DotSmall")
		  ("DotBlank" . "_DotBlank")
		  ("Small" . "_Small")
		  ("BoxBlank" . "_BoxBlank")
		  ("BoxFilled" . "_BoxFilled")
		  ("DatumBlank" . "_DatumBlank")
		  ("DatumFilled" . "_DatumFilled")
		  ("Integral" . "_Integral")
		  ("systemDefault" . "_system")
		 )
		 (("None" . "_None")
		  ("Cross" . "_NBT_cross")
		  ("ClosedSolid" . ".")
		  ("ClosedFilled" . "_ClosedBlank")
		  ("Closed" . "_Closed")
		  ("Dot" . "_Dot")
		  ("ArchTick" . "_ArchTick")
		  ("Oblique" . "_Oblique")
		  ("Open" . "_Open")
		  ("Origin" . "_Origin")
		  ("Origin2" . "_Origin2")
		  ("Open90" . "_Open90")
		  ("Open30" . "_Open30")
		  ("DotSmall" . "_DotSmall")
		  ("DotBlank" . "_DotBlank")
		  ("Small" . "_Small")
		  ("BoxBlank" . "_BoxBlank")
		  ("BoxFilled" . "_BoxFilled")
		  ("DatumBlank" . "_DatumBlank")
		  ("DatumFilled" . "_DatumFilled")
		  ("Integral" . "_Integral")
		  ("systemDefault" . "_system")
		 )
		 (("無" . "_None")
		  ("十字" . "_NBT_cross")
		  ("實心閉合" . ".")
		  ("空心閉合" . "_ClosedBlank")
		  ("閉合" . "_Closed")
		  ("點" . "_Dot")
		  ("建築標記" . "_ArchTick")
		  ("傾斜" . "_Oblique")
		  ("打開" . "_Open")
		  ("原點" . "_Origin")
		  ("原點2" . "_Origin2")
		  ("打開90" . "_Open90")
		  ("打開30" . "_Open30")
		  ("小點" . "_DotSmall")
		  ("空心點" . "_DotBlank")
		  ("小" . "_Small")
		  ("空心框" . "_BoxBlank")
		  ("實心框" . "_BoxFilled")
		  ("空心基準三角形" . "_DatumBlank")
		  ("实心基準三角形" . "_DatumFilled")
		  ("積分" . "_Integral")
		  ("系統默認" . "_system")
		 )
		)
	     )
      )
      (FOREACH TMP DIMPOITSTYLELST
	(setq DIMPOITSTYLE (CONS (CDR TMP) DIMPOITSTYLE))
	(setq DIMPOITSTYLECN (CONS (CAR TMP) DIMPOITSTYLECN))
      )
      (setq DIMPOITSTYLE (REVERSE DIMPOITSTYLE))
      (setq DIMPOITSTYLECN (REVERSE DIMPOITSTYLECN))
      (if (= DIMLDRBLK_OLD "")
	(PROGN (setq DIMLDRBLK_OLD "."))
      )
      (COND ((/= NBTV_DIMZB_POINTSTYLE "_system")
	     (SETVAR "DIMLDRBLK" NBTV_DIMZB_POINTSTYLE)
	    )
      )
      (if (= (setq DIMBLK_ID (GETVAR "DIMLDRBLK")) "")
	(PROGN (setq DIMBLK_ID "0"))
	(PROGN (if (setq TMP (ASSOC DIMBLK_ID DIMPOITSTYLELST))
		 (PROGN (setq DIMBLK_ID (CDR TMP)))
	       )
	       (if (WCMATCH DIMBLK_ID "~_*")
		 (PROGN (setq DIMBLK_ID (STRCAT "_" DIMBLK_ID)))
	       )
	       (setq DIMBLK_ID
		      (CDR
			(ASSOC
			  5
			  (ENTGET
			    (CDR
			      (ASSOC
				330
				(ENTGET (TBLOBJNAME "block" DIMBLK_ID))
			      )
			    )
			  )
			)
		      )
	       )
	)
      )
      (SETVAR "DIMZIN" 0)
      (SETVAR "cmdecho" 0)
      (SETVAR "DIMASZ" NBTV_DIMZB_POINTSIZE)
      (if (= (REM (setq TMP (GETVAR "PICKSTYLE")) 2) 0)
	(PROGN (SETVAR "PICKSTYLE" (+ TMP NBTV_PICKSTYLE)))
      )
      (COND ((= NBTV_DIMZB_TEXTSTYLE V_STR_DEFAULT)
	     (setq CTSTYLE "Dim_Coord")
	    )
	    ((= NBTV_DIMZB_TEXTSTYLE V_STR_ACTIVE)
	     (setq CTSTYLE (GETVAR "Textstyle"))
	    )
	    (T (setq CTSTYLE NBTV_DIMZB_TEXTSTYLE))
      )
      (if
	(OR (AND (EQUAL UCSX '(1.0 0.0 0.0))
		 (EQUAL UCSY '(0.0 1.0 0.0))
	    )
	    (AND (= (CAR UCSX) (CADR UCSY)) (/= (CADR UCSX) (CAR UCSY)))
	)
	 (PROGN
	   (setq NBTV_UCSCFG 0)
	   (if (OR (= DIMZB_ANG (/ PI 2)) (= DIMZB_ANG (* PI (/ 3 2))))
	     (PROGN (setq DIMZB_ANG 0))
	   )
	 )
	 (PROGN	(setq NBTV_UCSCFG 1)
		(if (= DIMZB_ANG 0)
		  (PROGN (setq DIMZB_ANG (/ PI 2)))
		)
	 )
      )
      (if
	(NOT
	  (setq NBTV_DIMZB_XYTOYX (NBTF_GETLDATA "NBTv_DimZB_XYTOYX"))
	)
	 (PROGN	(if (= NBTV_UCSCFG 0)
		  (PROGN (NBTF_SETLDATA "NBTv_DimZB_XYTOYX" 1))
		  (PROGN (NBTF_SETLDATA "NBTv_DimZB_XYTOYX" 0))
		)
	 )
      )
      (if (= NBTV_DIMZB_UCS 2)
	(PROGN (setq DIMZB_SC NBTV_DIMZB_UCS_SC))
	(PROGN (setq DIMZB_SC NBTV_DIMZB_SC))
      )
      (if (<= (SUBSTR (GETVAR "ACADVER") 1 4) "16.0")
	(PROGN (NBTF_SETLDATA "NBTV_DimZB_wipeout" 0))
      )
    )
    (DEFUN SETCLOSE_ZB ()
      (SETVAR "textstyle" TEXTSTYLE_OLD)
      (SETVAR "TEXTEVAL" TEXTEVAL_OLD)
      (SETVAR "TEXTFILL" TEXTFILL_OLD)
      (SETVAR "TEXTQLTY" TEXTQLTY_OLD)
      (SETVAR "TEXTSIZE" TEXTSIZE_OLD)
      (SETVAR "DIMZIN" DIMZIN_OLD)
      (SETVAR "DIMASZ" DIMASZ_OLD)
      (SETVAR "OSMODE" OSMODE_OLD0)
      (SETVAR "clayer" OLD_LAY)
      (SETVAR "DIMASZ" DIMASZ_OLD)
      (SETVAR "DIMLDRBLK" DIMLDRBLK_OLD)
      (setq *ERROR* ERR_OLD)
      (SETVAR "cmdecho" 1)
      (PRINC)
    )
    (DEFUN ERR_NEW (MSG)
      (if DCL_ID
	(PROGN (UNLOAD_DIALOG DCL_ID))
      )
      (if ORTHOMODE_OLD
	(PROGN (SETVAR "orthomode" ORTHOMODE_OLD))
      )
      (if SS
	(PROGN (VL-CMDF "_.erase" SS ""))
      )
      (if L1
	(PROGN (VL-CMDF "_.erase" L1 ""))
      )
      (PRINC MSG)
      (SETCLOSE_ZB)
    )
    (DEFUN PRG_UCS1_2 (PT)
      (NBTF_POINTXY->AB
	NBTV_DIMZB_UCS_PT0
	NBTV_DIMZB_UCS_ANG
	1.0
	PT
      )
    )
    (DEFUN FUN-SS-DRAG-MOVE (MOVESS	PT0	   MSG
			     KEYWORD	LINETF	   /
			     E0		ROOP	   EL
			     PT1	PT2	   DRAG_OSMODE_OLD
			     DRAG_CMDECHO_OLD	   BACKVAR
			     SET_CLOSE	L1	   ENTL
			     DI		MY_VIEWSIZE
			     PLANG	DIM_ORIE
			    )
      (DEFUN SET_CLOSE ()
	(SETVAR "cmdecho" DRAG_CMDECHO_OLD)
	(SETVAR "osmode" DRAG_OSMODE_OLD)
      )
      (setq DRAG_OSMODE_OLD (GETVAR "osmode"))
      (SETVAR "osmode" 0)
      (setq DRAG_CMDECHO_OLD (GETVAR "cmdecho"))
      (SETVAR "cmdecho" 0)
      (if (AND LINETF (> LINETF 0))
	(PROGN
	  (ENTMAKE (LIST '(0 . "LEADER")
			 '(100 . "AcDbEntity")
			 '(100 . "AcDbLeader")
			 '(71 . 1)
			 '(10 0 0 0)
			 '(10 0 1 0)
			 (LIST -3
			       (LIST "ACAD"
				     '(1000 . "DSTYLE")
				     '(1002 . "{")
				     '(1070 . 41)
				     (CONS 1040 NBTV_DIMZB_POINTSIZE)
				     '(1070 . 341)
				     (CONS 1005 DIMBLK_ID)
				     '(1002 . "}")
			       )
			 )
		   )
	  )
	  (setq L1 (ENTLAST))
	  (setq ENTL (ENTGET L1))
	)
      )
      (setq ROOP T)
      (setq PT1 PT0)
      (PROMPT
	(STRCAT MSG (NBTF_GETLANGUAGESTR '("<退出>:" "<Exit>:")))
      )
      (while (and ROOP)
	(setq EL (GRREAD T 2 0))
	(COND
	  ((MEMBER EL '((2 13) (2 32)))
	   (VL-CMDF "_.erase" MOVESS L1 "")
	   (setq ROOP nil)
	  )
	  ((= (CAR EL) 2)
	   (if (OR (VL-STRING-POSITION (CADR EL) (STRCASE KEYWORD))
		   (VL-STRING-POSITION (CADR EL) (STRCASE KEYWORD T))
	       )
	     (PROGN (VL-CMDF "_.erase" MOVESS L1 "")
		    (setq BACKVAR (STRCASE (CHR (CADR EL))))
		    (setq ROOP nil)
	     )
	     (PROGN
	       (PRINC
		 (STRCAT (NBTF_GETLANGUAGESTR
			   '("\n需要选择对象或关键字:"
			     "\nYou need to select objects or keywords."
			     "\n需要選擇對象或關鍵字"
			    )
			 )
			 KEYWORD
		 )
	       )
	       (PROMPT
		 (STRCAT MSG
			 (NBTF_GETLANGUAGESTR '("<退出>:" "<Exit>:"))
		 )
	       )
	     )
	   )
	  )
	  ((= (CAR EL) 25) (setq EL nil))
	  ((= (CAR EL) 5)
	   (setq MY_VIEWSIZE (GETVAR "viewsize"))
	   (setq PT2 (CADR EL))
	   (setq DI (DISTANCE PT2 PT1))
	   (if (> (/ DI MY_VIEWSIZE) 0.005)
	     (PROGN
	       (setq PLANG (ANGLE PT2 PT0))
	       (if (= NBTV_UCSCFG 0)
		 (PROGN
		   (if (< (REM (+ (/ PI 2) NBTV_DIMZB_ANG) (* PI 2))
			  PLANG
			  (REM (+ (* PI 1.5) NBTV_DIMZB_ANG) (* PI 2))
		       )
		     (PROGN (setq DIM_ORIE T))
		     (PROGN (setq DIM_ORIE nil))
		   )
		 )
		 (PROGN
		   (if (< NBTV_DIMZB_ANG PLANG (+ PI NBTV_DIMZB_ANG))
		     (PROGN (setq DIM_ORIE nil))
		     (PROGN (setq DIM_ORIE T))
		   )
		 )
	       )
	       (if (AND	(= 1 NBTV_DIMZB_AUTOORIE)
			(/= OLD_DIM_ORIE DIM_ORIE)
		   )
		 (PROGN	(setq OLD_DIM_ORIE DIM_ORIE)
			(if DIM_ORIE
			  (PROGN (NBTF_SETLDATA "NBTV_DimZB_mode_LR" 1))
			  (PROGN (NBTF_SETLDATA "NBTV_DimZB_mode_LR" 0))
			)
			(SETVAR "orthomode" 0)
			(DRAWTXT PT0 PT2)
			(setq MOVESS SS)
			(SETVAR "orthomode" ORTHOMODE_OLD)
		 )
		 (PROGN (VL-CMDF "_.Move" MOVESS "" PT1 PT2))
	       )
	       (setq PT1 PT2)
	       (if (AND LINETF (> LINETF 0))
		 (PROGN	(setq ENTL (SUBST (CONS 10 (TRANS PT0 1 0))
					  (ASSOC 10 ENTL)
					  ENTL
				   )
			)
			(setq
			  ENTL (REVERSE	(SUBST (CONS 10 (TRANS PT2 1 0))
					       (ASSOC 10 (REVERSE ENTL))
					       (REVERSE ENTL)
					)
			       )
			)
			(ENTMOD ENTL)
		 )
	       )
	       (if (AND LINETF (= LINETF 2))
		 (PROGN (REDRAW L1 3))
	       )
	     )
	   )
	  )
	  ((= (CAR EL) 3)
	   (setq BACKVAR (CADR EL))
	   (VL-CMDF "_.Move" MOVESS "" PT1 BACKVAR)
	   (VL-CMDF "_.erase" L1 "")
	   (setq L1 nil)
	   (setq ROOP nil)
	  )
	)
      )
      (SET_CLOSE)
      BACKVAR
    )
    (DEFUN FUN_PX (LST / STRBRK X-SORT)
      (DEFUN STRBRK (STRLST / I STR ST1 N)
	(setq I 0)
	(setq STR (CAR STRLST))
	(while
	  (and (NOT (OR	(= "" (setq ST1 (SUBSTR STR (setq I (1+ I)))))
			(setq N (DISTOF ST1))
		    )
	       )
	  )
	)
	(LIST (SUBSTR STR 1 (1- I))
	      (if N
		(PROGN N)
		(PROGN 0)
	      )
	      STRLST
	)
      )
      (DEFUN X-SORT (LST DO N)
	(VL-SORT LST
		 '(LAMBDA (X Y) ((EVAL DO) (NTH N X) (NTH N Y)))
	)
      )
      (MAPCAR 'LAST
	      (X-SORT (X-SORT (MAPCAR 'STRBRK LST) '< 1) '< 0)
      )
    )
    (DEFUN FUN_SETNUM (/ TMP1 TMP2 E FUN_EDITP OLDBH NEWBH STRBRK)
      (DEFUN STRBRK (STR / I ST1 N)
	(setq I 0)
	(while
	  (and (NOT (OR	(= "" (setq ST1 (SUBSTR STR (setq I (1+ I)))))
			(setq N (DISTOF ST1))
		    )
	       )
	  )
	)
	(LIST (SUBSTR STR 1 (1- I))
	      (if N
		(PROGN (FIX N))
		(PROGN 0)
	      )
	)
      )
      (DEFUN FUN_EDITP (/ ZBSS DBH OLDNUM NEWNUM NEWSTR)
	(PRINC
	  (NBTF_GETLANGUAGESTR
	    '("\n请选择需要修改编号的坐标标注:"
	      "\nPlease select the coordinates that need to change the number."
	      "\n請選擇需要修改編號的座標標注"
	     )
	  )
	)
	(if (setq ZBSS (SSGET '((0 . "INSERT")
				(-3
				 ("NBT_OBJID")
				 ("PE_URL")
				 ("NBT_CoordLR")
				 ("NBT_Coord")
				)
			       )
		       )
	    )
	  (PROGN
	    (setq E (SSNAME ZBSS 0))
	    (setq OLDBH (CDR (ASSOC 1000 (NBTF_VXGETXDATA E "PE_URL"))))
	    (AND
	      (setq NEWBH
		     (GETSTRING
		       (STRCAT
			 (NBTF_GETLANGUAGESTR
			   '("\n 请输入新的编号(注:前缀＋数字形式。尾数必须为数字)<"
			     "\nPlease enter a new number (Note: prefix + digital form,Mantissa must be digits.)"
			     "\n請輸入新的編號（注：首碼＋數位形式，尾數必須為數字）"
			    )
			 )
			 OLDBH
			 ">:"
		       )
		     )
	      )
	      (if (WCMATCH NEWBH "*#")
		(PROGN
		  (setq OLDBH (STRBRK OLDBH))
		  (setq OLDNUM (CADR OLDBH))
		  (setq NEWBH (STRBRK NEWBH))
		  (setq NEWSTR (CAR NEWBH))
		  (setq NEWNUM (CADR NEWBH))
		  (setq DDBH (- NEWNUM OLDNUM))
		  (setq NEWBH (STRCAT NEWSTR (ITOA NEWNUM)))
		  (NBTF_VXPUTXDATA
		    E
		    (LIST '(1001 . "PE_URL") (CONS 1000 NEWBH))
		  )
		  (SSDEL E ZBSS)
		  (setq E nil)
		  (while (and (setq E (SSNAME ZBSS 0)))
		    (setq OLDBH
			   (CDR
			     (ASSOC 1000 (NBTF_VXGETXDATA E "PE_URL"))
			   )
		    )
		    (setq OLDBH (STRBRK OLDBH))
		    (setq OLDNUM (CADR OLDBH))
		    (setq NEWBH (STRCAT NEWSTR (ITOA (+ OLDNUM DDBH))))
		    (NBTF_VXPUTXDATA
		      E
		      (LIST '(1001 . "PE_URL") (CONS 1000 NEWBH))
		    )
		    (SSDEL E ZBSS)
		    (setq E nil)
		  )
		)
		(PROGN
		  (PRINC
		    (NBTF_GETLANGUAGESTR
		      '("\n错误:尾数不为数字,未能修改!"
			"\nError: the mantissa is not numeric and cannot be modified."
			"\n錯誤：尾數不為數字，未能修改！"
		       )
		    )
		  )
		)
	      )
	    )
	  )
	)
      )
      (while (and (AND (setq TMP1
			      (STRCAT
				(NBTF_GETLANGUAGESTR
				  '("\n选择需要改编号的坐标标注对象(当前编号:"
				    "\nSelect the coordinate marking object that needs the adaption number (the Active number:"
				    "\n選擇需要改編號的座標標注對象(當前編號:"
				   )
				)
				(STRCAT	NBTV_DIMZB_BHHOME
					(ITOA NBTV_DIMZB_BH)
					NBTV_DIMZB_BHEND
				)
				(NBTF_GETLANGUAGESTR
				  '(")[前缀:" ")[prefix" ")[前綴")
				)
				NBTV_DIMZB_BHHOME
				"(H)"
				(NBTF_GETLANGUAGESTR
				  '("/当前编号:" "/ActiveNumber" "/當前編號:")
				)
				(ITOA NBTV_DIMZB_BH)
				(NBTF_GETLANGUAGESTR
				  '("(N)/批量改编号(P)]"
				    "(N)/Batch change number(P)]"
				    "(N)/批量改編號(P)]"
				   )
				)
			      )
		       )
		       (setq E (NBTF_ENTSEL
				 TMP1
				 "H N P"
				 '((0 . "INSERT")
				   (-3
				    ("NBT_OBJID")
				    ("PE_URL")
				    ("NBT_CoordLR")
				    ("NBT_Coord")
				   )
				  )
			       )
		       )
		  )
	     )
	(COND
	  ((= E "H")
	   (AND	(/= (setq TMP
			   (GETSTRING
			     (STRCAT
			       (NBTF_GETLANGUAGESTR
				 '("\n 请确定坐标标注编号的前缀(*为空)<"
				   "\n Please Enter the prefix of the coordinate numbering number.(* for NULL)<"
				   "\n 请确定坐标标注编号的前缀(*为空)<"
				  )
			       )
			       NBTV_DIMZB_BHHOME
			       ">:"
			     )
			   )
		    )
		    ""
		)
		(NBTF_SETLDATA "NBTV_DimZB_BHHome" TMP)
		(= NBTV_DIMZB_BHHOME "*")
		(NBTF_SETLDATA "NBTV_DimZB_BHHome" "")
	   )
	  )
	  ((= E "N")
	   (INITGET 6)
	   (AND	(setq TMP
		       (GETINT
			 (STRCAT
			   (NBTF_GETLANGUAGESTR
			     '("\n 请输入坐标标注起始编号<"
			       "\n Please Enter the starting number of coordinates.<"
			       "\n 請輸入座標標注起始編號<"
			      )
			   )
			   (ITOA NBTV_DIMZB_BH)
			   ">:"
			 )
		       )
		)
		(NBTF_SETLDATA "NBTV_DimZB_BH" TMP)
	   )
	  )
	  ((= E "P") (FUN_EDITP))
	  ((= (TYPE E) 'LIST)
	   (setq E (CAR E))
	   (setq OLDBH (CDR (ASSOC 1000 (NBTF_VXGETXDATA E "PE_URL"))))
	   (AND
	     (setq NEWBH
		    (GETSTRING
		      (STRCAT
			(NBTF_GETLANGUAGESTR
			  '("\n 请输入新的编号(注:前缀＋数字形式。尾数必须为数字)<"
			    "\nPlease enter a new number (Note: prefix + digital form,Mantissa must be digits.)"
			    "\n請輸入新的編號(注：首碼＋數位形式，尾數必須為數字)"
			   )
			)
			OLDBH
			">:"
		      )
		    )
	     )
	     (if (WCMATCH NEWBH "*#")
	       (PROGN (NBTF_VXPUTXDATA
			E
			(LIST '(1001 . "PE_URL") (CONS 1000 NEWBH))
		      )
	       )
	       (PROGN
		 (PRINC
		   (NBTF_GETLANGUAGESTR
		     '("\n 错误:尾数不为数字,未能修改!"
		       "\n Error: the mantissa is not numeric and cannot be modified!"
		       "\n 錯誤：尾數不為數字，未能修改!"
		      )
		   )
		 )
	       )
	     )
	   )
	  )
	)
      )
    )
    (DEFUN ZBBZ_FIXUP (/ SETENT)
      (SET_ANG)
      (setq TMP
	     (STRCAT
	       (NBTF_GETLANGUAGESTR
		 '("\n {坐标系:" "\n {Coordinate system:" "\n {坐標系:")
	       )
	       (COND ((= NBTV_DIMZB_UCS 0)
		      (NBTF_GETLANGUAGESTR '("当前" "Active" "當前"))
		     )
		     ((= NBTV_DIMZB_UCS 1)
		      (NBTF_GETLANGUAGESTR '("世界" "World"))
		     )
		     ((= NBTV_DIMZB_UCS 2)
		      (NBTF_GETLANGUAGESTR '("自定义" "Custom" "自定義"))
		     )
	       )
	       (NBTF_GETLANGUAGESTR '("\\比例:" "\\Scale:"))
	       (RTOS DIMZB_SC 2 5)
	       (NBTF_GETLANGUAGESTR
		 '("\\XY互换:" "\\XYInterchange:" "\\XY互換:")
	       )
	       (if (= NBTV_DIMZB_XYTOYX 1)
		 (PROGN (NBTF_GETLANGUAGESTR '("是" "Yes")))
		 (PROGN (NBTF_GETLANGUAGESTR '("否" "No")))
	       )
	       (NBTF_GETLANGUAGESTR '("\\编号:" "\\Number:" "\\編號:"))
	       (STRCAT NBTV_DIMZB_BHHOME
		       (ITOA NBTV_DIMZB_BH)
		       NBTV_DIMZB_BHEND
	       )
	       "}["
	       (NBTF_GETLANGUAGESTR
		 '("更新(R)/批量(P)/编号(B)/选项(O)]"
		   "Update(R)/Batch(P)/Number(B)/Option(O)]"
		   "更新(R)/批量(P)/編號(B)/選項(O)]"
		  )
	       )
	       (NBTF_GETLANGUAGESTR
		 '("确定标注基点<退出>:"
		   "Coordinate marking base points<Exit>:"
		   "確定標注基點<退出>:"
		  )
	       )
	     )
      )
      (PRINC)
      (INITGET "C H O R P B")
      (if (setq PT1 (GETPOINT TMP))
	(PROGN (if (= (TYPE PT1) 'STR)
		 (PROGN	(COND ((= PT1 "C") (SETHOME))
			      ((= PT1 "H") (SETTEXTH))
			      ((= PT1 "O")
			       (SET_OTHER)
			       (if BFGW
				 (PROGN (setq PT1 nil))
			       )
			      )
			      ((= PT1 "R") (DO_ZB_UPDATE))
			      ((= PT1 "P") (DO_PDIM))
			      ((= PT1 "B") (FUN_SETNUM))
			)
		 )
		 (PROGN	(command "_.undo")
			(command "be")
			(while (and (PRG_BZPOINT)))
			(command "_.undo")
			(command "e")
		 )
	       )
	)
      )
      (while (and (if (OR SETENT (NOT PT1))
		    (PROGN nil)
		    (PROGN T)
		  )
	     )
      )
      (AND SETENT (FUN_SETENT))
    )
    (DEFUN PRG_BZPOINT (/ SS1 E1 EL)
      (setq TMP
	     (STRCAT
	       (NBTF_GETLANGUAGESTR
		 '("\n 选择标注点或[标注方位角(A)/换向(E)/更多选项(O)]<返回>:"
		   "\n Select the annotation point or[azimuth(A)/ChangeOrientation(E)/MoreOptions(O)]<返回>:"
		   "\n 選擇標注點或[標注方位角(A)/換向(E)/更多選項(O)]<返回>:"
		  )
	       )
	     )
      )
      (setq ORTHOMODE_OLD (GETVAR "orthomode"))
      (SETVAR "orthomode" 0)
      (DRAWTXT PT1 PT1)
      (INITGET "C H D A E O")
      (setq PT2 (FUN-SS-DRAG-MOVE SS PT1 TMP "A E O" 1))
      (SETVAR "orthomode" ORTHOMODE_OLD)
      (if (= (TYPE PT2) 'STR)
	(PROGN (COND ((= PT2 "A") (SETGDANG) T)
		     ((= PT2 "O")
		      (SET_OTHER)
		      (if BFGW
			(PROGN (setq PT1 nil) (setq PT2 nil))
			(PROGN T)
		      )
		     )
		     ((= PT2 "E")
		      (NBTF_SETLDATA
			"NBTV_DimZB_mode_LR"
			(BOOLE 6 NBTV_DIMZB_MODE_LR 1)
		      )
		      T
		     )
		     (T T)
	       )
	)
	(PROGN (if PT2
		 (PROGN (FUN_CREATEZB))
		 (PROGN	(if SS
			  (PROGN (VL-CMDF "_.erase" SS ""))
			)
			nil
		 )
	       )
	)
      )
    )
    (DEFUN FUN_CREATEZB
	   (/ SS1 ELST E0 E1 EBH EL BLKANG BHPT0 BHPT1 BHANGLE)
      (setq OSMODE_OLD (GETVAR "osmode"))
      (SETVAR "osmode" 0)
      (COND ((= NBTV_DIMZB_CLAYER V_STR_DEFAULT)
	     (NBTF_SETCLAYER (LIST "DimCoord" 3))
	    )
	    ((= NBTV_DIMZB_CLAYER V_STR_ACTIVE)
	     (NBTF_SETCLAYER OLD_LAY)
	    )
	    (T (NBTF_SETCLAYER NBTV_DIMZB_CLAYER))
      )
      (setq SS1 (SSADD))
      (setq E0 (SSNAME SS 0))
      (setq EBH (ENTNEXT E0))
      (setq BLKANG (NBTF_DXF 50 E0))
      (setq BHPT0 (NBTF_DXF 10 EBH))
      (setq BHPT1 (NBTF_DXF 11 EBH))
      (setq BHANGLE (NBTF_DXF 50 EBH))
      (VL-CMDF "leader" PT1 PT2 "" "" "C" E0)
      (setq E1 (ENTLAST))
      (setq EL (NBTF_DXF 330 E1))
      (setq ELST (ENTGET EL '("*")))
      (setq ELST (SUBST (CONS 8 (GETVAR "clayer")) (ASSOC 8 ELST) ELST))
      (ENTMOD ELST)
      (setq ELST (ENTGET E1))
      (setq ELST (SUBST (CONS 8 (GETVAR "clayer")) (ASSOC 8 ELST) ELST))
      (setq ELST (SUBST (CONS 50 BLKANG) (ASSOC 50 ELST) ELST))
      (if (/= NBTV_UCSCFG 0)
	(PROGN (setq ELST (SUBST (CONS 210 (NBTF_DXF 210 E0))
				 (ASSOC 210 ELST)
				 ELST
			  )
	       )
	       (setq ELST
		      (SUBST (CONS 10 (NBTF_DXF 10 E0)) (ASSOC 10 ELST) ELST)
	       )
	)
      )
      (ENTMOD ELST)
      (VL-CMDF "_.draworder" E1 "" "a" E0 "")
      (NBTF_VXADDMYXDATA
	E1
	(LIST "NBT-DimCoord"
	      (LIST "NBT_Coord" DIMPT)
	      (LIST "NBT_CoordLR" NBTV_DIMZB_MODE_LR)
	      (LIST "PE_URL" (FUN_GETBHSTR))
	)
      )
      (NBTF_SETLDATA
	"NBTV_DimZB_BH"
	(+ NBTV_DIMZB_BH NBTV_DIMZB_BHINCREMENT)
      )
      (SSADD E1 SS1)
      (SSADD EL SS1)
      (setq EBH (ENTNEXT E1))
      (setq ELST (ENTGET EBH))
      (setq ELST (SUBST (CONS 10 BHPT0) (ASSOC 10 ELST) ELST))
      (setq ELST (SUBST (CONS 11 BHPT1) (ASSOC 11 ELST) ELST))
      (setq ELST (SUBST (CONS 50 BHANGLE) (ASSOC 50 ELST) ELST))
      (setq ELST (ENTMOD ELST))
      (ENTUPD E1)
      (VL-CMDF "_.group" "" "*" "" SS1 "")
      (setq SS1 nil)
      (ENTDEL E0)
      (SETVAR "osmode" OSMODE_OLD)
      (setq SS nil)
    )
    (DEFUN SETHOME (/ TMP)
      (PRINC)
      (INITGET 6 "nO Xy Ab Ne")
      (if (setq	TMP
		 (GETKWORD
		   (STRCAT
		     (NBTF_GETLANGUAGESTR
		       '("\n 用何前缀[无(O)/xy(X)/ab(A)/ne(N)]<"
			 "\n How to use prefix[None(O)/xy(X)/ab(A)/ne(N)]<"
			 "\n 用何前綴[无(O)/xy(X)/ab(A)/ne(N)]<"
			)
		     )
		     NBTV_BZHOME
		     ">:"
		   )
		 )
	  )
	(PROGN (NBTF_SETLDATA "NBTV_bzhome" TMP))
      )
    )
    (DEFUN SET_XS (/ TMP)
      (INITGET 4)
      (if (setq	TMP (GETINT (STRCAT (NBTF_GETLANGUAGESTR
				      '("\n保留小数位<"
					"\nReserved decimal<"
					"\n保留小數位<"
				       )
				    )
				    (ITOA NBTV_DIMZB_XS)
				    ">:"
			    )
		    )
	  )
	(PROGN (NBTF_SETLDATA "NBTV_Dimzb_xs" TMP))
      )
    )
    (DEFUN DO_ZB_UPDATE	(/	 S1	 SL	 I	 E
			 E0	 E1	 E2	 E3	 EBH
			 BHPT0	 BHPT1	 BHANGLE NA	 LINEL
			 PT0	 PT1	 PT2	 TPT1	 TPT2
			 ANG1	 R1	 R2	 SSTR1	 SSTR2
			 XDLIST	 TMP	 UPALL	 PTMP	 PTX1
			 PTY1	 PTZ1	 LINEL	 ELIST	 NBTF_CROODLR
			 BZLEADER	 LST1005 DIMPSTYLE
			 UPJTALL
			)
      (SETVAR "Dimzin" 0)
      (INITGET 6 "Y N")
      (if (setq	TMP
		 (GETKWORD
		   (NBTF_GETLANGUAGESTR
		     '("\n包括检查并即时更新坐标值？[是(Y)/否(N)]<否(N)>:"
		       "\nIncluding checking and updating coordinates immediately?[Yes(Y)/No(N)]<No(N)>:"
		       "\n包括檢查並即時更新座標值？[是(Y)/否(N)]<否(N)>:"
		      )
		   )
		 )
	  )
	(PROGN (if (= TMP "Y")
		 (PROGN (setq UPALL T))
		 (PROGN (setq UPALL nil))
	       )
	)
      )
      (INITGET 6 "Y N")
      (if (setq
	    TMP	(GETKWORD
		  (NBTF_GETLANGUAGESTR
		    '("\n全部更新引线箭头？[是(Y)/否(N)]<否(N)>:"
		      "\nUpdate all lead arrows?[Yes(Y)/No(N)]<No(N)>:"
		      "\n全部更新引線箭頭？[是(Y)/否(N)]<否(N)>:"
		     )
		  )
		)
	  )
	(PROGN (if (= TMP "Y")
		 (PROGN (setq UPJTALL T))
		 (PROGN (setq UPJTALL nil))
	       )
	)
      )
      (setq I 0)
      (setq SL 0)
      (PRINC (NBTF_GETLANGUAGESTR
	       '("\n选择需要更新的坐标标注对象:"
		 "\nselect coordinates to be updated :"
		 "\n選擇需要更新的座標標注對象:"
		)
	     )
      )
      (if (setq	S1
		 (SSGET '((0 . "INSERT") (-3 ("NBT_OBJID") ("NBT_Coord"))))
	  )
	(PROGN (setq SL (SSLENGTH S1)))
      )
      (while (and (< I SL))
	(setq E (SSNAME S1 I))
	(setq I (1+ I))
	(setq NA (NBTF_DXF 2 E))
	(setq XDLIST (NBTF_VXGETMYXDATA E ""))
	(setq PT0 (CADR (ASSOC "NBT_Coord" (CDR XDLIST))))
	(setq NBTF_CROODLR (CADR (ASSOC "NBT_CoordLR" (CDR XDLIST))))
	(setq BZLEADER (NBTF_DXF 330 E))
	(setq EBH (ENTNEXT E))
	(setq BHPT0 (NBTF_DXF 10 E))
	(setq BHANGLE (NBTF_DXF 50 E))
	(if (setq E0 (TBLOBJNAME "block" NA))
	  (PROGN
	    (setq E1 (ENTNEXT E0))
	    (setq E2 (ENTNEXT E1))
	    (setq E3 (ENTNEXT E2))
	    (setq STR1 (NBTF_DXF 1 E1))
	    (setq TPT1 (NBTF_DXF 10 E1))
	    (setq STR2 (NBTF_DXF 1 E2))
	    (setq TPT2 (NBTF_DXF 11 E2))
	    (setq PT1 (NBTF_DXF 10 E3))
	    (setq PT2 (NBTF_DXF 11 E3))
	    (setq ANG1 (ANGLE PT1 PT2))
	    (COND ((= NBTV_BZHOME "Xy")
		   (setq SSTR1 "X=")
		   (setq SSTR2 "Y=")
		  )
		  ((= NBTV_BZHOME "Ab")
		   (setq SSTR1 "A=")
		   (setq SSTR2 "B=")
		  )
		  ((= NBTV_BZHOME "Ne")
		   (setq SSTR1 "N=")
		   (setq SSTR2 "E=")
		  )
		  (T (setq SSTR1 "") (setq SSTR2 ""))
	    )
	    (if	(AND UPALL (NOT (vlax-erased-p BZLEADER)))
	      (PROGN
		(setq PT0 (TRANS (NBTF_DXF 10 BZLEADER) 0 1))
		(COND ((= NBTV_DIMZB_UCS 0) (setq PTMP PT0))
		      ((= NBTV_DIMZB_UCS 1) (setq PTMP (TRANS PT0 1 0)))
		      ((= NBTV_DIMZB_UCS 2)
		       (setq PTMP (PRG_UCS1_2 (TRANS PT0 1 0)))
		      )
		)
		(NBTF_VXADDMYXDATA
		  E
		  (LIST	"NBT-DimCoord"
			(LIST "NBT_Coord" PTMP)
			(LIST "NBT_CoordLR" NBTF_CROODLR)
		  )
		)
	      )
	      (PROGN (setq PTMP PT0))
	    )
	    (setq DIMPT PTMP)
	    (setq DIMPT (MAPCAR '* DIMPT (LIST DIMZB_SC DIMZB_SC 1.0)))
	    (setq PTX1 (RTOS (* (CAR PTMP) DIMZB_SC) 2 NBTV_DIMZB_XS))
	    (setq PTY1 (RTOS (* (CADR PTMP) DIMZB_SC) 2 NBTV_DIMZB_XS))
	    (setq PTZ1 (RTOS (* (CADDR PTMP) DIMZB_SC) 2 NBTV_DIMZB_XS))
	    (if	(= NBTV_DIMZB_XYTOYX 0)
	      (PROGN (setq STRX1 (STRCAT " " SSTR1 PTX1 " "))
		     (setq STRY1 (STRCAT " " SSTR2 PTY1 " "))
	      )
	      (PROGN (setq STRX1 (STRCAT " " SSTR1 PTY1 " "))
		     (setq STRY1 (STRCAT " " SSTR2 PTX1 " "))
	      )
	    )
	    (setq LINEL	(FUN_GETTXTLEN
			  (LIST	(CONS 1 STRX1)
				(CONS 7 CTSTYLE)
				(CONS 40 NBTV_DIMZB_TSIZE)
				'(72 . 0)
				'(73 . 0)
			  )
			  (LIST	(CONS 1 STRY1)
				(CONS 7 CTSTYLE)
				(CONS 40 NBTV_DIMZB_TSIZE)
				'(72 . 0)
				'(73 . 3)
			  )
			)
	    )
	    (setq PT2 (POLAR PT1 ANG1 LINEL))
	    (if	(= NBTF_CROODLR 1)
	      (PROGN (setq TPT1 PT1))
	      (PROGN (setq TPT1 PT2)
		     (setq ANG1 (+ ANG1 PI))
		     (setq BHPT0 (POLAR BHPT0 (+ BHANGLE PI) LINEL))
	      )
	    )
	    (setq BHPT1
		   (POLAR BHPT0
			  (+ BHANGLE (/ PI 2))
			  (+ (/ NBTV_DIMZB_TSIZE 3.0) NBTV_DIMZB_TSIZE)
		   )
	    )
	    (setq TPT2 (POLAR TPT1
			      (- ANG1 (/ PI 2))
			      (/ NBTV_DIMZB_TSIZE 3.0)
		       )
	    )
	    (setq TPT1 (POLAR TPT1
			      (+ ANG1 (/ PI 2))
			      (/ NBTV_DIMZB_TSIZE 3.0)
		       )
	    )
	    (setq ELIST (ENTGET E1))
	    (setq ELIST (SUBST (CONS 1 STRX1) (ASSOC 1 ELIST) ELIST))
	    (setq ELIST (SUBST (CONS 10 TPT1) (ASSOC 10 ELIST) ELIST))
	    (setq ELIST (SUBST (CONS 7 CTSTYLE) (ASSOC 7 ELIST) ELIST))
	    (setq ELIST	(SUBST (CONS 40 NBTV_DIMZB_TSIZE)
			       (ASSOC 40 ELIST)
			       ELIST
			)
	    )
	    (ENTMOD ELIST)
	    (setq ELIST (ENTGET E2))
	    (setq ELIST (SUBST (CONS 1 STRY1) (ASSOC 1 ELIST) ELIST))
	    (setq ELIST (SUBST (CONS 10 TPT2) (ASSOC 10 ELIST) ELIST))
	    (setq ELIST (SUBST (CONS 7 CTSTYLE) (ASSOC 7 ELIST) ELIST))
	    (setq ELIST	(SUBST (CONS 40 NBTV_DIMZB_TSIZE)
			       (ASSOC 40 ELIST)
			       ELIST
			)
	    )
	    (ENTMOD ELIST)
	    (setq ELIST (ENTGET E3))
	    (setq ELIST (SUBST (CONS 11 PT2) (ASSOC 11 ELIST) ELIST))
	    (ENTMOD ELIST)
	    (setq ELIST (ENTGET EBH))
	    (setq ELIST (SUBST (CONS 10 BHPT1) (ASSOC 10 ELIST) ELIST))
	    (setq ELIST (SUBST (CONS 11 BHPT1) (ASSOC 11 ELIST) ELIST))
	    (setq ELIST	(SUBST (CONS 40 NBTV_DIMZB_TSIZE)
			       (ASSOC 40 ELIST)
			       ELIST
			)
	    )
	    (ENTMOD ELIST)
	    (ENTUPD E0)
	    (ENTUPD E)
	    (if	(AND UPJTALL (NOT (vlax-erased-p BZLEADER)))
	      (PROGN
		(if
		  (NULL (setq ELST (NBTF_VXGETXDATA BZLEADER "ACAD")))
		   (PROGN (setq	ELST '((1001 . "ACAD")
				       (1000 . "DSTYLE")
				       (1002 . "{")
				       (1002 . "}")
				      )
			  )
		   )
		)
		(if (MEMBER '(1070 . 341) ELST)
		  (PROGN (setq ELST (NBTF_LST_SUBSTNTH
				      (1+ (VL-POSITION '(1070 . 341) ELST))
				      (CONS 1005 DIMBLK_ID)
				      ELST
				    )
			 )
		  )
		  (PROGN (setq ELST (APPEND (LIST (CAR ELST)
						  (CADR ELST)
						  (CADDR ELST)
						  '(1070 . 341)
						  (CONS 1005 DIMBLK_ID)
					    )
					    (CDDDR ELST)
				    )
			 )
		  )
		)
		(if (MEMBER '(1070 . 41) ELST)
		  (PROGN (setq ELST (NBTF_LST_SUBSTNTH
				      (1+ (VL-POSITION '(1070 . 41) ELST))
				      (CONS 1040 NBTV_DIMZB_POINTSIZE)
				      ELST
				    )
			 )
		  )
		  (PROGN (setq ELST
				(APPEND	(LIST (CAR ELST)
					      (CADR ELST)
					      (CADDR ELST)
					      '(1070 . 41)
					      (CONS 1040 NBTV_DIMZB_POINTSIZE)
					)
					(CDDDR ELST)
				)
			 )
		  )
		)
		(NBTF_VXPUTXDATA BZLEADER ELST)
	      )
	    )
	  )
	)
      )
      (SETVAR "Dimzin" 8)
    )
    (DEFUN SETTEXTH (/ TMP)
      (if (setq
	    TMP	(GETREAL (STRCAT (NBTF_GETLANGUAGESTR
				   '("\n输入字高:<"
				     "\nEnter the height of the word:<"
				     "\n輸入字高:<"
				    )
				 )
				 (RTOS NBTV_DIMZB_TSIZE)
				 ">:"
			 )
		)
	  )
	(PROGN (NBTF_SETLDATA "NBTV_Dimzb_tsize" TMP))
      )
    )
    (DEFUN SETGDANG (/ TMP)
      (if (setq
	    TMP	(GETANGLE (STRCAT (NBTF_GETLANGUAGESTR
				    '("\n确定标注方向<"
				      "\nDetermine labeling direction<"
				      "\n確定標注方向<"
				     )
				  )
				  (ANGTOS NBTV_DIMZB_ANG 1 5)
				  ">:"
			  )
		)
	  )
	(PROGN (NBTF_SETLDATA "NBTV_Dimzb_ang" TMP)
	       (FUN_SETDIMZBANG)
	)
      )
    )
    (DEFUN FUN_SETDIMZBANG (/ TMP)
      (setq TMP NBTV_DIMZB_ANG)
      (setq TMP (REM TMP PI))
      (if (> TMP (/ PI 2))
	(PROGN (setq TMP (+ TMP PI)))
      )
      (NBTF_SETLDATA "NBTV_Dimzb_ang" TMP)
    )
    (DEFUN FUN_GETTXTLEN (EMT1 EMT2 / BOX1 BOX2)
      (setq BOX1 (TEXTBOX EMT1))
      (setq BOX2 (TEXTBOX EMT2))
      (* (MAX (APPLY 'DISTANCE BOX1) (APPLY 'DISTANCE BOX2)) 1.15)
    )
    (DEFUN FUN_GETBHSTR	()
      (STRCAT " "
	      NBTV_DIMZB_BHHOME
	      (ITOA NBTV_DIMZB_BH)
	      NBTV_DIMZB_BHEND
      )
    )
    (DEFUN DRAWTXT (PT0	    PT1	    /	    PTDRAW  FUN_GETPT
		    LINEL   PT1_OLD PTMP    PT2	    PT3	    PT4
		    ELST    PT5	    PTBH    UCSANG  BLKNAME LEN1
		    LEN2    SS1	    MTEXTANGLE	    TT1	    EBLK
		    EMT1    EMT2
		   )
      (DEFUN FUN_GETPT (/ DIMZB_ANG)
	(setq LINEL (FUN_GETTXTLEN
		      (LIST '(0 . "TEXT")
			    (CONS 1 STRX1)
			    (CONS 7 CTSTYLE)
			    (CONS 40 NBTV_DIMZB_TSIZE)
			    '(72 . 0)
			    '(73 . 1)
		      )
		      (LIST '(0 . "TEXT")
			    (CONS 1 STRY1)
			    (CONS 7 CTSTYLE)
			    (CONS 40 NBTV_DIMZB_TSIZE)
			    '(72 . 0)
			    '(73 . 3)
		      )
		    )
	)
	(setq DIMZB_ANG 0)
	(if (= NBTV_DIMZB_MODE_LR 1)
	  (PROGN (setq PT2 PT1)
		 (setq PT3 (POLAR PT1
				  (+ DIMZB_ANG (/ PI 2))
				  (/ NBTV_DIMZB_TSIZE 3.0)
			   )
		 )
		 (setq PT4 (POLAR PT1
				  (- DIMZB_ANG (/ PI 2))
				  (/ NBTV_DIMZB_TSIZE 3.0)
			   )
		 )
		 (setq PT5 (POLAR PT1 DIMZB_ANG LINEL))
	  )
	  (PROGN (setq PT2 (POLAR PT1 (+ DIMZB_ANG PI) LINEL))
		 (setq PT3 (POLAR PT2
				  (+ DIMZB_ANG (/ PI 2))
				  (/ NBTV_DIMZB_TSIZE 3.0)
			   )
		 )
		 (setq PT4 (POLAR PT2
				  (- DIMZB_ANG (/ PI 2))
				  (/ NBTV_DIMZB_TSIZE 3.0)
			   )
		 )
		 (setq PT5 (POLAR PT1 (+ DIMZB_ANG PI) LINEL))
	  )
	)
      )
      (if SS
	(PROGN (VL-CMDF "_.erase" SS ""))
      )
      (setq SS nil)
      (SETVAR "Dimzin" 0)
      (setq PT1_OLD PT1)
      (setq
	UCSANG (ANGLE (TRANS '(0 0) 1 0)
		      (TRANS (POLAR '(0 0) NBTV_DIMZB_ANG 100) 1 0)
	       )
      )
      (COND ((= NBTV_BZHOME "Xy") (setq SSTR1 "X=") (setq SSTR2 "Y="))
	    ((= NBTV_BZHOME "Ab") (setq SSTR1 "A=") (setq SSTR2 "B="))
	    ((= NBTV_BZHOME "Ne") (setq SSTR1 "N=") (setq SSTR2 "E="))
	    (T (setq SSTR1 "") (setq SSTR2 ""))
      )
      (setq OSMODE_OLD (GETVAR "osmode"))
      (SETVAR "osmode" 0)
      (setq SS (SSADD))
      (setq SS1 (SSADD))
      (COND ((= NBTV_DIMZB_UCS 0) (setq PTMP PT0))
	    ((= NBTV_DIMZB_UCS 1) (setq PTMP (TRANS PT0 1 0)))
	    ((= NBTV_DIMZB_UCS 2)
	     (setq PTMP (PRG_UCS1_2 (TRANS PT0 1 0)))
	    )
      )
      (setq DIMPT PTMP)
      (setq DIMPT (MAPCAR '* DIMPT (LIST DIMZB_SC DIMZB_SC 1.0)))
      (setq PTX1 (RTOS (* (CAR PTMP) DIMZB_SC) 2 NBTV_DIMZB_XS))
      (setq PTY1 (RTOS (* (CADR PTMP) DIMZB_SC) 2 NBTV_DIMZB_XS))
      (if (= NBTV_DIMZB_XYTOYX 0)
	(PROGN (setq STRX1 (STRCAT " " SSTR1 PTX1 ""))
	       (setq STRY1 (STRCAT " " SSTR2 PTY1 ""))
	)
	(PROGN (setq STRX1 (STRCAT " " SSTR1 PTY1 ""))
	       (setq STRY1 (STRCAT " " SSTR2 PTX1 ""))
	)
      )
      (setq PT1 '(0 0))
      (FUN_GETPT)
      (if (< (setq MTEXTANGLE
		    (- (ANGLE '(0 0) (GETVAR "UCSXDIR")) (/ PI 2.0))
	     )
	     0
	  )
	(PROGN (setq MTEXTANGLE (+ MTEXTANGLE PI PI)))
      )
      (setq TT1	(ENTMAKE '((0 . "BLOCK")
			   (8 . "0")
			   (10 0.0 0.0 0.0)
			   (2 . "*U")
			   (70 . 1)
			  )
		)
      )
      (setq ELST (LIST '(0 . "MTEXT")
		       '(100 . "AcDbEntity")
		       '(8 . "0")
		       '(100 . "AcDbMText")
		       (CONS 10 PT3)
		       (CONS 40 NBTV_DIMZB_TSIZE)
		       (CONS 41 0.0)
		       '(71 . 7)
		       '(72 . 5)
		       (CONS 1 STRX1)
		       (CONS 7 CTSTYLE)
		       '(210 0.0 0.0 1.0)
		       '(11 1.0 0.0 0.0)
		       '(42 . 0)
		       (CONS 50 (- NBTV_DIMZB_ANG UCSANG))
		       '(73 . 1)
		       '(44 . 1.0)
		 )
      )
      (if (= NBTV_DIMZB_WIPEOUT 1)
	(PROGN
	  (setq
	    ELST (APPEND ELST
			 '((90 . 3) (63 . 256) (45 . 1.15) (441 . 0))
		 )
	  )
	)
      )
      (setq EOBJ (ENTMAKE ELST))
      (setq ELST (LIST '(0 . "MTEXT")
		       '(100 . "AcDbEntity")
		       '(8 . "0")
		       '(100 . "AcDbMText")
		       (CONS 10 PT4)
		       (CONS 40 NBTV_DIMZB_TSIZE)
		       (CONS 41 0.0)
		       '(71 . 1)
		       '(72 . 5)
		       (CONS 1 STRY1)
		       (CONS 7 CTSTYLE)
		       '(210 0.0 0.0 1.0)
		       '(11 1.0 0.0 0.0)
		       '(42 . 0)
		       (CONS 50 (- NBTV_DIMZB_ANG UCSANG))
		       '(73 . 1)
		       '(44 . 1.0)
		 )
      )
      (if (= NBTV_DIMZB_WIPEOUT 1)
	(PROGN
	  (setq
	    ELST (APPEND ELST
			 '((90 . 3) (63 . 256) (45 . 1.15) (441 . 0))
		 )
	  )
	)
      )
      (ENTMAKE ELST)
      (setq ELST (LIST '(0 . "LINE")
		       '(8 . "0")
		       '(62 . 0)
		       (CONS 10 PT1)
		       (CONS 11 PT5)
		 )
      )
      (ENTMAKE ELST)
      (setq BLKNAME (ENTMAKE '((0 . "ENDBLK"))))
      (if BLKNAME
	(PROGN
	  (ENTMAKE (LIST '(0 . "INSERT")
			 '(8 . "0")
			 '(66 . 1)
			 (CONS 2 BLKNAME)
			 (CONS 10 (TRANS PT1_OLD 1 0))
			 (CONS 50 UCSANG)
		   )
	  )
	  (if (= NBTV_DIMZB_MODE_LR 1)
	    (PROGN (setq
		     PTBH (TRANS
			    (POLAR
			      PT1_OLD
			      (+ NBTV_DIMZB_ANG (/ PI 2))
			      (+ (/ NBTV_DIMZB_TSIZE 3.0) NBTV_DIMZB_TSIZE)
			    )
			    1
			    0
			  )
		   )
	    )
	    (PROGN (setq
		     PTBH (TRANS
			    (POLAR
			      (POLAR PT1_OLD (+ NBTV_DIMZB_ANG PI) LINEL)
			      (+ NBTV_DIMZB_ANG (/ PI 2))
			      (+ (/ NBTV_DIMZB_TSIZE 3.0) NBTV_DIMZB_TSIZE)
			    )
			    1
			    0
			  )
		   )
	    )
	  )
	  (setq
	    ELST (APPEND '((0 . "ATTRIB")
			   (100 . "AcDbEntity")
			   (410 . "Model")
			   (8 . "0")
			  )
			 (LIST '(100 . "AcDbText")
			       (CONS 10 PTBH)
			       (CONS 40 NBTV_DIMZB_TSIZE)
			       (CONS 1 "Free")
			       (CONS 50 UCSANG)
			       (CONS 41 (NBTF_GETTEXTSTYLEWS CTSTYLE))
			       '(51 . 0.0)
			       (CONS 7 CTSTYLE)
			       '(71 . 0)
			       '(72 . 0)
			       (CONS 11 PTBH)
			       '(210 0.0 0.0 1.0)
			       '(100 . "AcDbAttribute")
			       '(2 . "Number")
			       (CONS 70
				     (if (> NBTV_DIMZB_SHOWBH 0)
				       (PROGN 2)
				       (PROGN 1)
				     )
			       )
			       '(73 . 0)
			       '(74 . 1)
			 )
		 )
	  )
	  (ENTMAKE ELST)
	  (ENTMAKE '((0 . "SEQEND")))
	  (SSADD (ENTLAST) SS)
	)
	(PROGN (PRINC (NBTF_GETLANGUAGESTR
			'("\n创建坐标失败!"
			  "\nFailed to create coordinates!"
			  "\n創建座標失敗!"
			 )
		      )
	       )
	       (PRINC "")
	)
      )
      (SETVAR "osmode" OSMODE_OLD)
      (SETVAR "Dimzin" 8)
      (NBTF_SS_REDRAW SS 2)
      SS
    )
    (DEFUN DO_PDIM (/	    SS1	    S1	    TMP	    KEY	    ODIST
		    FILTER  I	    SL	    E	    TY	    PT1
		    PT2	    ANG1    PTLIST  DRAWT   MODE_LR_OLD
		    DIMXX   ODIST
		   )
      (DEFUN DRAWT ()
	(setq OSMODE_OLD (GETVAR "osmode"))
	(SETVAR "osmode" 0)
	(DRAWTXT PT1 PT1)
	(VL-CMDF "_.move" SS "" PT1 PT2)
	(FUN_CREATEZB)
      )
      (setq MODE_LR_OLD NBTV_DIMZB_MODE_LR)
      (setq KEY "C")
      (INITGET 6 "S B C 1 2 3 4 P L")
      (if (setq	TMP
		 (GETKWORD
		   (NBTF_GETLANGUAGESTR
		     '("\n 选择批量标注的对象条件[基础桩(S)/块(B)/圆(C)/点(P)/直线起点(1)/直线终点(2)/直线两点(3)/直线中点(4)/多段线全点(L)]<圆>:"
		       "\n choose batch label object conditions [base pile (S)/Block(B)/Circle(C)/Point(P)/linestartPoint(1)/lineEndPoint(2)/lineTwoPoint(3)/linemidpoint (4)/polylineallpoints(L)]<circle>:"
		       "\n 選擇批量標注的對象條件[基础桩(S)/塊(B)/圓(C)/點(P)/直線起點(1)/直線終點(2)/直線兩點(3)/直線中點(4)/多段線全點(L)]<圓>:"
		      )
		   )
		 )
	  )
	(PROGN (setq KEY TMP))
      )
      (COND
	((= KEY "S")
	 (setq FILTER '((0 . "INSERT")
			(-4 . "<OR")
			(2 . "_YZ_K")
			(2 . "_YZ_S")
			(2 . "_FZ_K")
			(2 . "_FZ_S")
			(-4 . "OR>")
		       )
	 )
	)
	((= KEY "B")
	 (PRINC	(NBTF_GETLANGUAGESTR
		  '("\n选择样板图块:"
		    "\nSelect template Block:"
		    "\n選擇樣板圖塊:"
		   )
		)
	 )
	 (setq TMP (SSGET '((0 . "INSERT"))))
	 (if TMP
	   (PROGN (setq	FILTER (LIST '(0 . "INSERT")
				     (CONS 2 (NBTF_DXF 2 (SSNAME TMP 0)))
			       )
		  )
	   )
	 )
	)
	((= KEY "C") (setq FILTER '((0 . "CIRCLE"))))
	((OR (= KEY "1") (= KEY "2") (= KEY "3") (= KEY "4"))
	 (setq FILTER '((0 . "LINE")))
	)
	((= KEY "P") (setq FILTER '((0 . "POINT"))))
	((= KEY "L") (setq FILTER '((0 . "LWPOLYLINE"))))
      )
      (setq ODIST (* NBTV_DIMZB_TSIZE 8.0))
      (if (setq	TMP (GETDIST (STRCAT "\n 选择批量标注偏移距离<"
				     (RTOS ODIST 2 5)
				     ">:"
			     )
		    )
	  )
	(PROGN (setq ODIST TMP))
      )
      (setq DIMXX 2)
      (INITGET 6 "1 2 3 4")
      (if (setq	TMP
		 (GETINT
		   "\n 选择批量标注所在位置[左上(1)/右上(2)/右下(3)/左下(4)]<右上>:"
		 )
	  )
	(PROGN (setq DIMXX TMP))
      )
      (COND ((= DIMXX 1) (setq ANG1 (* PI 0.75)))
	    ((= DIMXX 2) (setq ANG1 (* PI 0.25)))
	    ((= DIMXX 3) (setq ANG1 (* PI 1.75)))
	    ((= DIMXX 4) (setq ANG1 (* PI 1.25)))
      )
      (if (< 1 DIMXX 4)
	(PROGN (setq NBTV_DIMZB_MODE_LR 1))
	(PROGN (setq NBTV_DIMZB_MODE_LR 0))
      )
      (setq I 0)
      (setq SL 0)
      (PRINC "\n 请选择需要进行坐标标注的对象:")
      (if (setq S1 (SSGET FILTER))
	(PROGN (setq SL (SSLENGTH S1)))
      )
      (while (and (< I SL))
	(setq E (SSNAME S1 I))
	(setq I (1+ I))
	(setq TY (NBTF_DXF 0 E))
	(setq SS1 (SSADD))
	(COND
	  ((OR (= KEY "S") (= KEY "B"))
	   (setq PT1 (TRANS (NBTF_DXF 10 E) 0 1))
	   (setq PT2 (POLAR PT1 ANG1 (* ODIST (EXPT 0.5))))
	   (DRAWT)
	  )
	  ((OR (= KEY "P") (= KEY "C"))
	   (setq PT1 (TRANS (NBTF_DXF 10 E) 0 1))
	   (setq PT2 (POLAR PT1 ANG1 (* ODIST (EXPT 0.5))))
	   (DRAWT)
	  )
	  ((AND (= KEY "1"))
	   (setq PT1 (TRANS (NBTF_DXF 10 E) 0 1))
	   (setq PT2 (POLAR PT1 ANG1 (* ODIST (EXPT 0.5))))
	   (DRAWT)
	  )
	  ((AND (= KEY "2"))
	   (setq PT1 (TRANS (NBTF_DXF 11 E) 0 1))
	   (setq PT2 (POLAR PT1 ANG1 (* ODIST (EXPT 0.5))))
	   (DRAWT)
	  )
	  ((AND (= KEY "3"))
	   (setq PT1 (TRANS (NBTF_DXF 10 E) 0 1))
	   (setq PT2 (POLAR PT1 ANG1 (* ODIST (EXPT 0.5))))
	   (DRAWT)
	   (setq PT1 (TRANS (NBTF_DXF 11 E) 0 1))
	   (setq PT2 (POLAR PT1 ANG1 (* ODIST (EXPT 0.5))))
	   (DRAWT)
	  )
	  ((AND (= KEY "4"))
	   (setq PT1 (NBTF_GETLINEMID (NBTF_DXF 10 E) (NBTF_DXF 11 E)))
	   (setq PT1 (TRANS PT1 0 1))
	   (setq PT2 (POLAR PT1 ANG1 (* ODIST (EXPT 0.5))))
	   (DRAWT)
	  )
	  ((= KEY "L")
	   (setq PTLIST (NBTF_GETPLPTL E))
	   (FOREACH PT1	PTLIST
	     (setq PT1 (TRANS PT1 0 1))
	     (setq PT2 (POLAR PT1 ANG1 (* ODIST (EXPT 0.5))))
	     (DRAWT)
	   )
	  )
	)
	(setq NBTV_DIMZB_MODE_LR MODE_LR_OLD)
      )
    )
    (DEFUN DO_PUTFILE (/       FUN_WRFILE      FILENAME	       FT_ID
		       FD_ID   SSS     SL      NA      ZBDATLST
		       ZBTXTLST	       E       I       E0      E1
		       E2      NUM     PT1     XDLIST
		      )
      (DEFUN FUN_WRFILE	(ZBLST / I)
	(PRINC
	  (STRCAT "图名:" (VL-FILENAME-BASE (GETVAR "DWGNAME")) "\n")
	  FT_ID
	)
	(PRINC "序号\tX\t\tY\t\tZ\n" FT_ID)
	(setq I 1)
	(FOREACH TMP ZBLST
	  (PRINC (VL-STRING-TRIM " " (CAR TMP)) FT_ID)
	  (PRINC "\t" FT_ID)
	  (PRINC (RTOS (CADR (CADR TMP)) 2 3) FT_ID)
	  (PRINC "\t" FT_ID)
	  (PRINC (RTOS (CAR (CADR TMP)) 2 3) FT_ID)
	  (PRINC "\t" FT_ID)
	  (PRINC (RTOS (CADDR (CADR TMP)) 2 3) FT_ID)
	  (PRINC "\n" FT_ID)
	  (PRINC (STRCAT (VL-STRING-TRIM " " (CAR TMP))
			 ",,"
			 (RTOS (CAR (CADR TMP)) 2 3)
			 ","
			 (RTOS (CADR (CADR TMP)) 2 3)
			 ","
			 (RTOS (CADDR (CADR TMP)) 2 3)
			 "\n"
		 )
		 FD_ID
	  )
	  (setq I (1+ I))
	)
      )
      (setq TMP	(STRCAT	(GETVAR "DWGPREFIX")
			(VL-FILENAME-BASE (GETVAR "DWGNAME"))
		)
      )
      (if (AND (setq
		 FILENAME (GETFILED
			    (NBTF_GETLANGUAGESTR
			      '("选择生成的坐标文件名"
				"Select the generated coordinate file name"
				"選擇生成的座標檔名"
			       )
			    )
			    TMP
			    "txt"
			    1
			  )
	       )
	       (setq FT_ID (OPEN FILENAME "w"))
	       (setq FD_ID (OPEN (STRCAT FILENAME ".dat") "w"))
	  )
	(PROGN
	  (if (setq
		SSS (SSGET
		      '((0 . "INSERT") (-3 ("NBT_OBJID") ("NBT_Coord")))
		    )
	      )
	    (PROGN (setq SL (SSLENGTH SSS)))
	  )
	  (setq I 0)
	  (while (and (< I SL))
	    (setq E (SSNAME SSS I))
	    (setq I (1+ I))
	    (setq NA (NBTF_DXF 2 E))
	    (setq NUM (CDR (ASSOC 1000 (NBTF_VXGETXDATA E "PE_URL"))))
	    (setq XDLIST (NBTF_VXGETMYXDATA E ""))
	    (setq PT1 (CADR (ASSOC "NBT_Coord" (CDR XDLIST))))
	    (setq ZBTXTLST (APPEND ZBTXTLST (LIST (LIST NUM PT1))))
	  )
	  (setq ZBTXTLST (FUN_PX ZBTXTLST))
	  (FUN_WRFILE ZBTXTLST)
	  (CLOSE FT_ID)
	  (CLOSE FD_ID)
	  (startapp "notepad" FILENAME)
	  (startapp "notepad" (STRCAT FILENAME ".dat"))
	)
	(PROGN (ALERT (NBTF_GETLANGUAGESTR
			'("文件生成失败！"
			  "File generation failed"
			  "檔案生成失敗！"
			 )
		      )
	       )
	)
      )
    )
    (DEFUN DO_WIPEOUT ()
      (if (> (SUBSTR (GETVAR "ACADVER") 1 4) "16.0")
	(PROGN (NBTF_SETLDATA "NBTV_DimZB_wipeout" (READ $VALUE)))
	(PROGN
	  (ALERT
	    (NBTF_GETLANGUAGESTR
	      '("此功能仅支持AutoCAD 2005及以上版本!"
		"This feature only supports AutoCAD 2005 and above!"
		"此功能僅支持AutoCAD 2005及以上版本!"
	       )
	    )
	  )
	)
      )
    )
    (DEFUN FUN_SHOWBONUS
	   (IMAGEKEY / VLST_IMAGES VLST_IMAGE CHKNUM CHKME)
      (setq VLST_IMAGES (LOAD "bonus_128"))
      (setq VLST_IMAGE (CDR (ASSOC "bonus" VLST_IMAGES)))
      (setq CHKNUM 0)
      (setq CHKME 981178)
      (FOREACH PT VLST_IMAGE
	(setq CHKNUM (+ CHKNUM (APPLY '+ PT)))
      )
      (if (/= CHKNUM CHKME)
	(PROGN (ALERT (NBTF_GETLANGUAGESTR
			'("版本信息被非法修改!"
			  "Version information was illegally modified!"
			  "版本資訊被非法修改!"
			 )
		      )
	       )
	       (DONE_DIALOG 0)
	)
      )
      (START_IMAGE IMAGEKEY)
      (FILL_IMAGE
	0
	0
	(DIMX_TILE IMAGEKEY)
	(DIMY_TILE IMAGEKEY)
	-15
      )
      (FOREACH PT VLST_IMAGE
	(setq X (CAR PT))
	(setq Y (CADR PT))
	(setq LEN (CADDR PT))
	(setq INDEXCOLOR (CADDDR PT))
	(FILL_IMAGE X Y 1 LEN INDEXCOLOR)
      )
      (END_IMAGE)
    )
    (DEFUN SET_OTHER (/ DCL_ID DCLFN LANGINDEX)
      (setq WHAT_NEXT 8)
      (while (and (< 2 WHAT_NEXT))
	(setq DCLFN (COND ((= V_LANGUAGE "en") "zbbz_en.dcl")
			  ((= V_LANGUAGE "zh-tw") "zbbz_tw.dcl")
			  (T "zbbz.dcl")
		    )
	)
	(if (NOT
	      (NEW_DIALOG "SetDimZB" (setq DCL_ID (LOAD_DIALOG DCLFN)))
	    )
	  (PROGN (EXIT))
	)
	(setq BFGW nil)
	(SET_TILE
	  "myinfo"
	  (NBTF_GETLANGUAGESTR
	    '("请用微信扫描二维码打赏。技术支持 QQ:14997-84427 QQ群:5821780 email:ymf5340@163.com"
	      "Please use WeChat scan qr code to bonus. Support QQ:14997-84427 QQGroup:5821780 email:ymf5340@163.com"
	      "請用微信掃描二維碼打賞。技術支持 QQ:14997-84427 QQ群:5821780 email:ymf5340@163.com"
	     )
	  )
	)
	(FUN_DRAWIMAGE)
	(FUN_SHOWBONUS "ImageBonus")
	(NBTF_DCL_UPDATE_LIST "dimzb_layer" DIMZB_LAYER 3)
	(SET_TILE "dimzb_layer"
		  (ITOA (VL-POSITION NBTV_DIMZB_CLAYER DIMZB_LAYER))
	)
	(NBTF_DCL_UPDATE_LIST "dimzb_tstyle" DIMZB_TSTYLE 3)
	(SET_TILE "dimzb_tstyle"
		  (ITOA (VL-POSITION NBTV_DIMZB_TEXTSTYLE DIMZB_TSTYLE))
	)
	(NBTF_DCL_UPDATE_LIST "dimpoitstyle" DIMPOITSTYLECN 3)
	(SET_TILE "V_languagelist"
		  (ITOA (VL-POSITION (FUN_GETLANGUAGE) V_LANGUAGELIST))
	)
	(SET_TILE "dimpoitstyle"
		  (ITOA (VL-POSITION NBTV_DIMZB_POINTSTYLE DIMPOITSTYLE))
	)
	(SET_TILE "ucs_N" (RTOS (CADR NBTV_DIMZB_UCS_PT0) 2 15))
	(SET_TILE "ucs_E" (RTOS (CAR NBTV_DIMZB_UCS_PT0) 2 15))
	(SET_TILE "ucs_a" (ANGTOS NBTV_DIMZB_UCS_ANG 1 8))
	(setq NEWUCSPT0Y (CADR NBTV_DIMZB_UCS_PT0))
	(setq NEWUCSPT0X (CAR NBTV_DIMZB_UCS_PT0))
	(setq NEWUCSANG NBTV_DIMZB_UCS_ANG)
	(SET_UCS)
	(SET_TILE "dim_bl" (RTOS DIMZB_SC 2 8))
	(SET_TILE "dimzb_jd" (ITOA NBTV_DIMZB_XS))
	(SET_TILE "ZB_th" (RTOS NBTV_DIMZB_TSIZE 2 8))
	(SET_TILE "dimzb_an" (ANGTOS NBTV_DIMZB_ANG 1 5))
	(SET_TILE "nb_pstyle" (ITOA NBTV_PICKSTYLE))
	(SET_TILE "AutoOrientation" (ITOA NBTV_DIMZB_AUTOORIE))
	(SET_TILE "yx_T" (ITOA NBTV_DIMZB_XYTOYX))
	(SET_TILE "nb_wipeout" (ITOA NBTV_DIMZB_WIPEOUT))
	(SET_TILE "dimpsize" (RTOS NBTV_DIMZB_POINTSIZE 2 8))
	(SET_TILE "Dimzb_BHPrev" NBTV_DIMZB_BHHOME)
	(SET_TILE "Dimzb_ActBH" (ITOA NBTV_DIMZB_BH))
	(SET_TILE "dimzb_addbh" (ITOA NBTV_DIMZB_BHINCREMENT))
	(COND ((= NBTV_DIMZB_SHOWBH 0) (SET_TILE "nobh" "1"))
	      ((= NBTV_DIMZB_SHOWBH 1) (SET_TILE "bhtop" "1"))
	      ((= NBTV_DIMZB_SHOWBH 2) (SET_TILE "bhline" "1"))
	      (T (SET_TILE "nobh" "1"))
	)
	(SET_TILE
	  "Dim_bL_Sl"
	  (ITOA (READ (LAST (NBTF_STRPARSE (RTOS DIMZB_SC 1 4) "E"))))
	)
	(COND ((= NBTV_BZHOME "Xy") (SET_TILE "ZB_C_XY" "1"))
	      ((= NBTV_BZHOME "Ab") (SET_TILE "ZB_C_AB" "1"))
	      ((= NBTV_BZHOME "Ne") (SET_TILE "ZB_C_NE" "1"))
	      ((= NBTV_BZHOME "nO") (SET_TILE "ZB_C_NO" "1"))
	)
	(ACTION_TILE "dim_bl" "(do_dim_bl)")
	(ACTION_TILE
	  "dimzb_layer"
	  "(NBtf_SetLdata \"NBTV_DimZB_clayer\"(NBTF_Dcl_list_get $key))"
	)
	(ACTION_TILE "dimpoitstyle" "(do_dimpointstyle)")
	(ACTION_TILE
	  "dimpsize"
	  "(NBtf_Dcl_Verify_Data \"dimpsize\")(NBTF_SetLdata \"NBTV_DimZB_pointsize\" (read $value))(setvar \"DIMASZ\" NBTV_DimZB_pointsize)"
	)
	(ACTION_TILE "dimzb_tstyle" "(Do_dimzb_tstyle)")
	(ACTION_TILE
	  "dimzb_jd"
	  "(NBTF_SetLdata \"NBTV_Dimzb_xs\" (read $value))(Fun_DrawImage)"
	)
	(ACTION_TILE "ZB_th" "(fun_setth)")
	(ACTION_TILE "dimzb_an" "(DO_dang)")
	(ACTION_TILE "ZB_C_XY" "(Do_ZB_C)")
	(ACTION_TILE "ZB_C_AB" "(Do_ZB_C)")
	(ACTION_TILE "ZB_C_NE" "(Do_ZB_C)")
	(ACTION_TILE "ZB_C_NO" "(Do_ZB_C)")
	(ACTION_TILE "nobh" "(Do_ZB_ShowBH)")
	(ACTION_TILE "bhtop" "(Do_ZB_ShowBH)")
	(ACTION_TILE "bhline" "(Do_ZB_ShowBH)")
	(ACTION_TILE
	  "Dimzb_BHPrev"
	  "(NBTF_SetLdata \"NBTV_DimZB_BHHome\" $value)"
	)
	(ACTION_TILE
	  "Dimzb_ActBH"
	  "(NBTF_SetLdata \"NBTV_DimZB_BH\" (atoi $value))"
	)
	(ACTION_TILE
	  "dimzb_addbh"
	  "(NBTF_SetLdata \"NBTV_DimZB_BHincrement\" (atoi $value))"
	)
	(ACTION_TILE "nb_pstyle" "(Do_NBTF_PSTYLE)")
	(ACTION_TILE "AutoOrientation" "(Do_NBTF_AutoOrie)")
	(ACTION_TILE "Dim_bL_Sl" "(DO_Dim_bL_Sl)")
	(ACTION_TILE "ucs_N" "(setq newucspt0y (atof $value))")
	(ACTION_TILE "ucs_E" "(setq newucspt0x (atof $value))")
	(ACTION_TILE "ucs_a" "(setq newucsang (angtof  $value 1))")
	(ACTION_TILE
	  "yx_T"
	  "(NBTF_SetLdata \"NBTV_DimZB_XYTOYX\" (read $value))(Fun_DrawImage)"
	)
	(ACTION_TILE "nb_wipeout" "(Do_wipeout)")
	(ACTION_TILE
	  "zb_ucs_act"
	  "(NBTF_SetLdata \"NBTV_DimZB_UCS\" 0)(set_ucs)"
	)
	(ACTION_TILE
	  "ZB_ucs_word"
	  "(NBTF_SetLdata \"NBTV_DimZB_UCS\" 1)(set_ucs)"
	)
	(ACTION_TILE
	  "ZB_ucs_cus"
	  "(NBTF_SetLdata \"NBTV_DimZB_UCS\" 2)(set_ucs)"
	)
	(ACTION_TILE "get_angle" "(done_dialog 3)")
	(ACTION_TILE "get_ucs1" "(done_dialog 4)")
	(ACTION_TILE "get_ucs2" "(done_dialog 5)")
	(ACTION_TILE "creatfile" "(done_dialog 6)")
	(ACTION_TILE
	  "V_languagelist"
	  "(setq Langindex(NBTF_Dcl_list_get $key))(done_dialog 7)"
	)
	(ACTION_TILE "help" "(fun_do_help)")
	(ACTION_TILE "draw_fwg" "(setq bFGW t)(done_dialog 1)")
	(setq WHAT_NEXT (START_DIALOG))
	(COND ((= WHAT_NEXT 3) (DO_GETANGLE))
	      ((= WHAT_NEXT 4) (DO_GET_UCS1))
	      ((= WHAT_NEXT 5) (DO_GET_UCS2))
	      ((= WHAT_NEXT 6) (DO_PUTFILE))
	      ((= WHAT_NEXT 7) (DO_LANGUAGELIST))
	)
      )
      (if (AND (= 1 WHAT_NEXT) (= NBTV_DIMZB_UCS 2))
	(PROGN
	  (if
	    (OR
	      (NOT (EQUAL (CADR NBTV_DIMZB_UCS_PT0) NEWUCSPT0Y 1.0e-07))
	      (NOT (EQUAL (CAR NBTV_DIMZB_UCS_PT0) NEWUCSPT0X 1.0e-07))
	    )
	     (PROGN (NBTF_SETLDATA
		      "NBTV_DimZB_UCS_pt0"
		      (LIST NEWUCSPT0X NEWUCSPT0Y)
		    )
		    (NBTF_SETLDATA "NBTV_DimZB_UCS_sc" 1.0)
	     )
	  )
	  (if (NOT (EQUAL NBTV_DIMZB_UCS_ANG NEWUCSANG 1.0e-07))
	    (PROGN (NBTF_SETLDATA "NBTV_DimZB_UCS_ang" NEWUCSANG)
		   (NBTF_SETLDATA "NBTV_DimZB_UCS_sc" 1.0)
	    )
	  )
	)
      )
    )
    (DEFUN DO_DIM_BL_SL	()
      (NBTF_SETLDATA "NBTV_Dimzb_sc" (READ (STRCAT "1E" $VALUE)))
      (setq DIMZB_SC NBTV_DIMZB_SC)
      (SET_TILE "dim_bl" (RTOS DIMZB_SC 2 8))
      (FUN_DRAWIMAGE)
    )
    (DEFUN DO_DIM_BL ()
      (NBTF_DCL_VERIFY_DATA "dim_bl")
      (if (/= NBTV_DIMZB_UCS 2)
	(PROGN (NBTF_SETLDATA "NBTV_Dimzb_sc" (READ $VALUE))
	       (setq DIMZB_SC NBTV_DIMZB_SC)
	)
      )
      (FUN_DRAWIMAGE)
      (SET_TILE
	"Dim_bL_Sl"
	(ITOA (READ (LAST (NBTF_STRPARSE (RTOS DIMZB_SC 1 4) "E"))))
      )
    )
    (DEFUN DO_DIMPOINTSTYLE ()
      (NBTF_SETLDATA
	"NBTV_DimZB_pointstyle"
	(NBTF_DCL_LIST_GET $KEY)
      )
      (COND ((= NBTV_DIMZB_POINTSTYLE "_system")
	     (SETVAR "DIMLDRBLK" DIMLDRBLK_OLD)
	    )
	    (T (SETVAR "DIMLDRBLK" NBTV_DIMZB_POINTSTYLE))
      )
      (FUN_DRAWIMAGE)
      (if (= (setq DIMBLK_ID (GETVAR "DIMLDRBLK")) "")
	(PROGN (setq DIMBLK_ID "0"))
	(PROGN (if (setq TMP (ASSOC DIMBLK_ID DIMPOITSTYLELST))
		 (PROGN (setq DIMBLK_ID (CDR TMP)))
	       )
	       (if (WCMATCH DIMBLK_ID "~_*")
		 (PROGN (setq DIMBLK_ID (STRCAT "_" DIMBLK_ID)))
	       )
	       (setq DIMBLK_ID
		      (CDR
			(ASSOC
			  5
			  (ENTGET
			    (CDR
			      (ASSOC
				330
				(ENTGET (TBLOBJNAME "block" DIMBLK_ID))
			      )
			    )
			  )
			)
		      )
	       )
	)
      )
    )
    (DEFUN DO_DIMZB_TSTYLE ()
      (NBTF_SETLDATA
	"NBtv_DimZB_Textstyle"
	(NBTF_DCL_LIST_GET $KEY)
      )
      (COND ((= NBTV_DIMZB_TEXTSTYLE V_STR_DEFAULT)
	     (setq CTSTYLE "Dim_Coord")
	    )
	    ((= NBTV_DIMZB_TEXTSTYLE V_STR_ACTIVE)
	     (setq CTSTYLE (GETVAR "Textstyle"))
	    )
	    (T (setq CTSTYLE NBTV_DIMZB_TEXTSTYLE))
      )
    )
    (DEFUN DO_GET_UCS1 () (NBTF_SETCUSCOORD 1))
    (DEFUN DO_GET_UCS2 () (NBTF_SETCUSCOORD 2))
    (DEFUN DO_GETANGLE () (SETGDANG) (SET_ANG))
    (DEFUN DO_DANG ()
      (if (ANGTOF $VALUE 1)
	(PROGN (SET_TILE "error" "")
	       (NBTF_SETLDATA "NBTV_Dimzb_ang" (ANGTOF $VALUE 1))
	       (FUN_SETDIMZBANG)
	)
	(PROGN
	  (MODE_TILE $KEY 2)
	  (SET_TILE
	    "error"
	    (NBTF_GETLANGUAGESTR
	      '("数据错误!--本编辑框只能是角度表达式!!!"
		"Data error! This edit box can only be Angle expression!!! "
		"數據錯誤！--本編輯方塊只能是角度運算式!!!"
	       )
	    )
	  )
	)
      )
    )
    (DEFUN DO_LANGUAGELIST ()
      (COND ((= LANGINDEX "简体中文") (setq V_LANGUAGE "zh-cn"))
	    ((= LANGINDEX "English") (setq V_LANGUAGE "en"))
	    ((= LANGINDEX "繁體中文") (setq V_LANGUAGE "zh-tw"))
	    (T (setq V_LANGUAGE "zh-cn"))
      )
      (FUN_WRITEREG "Language" V_LANGUAGE)
    )
    (DEFUN FUN_SETTH ()
      (NBTF_DCL_VERIFY_DATA "ZB_th")
      (NBTF_SETLDATA "NBTV_Dimzb_tsize" (ATOF $VALUE))
      (NBTF_SETLDATA "NBTV_DimZB_pointsize" (ATOF $VALUE))
      (SETVAR "DIMASZ" NBTV_DIMZB_POINTSIZE)
      (SET_TILE "dimpsize" (RTOS NBTV_DIMZB_POINTSIZE 2 8))
    )
    (DEFUN DO_ZB_C ()
      (COND ((= $KEY "ZB_C_NO") (NBTF_SETLDATA "NBTV_bzhome" "nO"))
	    ((= $KEY "ZB_C_XY") (NBTF_SETLDATA "NBTV_bzhome" "Xy"))
	    ((= $KEY "ZB_C_NE") (NBTF_SETLDATA "NBTV_bzhome" "Ne"))
	    ((= $KEY "ZB_C_AB") (NBTF_SETLDATA "NBTV_bzhome" "Ab"))
      )
      (FUN_DRAWIMAGE)
    )
    (DEFUN DO_ZB_SHOWBH	()
      (COND ((= $KEY "nobh") (NBTF_SETLDATA "NBTV_dimzb_ShowBH" 0))
	    ((= $KEY "bhtop") (NBTF_SETLDATA "NBTV_dimzb_ShowBH" 1))
	    ((= $KEY "bhline") (NBTF_SETLDATA "NBTV_dimzb_ShowBH" 2))
	    (T (NBTF_SETLDATA "NBTV_dimzb_ShowBH" 0))
      )
      (FUN_DRAWIMAGE)
    )
    (DEFUN DO_NBTF_AUTOORIE ()
      (NBTF_SETLDATA "NBTV_Dimzb_AutoOrie" (READ $VALUE))
    )
    (DEFUN SET_UCS ()
      (COND ((= NBTV_DIMZB_UCS 0)
	     (SET_TILE "zb_ucs_act" "1")
	     (setq DIMZB_SC NBTV_DIMZB_SC)
	     (MODE_TILE "ucs_N" 1)
	     (MODE_TILE "ucs_E" 1)
	     (MODE_TILE "ucs_a" 1)
	     (MODE_TILE "ucs_f" 1)
	     (MODE_TILE "get_ucs1" 1)
	     (MODE_TILE "get_ucs2" 1)
	     (MODE_TILE "dim_bl" 0)
	     (MODE_TILE "Dim_bL_Sl" 0)
	    )
	    ((= NBTV_DIMZB_UCS 1)
	     (SET_TILE "ZB_ucs_word" "1")
	     (setq DIMZB_SC NBTV_DIMZB_SC)
	     (MODE_TILE "ucs_N" 1)
	     (MODE_TILE "ucs_E" 1)
	     (MODE_TILE "ucs_a" 1)
	     (MODE_TILE "ucs_f" 1)
	     (MODE_TILE "get_ucs1" 1)
	     (MODE_TILE "get_ucs2" 1)
	     (MODE_TILE "dim_bl" 0)
	     (MODE_TILE "Dim_bL_Sl" 0)
	    )
	    ((= NBTV_DIMZB_UCS 2)
	     (SET_TILE "ZB_ucs_cus" "1")
	     (setq DIMZB_SC NBTV_DIMZB_UCS_SC)
	     (MODE_TILE "ucs_N" 0)
	     (MODE_TILE "ucs_E" 0)
	     (MODE_TILE "ucs_a" 0)
	     (MODE_TILE "ucs_f" 0)
	     (MODE_TILE "get_ucs1" 0)
	     (MODE_TILE "get_ucs2" 0)
	     (MODE_TILE "dim_bl" 1)
	     (MODE_TILE "Dim_bL_Sl" 1)
	    )
      )
      (SET_TILE "dim_bl" (RTOS DIMZB_SC 2 8))
    )
    (DEFUN DO_NBTF_PSTYLE ()
      (if (= (NBTF_SETLDATA "NBTV_PICKSTYLE" (READ $VALUE)) 1)
	(PROGN (SETVAR "PICKSTYLE" (1+ (GETVAR "PICKSTYLE"))))
	(PROGN (SETVAR "PICKSTYLE" (1- (GETVAR "PICKSTYLE"))))
      )
    )
    (DEFUN SET_ANG ()
      (if (= NBTV_UCSCFG 1)
	(PROGN
	  (if (> (REM NBTV_DIMZB_ANG (* PI 2.0)) PI)
	    (PROGN
	      (NBTF_SETLDATA "NBTV_Dimzb_ang" (- NBTV_DIMZB_ANG PI))
	    )
	  )
	)
	(PROGN
	  (if (AND (>= NBTV_DIMZB_ANG (/ PI 2.0))
		   (< NBTV_DIMZB_ANG (* PI (/ 3 2.0)))
	      )
	    (PROGN
	      (NBTF_SETLDATA "NBTV_Dimzb_ang" (+ NBTV_DIMZB_ANG PI))
	    )
	  )
	)
      )
    )
    (DEFUN FUN_DRAWIMAGE (/	  PTMP	  PT1	  PT2	  SSTR1
			  SSTR2	  STRBH1  STRX1	  STRY1	  LINEL
			  LANG	  COLORAXIS
			 )
      (setq IMG_X (DIMX_TILE "ImagePreview"))
      (setq IMG_Y (DIMY_TILE "ImagePreview"))
      (setq COLORAXIS 1)
      (START_IMAGE "ImagePreview")
      (FILL_IMAGE 0 0 IMG_X IMG_Y -2)
      (END_IMAGE)
      (setq PTMP PTDEF)
      (COND ((= NBTV_BZHOME "Xy") (setq SSTR1 "X=") (setq SSTR2 "Y="))
	    ((= NBTV_BZHOME "Ab") (setq SSTR1 "A=") (setq SSTR2 "B="))
	    ((= NBTV_BZHOME "Ne") (setq SSTR1 "N=") (setq SSTR2 "E="))
	    (T (setq SSTR1 "") (setq SSTR2 ""))
      )
      (setq PTX1 (RTOS (* (CAR PTMP) DIMZB_SC) 2 NBTV_DIMZB_XS))
      (setq PTY1 (RTOS (* (CADR PTMP) DIMZB_SC) 2 NBTV_DIMZB_XS))
      (if (= NBTV_DIMZB_XYTOYX 0)
	(PROGN (setq STRX1 (STRCAT " " SSTR1 PTX1 " "))
	       (setq STRY1 (STRCAT " " SSTR2 PTY1 " "))
	)
	(PROGN (setq STRX1 (STRCAT " " SSTR1 PTY1 " "))
	       (setq STRY1 (STRCAT " " SSTR2 PTX1 " "))
	)
      )
      (setq LINEL (FIX (* (MAX (STRLEN STRX1) (STRLEN STRY1)) 5.7)))
      (setq PT1 (LIST (FIX (* IMG_X 0.5)) (FIX (* IMG_Y 0.3))))
      (setq PT2
	     (LIST (- (FIX (* IMG_X 0.5)) 60) (+ (FIX (* IMG_Y 0.3)) 60))
      )
      (setq STRBH1 (FUN_GETBHSTR))
      (if (= NBTV_DIMZB_SHOWBH 1)
	(PROGN (NBTF_VECTOR_TEXT
		 "ImagePreview"
		 "L"
		 (CAR PT1)
		 (- (CADR PT1) 14)
		 0
		 5
		 7
		 STRBH1
	       )
	)
      )
      (NBTF_VECTOR_TEXT
	"ImagePreview"
	"L"
	(CAR PT1)
	(CADR PT1)
	0
	5
	3
	STRX1
      )
      (NBTF_VECTOR_TEXT
	"ImagePreview"
	"L"
	(CAR PT1)
	(+ (CADR PT1) 14)
	0
	5
	3
	STRY1
      )
      (START_IMAGE "ImagePreview")
      (VECTOR_IMAGE
	(- (CAR PT1) 2)
	(+ (CADR PT1) 3)
	(+ (CAR PT1) LINEL)
	(+ (CADR PT1) 3)
	4
      )
      (VECTOR_IMAGE
	(- (CAR PT1) 2)
	(+ (CADR PT1) 3)
	(+ (CAR PT2) 10)
	(- (CADR PT2) 10)
	4
      )
      (VECTOR_IMAGE (- (CAR PT2) 10) (CADR PT2) 0 (CADR PT2) 50)
      (VECTOR_IMAGE
	(CAR PT2)
	(+ (CADR PT2) 10)
	(CAR PT2)
	IMG_Y
	50
      )
      (VECTOR_IMAGE 12 10 12 IMG_Y COLORAXIS)
      (VECTOR_IMAGE 12 10 16 15 COLORAXIS)
      (VECTOR_IMAGE 12 10 8 15 COLORAXIS)
      (VECTOR_IMAGE
	0
	(- IMG_Y 12)
	(- IMG_X 10)
	(- IMG_Y 12)
	COLORAXIS
      )
      (VECTOR_IMAGE
	(- IMG_X 10)
	(- IMG_Y 12)
	(- IMG_X 17)
	(- IMG_Y 16)
	COLORAXIS
      )
      (VECTOR_IMAGE
	(- IMG_X 10)
	(- IMG_Y 12)
	(- IMG_X 17)
	(- IMG_Y 8)
	COLORAXIS
      )
      (END_IMAGE)
      (NBTF_VECTOR_TEXT
	"ImagePreview"
	"L"
	11
	(CADR PT2)
	90
	5
	7
	(RTOS (CADR PTMP) 2 14)
      )
      (NBTF_VECTOR_TEXT
	"ImagePreview"
	"L"
	(+ (CAR PT2) 2)
	(- IMG_Y 2)
	0
	5
	7
	(RTOS (CAR PTMP) 2 14)
      )
      (FUN_DRAWLDR)
      (NBTF_LOGO "ImagePreview" (LIST (- IMG_X 5) 15))
    )
    (DEFUN FUN_DRAWLDR (/	BLKSTYLE	PTX	PTY	X1LST
			Y1LST	X2LST	Y2LST	COLLST	PT3	PT4
			I
		       )
      (setq PTX (CAR PT2))
      (setq PTY (CADR PT2))
      (if (= NBTV_DIMZB_POINTSTYLE "_system")
	(PROGN (setq BLKSTYLE (GETVAR "dimldrblk"))
	       (if (= BLKSTYLE "")
		 (PROGN (setq BLKSTYLE "."))
	       )
	       (if (setq TMP (ASSOC BLKSTYLE DIMPOITSTYLELST))
		 (PROGN (setq BLKSTYLE (CDR TMP)))
	       )
	       (if (AND (WCMATCH BLKSTYLE "~_*") (/= BLKSTYLE "."))
		 (PROGN (setq BLKSTYLE (STRCAT "_" BLKSTYLE)))
	       )
	)
	(PROGN (setq BLKSTYLE NBTV_DIMZB_POINTSTYLE))
      )
      (COND
	((= BLKSTYLE "_NBT_cross")
	 (setq X1LST (LIST PTX PTX (- PTX 7)))
	 (setq Y1LST (LIST PTY (- PTY 7) PTY))
	 (setq X2LST (LIST (+ PTX 10) PTX (+ PTX 7)))
	 (setq Y2LST (LIST (- PTY 10) (+ PTY 7) PTY))
	 (setq COLLST '(4 4 4))
	)
	((= BLKSTYLE ".")
	 (setq X1LST (LIST PTX))
	 (setq Y1LST (LIST PTY))
	 (setq X2LST (LIST (+ PTX 10)))
	 (setq Y2LST (LIST (- PTY 10)))
	 (setq COLLST '(4))
	 (setq I 5.25)
	 (while	(and (<= I 5.75))
	   (setq PT3 (POLAR PT2 I 14))
	   (setq X1LST (CONS PTX X1LST))
	   (setq Y1LST (CONS PTY Y1LST))
	   (setq X2LST (CONS (FIX (CAR PT3)) X2LST))
	   (setq Y2LST (CONS (FIX (CADR PT3)) Y2LST))
	   (setq COLLST (CONS 4 COLLST))
	   (setq PT3 PT4)
	   (setq I (+ I 0.05))
	 )
	 (setq PT3 (POLAR PT2 5.25 14))
	 (setq PT4 (POLAR PT2 5.75 14))
	 (setq X1LST (CONS (FIX (CAR PT3)) X1LST))
	 (setq Y1LST (CONS (FIX (CADR PT3)) Y1LST))
	 (setq X2LST (CONS (FIX (CAR PT4)) X2LST))
	 (setq Y2LST (CONS (FIX (CADR PT4)) Y2LST))
	 (setq COLLST (CONS 4 COLLST))
	)
	((= BLKSTYLE "_ClosedBlank")
	 (setq X1LST (LIST PTX PTX (+ PTX 7)))
	 (setq Y1LST (LIST PTY PTY (- PTY 12)))
	 (setq X2LST (LIST (+ PTX 7) (+ PTX 12) (+ PTX 12)))
	 (setq Y2LST (LIST (- PTY 12) (- PTY 7) (- PTY 7)))
	 (setq COLLST '(4 4 4))
	)
	((= BLKSTYLE "_Closed")
	 (setq X1LST (LIST PTX PTX PTX (+ PTX 7)))
	 (setq Y1LST (LIST PTY PTY PTY (- PTY 12)))
	 (setq X2LST (LIST (+ PTX 10) (+ PTX 7) (+ PTX 12) (+ PTX 12)))
	 (setq Y2LST (LIST (- PTY 10) (- PTY 12) (- PTY 7) (- PTY 7)))
	 (setq COLLST '(4 4 4 4))
	)
	((= BLKSTYLE "_Dot")
	 (FOREACH I '(1 2 3 4 5 6 7)
	   (NBTF_VECTOR_ARC "ImagePreview" 4 PT2 I 0 6.28)
	 )
	 (setq X1LST (LIST (+ PTX 6)))
	 (setq Y1LST (LIST (- PTY 6)))
	 (setq X2LST (LIST (+ PTX 10)))
	 (setq Y2LST (LIST (- PTY 10)))
	 (setq COLLST '(4))
	)
	((= BLKSTYLE "_ArchTick")
	 (setq X1LST (LIST PTX
			   (- PTX 2)
			   (- PTX 1)
			   (- PTX 0)
			   (+ PTX 1)
			   (+ PTX 2)
		     )
	 )
	 (setq Y1LST (LIST PTY
			   (- PTY 8)
			   (- PTY 8)
			   (- PTY 8)
			   (- PTY 8)
			   (- PTY 8)
		     )
	 )
	 (setq X2LST (LIST (+ PTX 10)
			   (- PTX 2)
			   (- PTX 1)
			   (- PTX 0)
			   (+ PTX 1)
			   (+ PTX 2)
		     )
	 )
	 (setq Y2LST (LIST (- PTY 10)
			   (+ PTY 8)
			   (+ PTY 8)
			   (+ PTY 8)
			   (+ PTY 8)
			   (+ PTY 8)
		     )
	 )
	 (setq COLLST '(4 4 4 4 4 4))
	)
	((= BLKSTYLE "_Oblique")
	 (setq X1LST (LIST PTX PTX))
	 (setq Y1LST (LIST PTY (- PTY 8)))
	 (setq X2LST (LIST (+ PTX 10) PTX))
	 (setq Y2LST (LIST (- PTY 10) (+ PTY 8)))
	 (setq COLLST '(4 4 4 4 4 4))
	)
	((= BLKSTYLE "_Open")
	 (setq X1LST (LIST PTX PTX PTX))
	 (setq Y1LST (LIST PTY PTY PTY))
	 (setq X2LST (LIST (+ PTX 10) (+ PTX 7) (+ PTX 12)))
	 (setq Y2LST (LIST (- PTY 10) (- PTY 12) (- PTY 7)))
	 (setq COLLST '(4 4 4 4))
	)
	((= BLKSTYLE "_Origin")
	 (FOREACH I '(7)
	   (NBTF_VECTOR_ARC "ImagePreview" 4 PT2 I 0 6.28)
	 )
	 (setq X1LST (LIST PTX))
	 (setq Y1LST (LIST PTY))
	 (setq X2LST (LIST (+ PTX 10)))
	 (setq Y2LST (LIST (- PTY 10)))
	 (setq COLLST '(4))
	)
	((= BLKSTYLE "_Origin2")
	 (FOREACH I '(3 7)
	   (NBTF_VECTOR_ARC "ImagePreview" 4 PT2 I 0 6.28)
	 )
	 (setq X1LST (LIST (+ PTX 6)))
	 (setq Y1LST (LIST (- PTY 6)))
	 (setq X2LST (LIST (+ PTX 10)))
	 (setq Y2LST (LIST (- PTY 10)))
	 (setq COLLST '(4))
	)
	((= BLKSTYLE "_Open90")
	 (setq X1LST (LIST PTX PTX PTX))
	 (setq Y1LST (LIST PTY PTY PTY))
	 (setq X2LST (LIST (+ PTX 10) PTX (+ PTX 10)))
	 (setq Y2LST (LIST (- PTY 10) (- PTY 10) PTY))
	 (setq COLLST '(4 4 4 4))
	)
	((= BLKSTYLE "_Open30")
	 (setq X1LST (LIST PTX PTX PTX))
	 (setq Y1LST (LIST PTY PTY PTY))
	 (setq X2LST (LIST (+ PTX 10) (+ PTX 5) (+ PTX 12)))
	 (setq Y2LST (LIST (- PTY 10) (- PTY 12) (- PTY 5)))
	 (setq COLLST '(4 4 4 4))
	)
	((= BLKSTYLE "_DotSmall")
	 (FOREACH I '(1 2 3)
	   (NBTF_VECTOR_ARC "ImagePreview" 4 PT2 I 0 6.28)
	 )
	 (setq X1LST (LIST (+ PTX 2)))
	 (setq Y1LST (LIST (- PTY 2)))
	 (setq X2LST (LIST (+ PTX 10)))
	 (setq Y2LST (LIST (- PTY 10)))
	 (setq COLLST '(4))
	)
	((= BLKSTYLE "_DotBlank")
	 (FOREACH I '(7)
	   (NBTF_VECTOR_ARC "ImagePreview" 4 PT2 I 0 6.28)
	 )
	 (setq X1LST (LIST (+ PTX 6)))
	 (setq Y1LST (LIST (- PTY 6)))
	 (setq X2LST (LIST (+ PTX 10)))
	 (setq Y2LST (LIST (- PTY 10)))
	 (setq COLLST '(4))
	)
	((= BLKSTYLE "_Small")
	 (FOREACH I '(3)
	   (NBTF_VECTOR_ARC "ImagePreview" 4 PT2 I 0 6.28)
	 )
	 (setq X1LST (LIST PTX))
	 (setq Y1LST (LIST PTY))
	 (setq X2LST (LIST (+ PTX 10)))
	 (setq Y2LST (LIST (- PTY 10)))
	 (setq COLLST '(4))
	)
	((= BLKSTYLE "_BoxBlank")
	 (setq X1LST (LIST (+ PTX 5) (- PTX 10) PTX (+ PTX 10) PTX))
	 (setq Y1LST (LIST (- PTY 5) PTY (+ PTY 10) PTY (- PTY 10)))
	 (setq X2LST (LIST (+ PTX 10) PTX (+ PTX 10) PTX (- PTX 10)))
	 (setq Y2LST (LIST (- PTY 10) (+ PTY 10) PTY (- PTY 10) PTY))
	 (setq COLLST '(4 4 4 4 4))
	)
	((= BLKSTYLE "_BoxFilled")
	 (setq X1LST (LIST (+ PTX 5)))
	 (setq Y1LST (LIST (- PTY 5)))
	 (setq X2LST (LIST (+ PTX 10)))
	 (setq Y2LST (LIST (- PTY 10)))
	 (setq COLLST '(4))
	 (FOREACH I '(0 1 2 3 4 5 6 7 8 9 10)
	   (setq X1LST (CONS (+ PTX I -10) X1LST))
	   (setq Y1LST (CONS (- PTY I) Y1LST))
	   (setq X2LST (CONS (+ PTX I) X2LST))
	   (setq Y2LST (CONS (- PTY I -10) Y2LST))
	   (setq COLLST (CONS 4 COLLST))
	 )
	)
	((= BLKSTYLE "_DatumBlank")
	 (setq X1LST (LIST (- PTX 7) (- PTX 7) (+ PTX 7)))
	 (setq Y1LST (LIST (- PTY 7) (- PTY 7) (+ PTY 7)))
	 (setq X2LST (LIST (+ PTX 7) (+ PTX 10) (+ PTX 10)))
	 (setq Y2LST (LIST (+ PTY 7) (- PTY 10) (- PTY 10)))
	 (setq COLLST '(4 4 4))
	)
	((= BLKSTYLE "_DatumFilled")
	 (setq X1LST (LIST (- PTX 7) (- PTX 7) (+ PTX 7)))
	 (setq Y1LST (LIST (- PTY 7) (- PTY 7) (+ PTY 7)))
	 (setq X2LST (LIST (+ PTX 7) (+ PTX 10) (+ PTX 10)))
	 (setq Y2LST (LIST (+ PTY 7) (- PTY 10) (- PTY 10)))
	 (setq COLLST '(4 4 4))
	 (FOREACH I '(0 1 2 3 4 5 6 7 8 9 10)
	   (setq X1LST (CONS (+ PTX -7 (FIX (* I 1.8))) X1LST))
	   (setq Y1LST (CONS (- PTY 7 (FIX (* I 0.4))) Y1LST))
	   (setq X2LST (CONS (+ PTX 7 (FIX (* I 0.4))) X2LST))
	   (setq Y2LST (CONS (- PTY -7 (FIX (* I 1.8))) Y2LST))
	   (setq COLLST (CONS 4 COLLST))
	 )
	)
	((= BLKSTYLE "_Integral")
	 (setq X1LST (LIST PTX))
	 (setq Y1LST (LIST PTY))
	 (setq X2LST (LIST (+ PTX 10)))
	 (setq Y2LST (LIST (- PTY 10)))
	 (setq COLLST '(4))
	 (NBTF_VECTOR_ARC
	   "ImagePreview"
	   4
	   (LIST (+ PTX 6) (- PTY 4))
	   8
	   2.56
	   3.72
	 )
	 (NBTF_VECTOR_ARC
	   "ImagePreview"
	   4
	   (LIST (- PTX 6) (+ PTY 4))
	   8
	   5.7
	   6.87
	 )
	)
	(T
	 (setq X1LST (LIST PTX))
	 (setq Y1LST (LIST PTY))
	 (setq X2LST (LIST (+ PTX 10)))
	 (setq Y2LST (LIST (- PTY 10)))
	 (setq COLLST '(4))
	)
      )
      (if X1LST
	(PROGN (START_IMAGE "ImagePreview")
	       (MAPCAR 'VECTOR_IMAGE X1LST Y1LST X2LST Y2LST COLLST)
	       (END_IMAGE)
	)
      )
    )
    (DEFUN FUN_DO_HELP (/ HELP_TEXT)
      (setq HELP_TEXT
	     (COND
	       ((= V_LANGUAGE "en")
		(STRCAT
		  "＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝"
		  "\n‖【NetBeeTool】-[DimCoord] 2.54" "\n‖"
		  "\n‖2.1  1.Add backgroundFill(2005 or more)"
		  "\n‖2.2  1.Coordinate data extraction file to add XYZ"
		  "\n‖     2.Y migration BUG when coordinates are updated"
		  "\n‖     3.Fix the BUG in the annotation that the user catch the setting missing by ESC cancel command"
		  "\n‖2.3  1.Fix custom frame scale can't save BUG"
		  "\n‖2.4  1.Fixed BUG that went wrong when entering model drawings from layout"
		  "\n‖2.5  1.Add grid drawing"
		  "\n‖2.52 1.win8.1＋2014 64-bit edition fixes"
		  "\n‖2.53 1.Fixed a BUG that could not update coordinates"
		  "\n‖2.54 1.Fix a BUG that was missing from osmode Settings"
		  "\n‖     2.Add简体中文/english/繁體中文language switch function"
		  "\n‖     3.The function of adding coordinate number can be changed"
		  "\n‖"
		  "\n‖       See more functions[NetBeeTool] netbee.ys168.com"
		  "\n‖                              " "\n‖"
		  "\n‖                     China Yunnan Construction Investment Holding Group Co., Ltd."
		  "\n‖　                                      YangMingFeng "
		  "\n‖                    QQ:14997-84427 "
		  "\n‖                   ymf5340@@163.com"
		  "\n‖                     2018.08.08"
		  "\n＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝")
	       )
	       ((= V_LANGUAGE "ch-tw")
		(STRCAT
		  "＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝"
		  "\n‖【網蜂工具】之[座標標注] 2.54" "\n‖"
		  "\n‖2.1  1.添加背景遮擋功能(2005及以上)"
		  "\n‖2.2  1.座標數據選取檔案添加XYZ"
		  "\n‖     2.座標更新時Y偏移BUG"
		  "\n‖     3.修復標注中用戶按ESC取消命令捕捉設定消失的BUG"
		  "\n‖2.3  1.修復自定義坐標系比例不能保存BUG"
		  "\n‖2.4  1.修改從佈局中進入模型圖紙標注時出錯的BUG"
		  "\n‖2.5  1.添加座標格網繪製"
		  "\n‖2.52 1.win8.1＋2014 64位版修復"
		  "\n‖2.53 1.修復不能更新座標的BUG"
		  "\n‖2.54 1.修復捕捉設定遺失的BUG"
		  "\n‖     2.添加中文/english/繁體中文語言切換功能"
		  "\n‖     3.添加座標編號可改功能" "\n‖"
		  "\n‖       更多功能看[網蜂工具箱] netbee.ys168.com"
		  "\n‖                              " "\n‖"
		  "\n‖                     雲南建投  楊明峰"
		  "\n‖                    QQ:14997-84427 "
		  "\n‖                   ymf5340@@163.com"
		  "\n‖                     2018.08.08"
		  "\n＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝")
	       )
	       (T
		(STRCAT
		  "＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝"
		  "\n‖【网蜂工具】之[坐标标注] 2.54" "\n‖"
		  "\n‖2.1  1.添加背景遮挡功能(2005及以上)"
		  "\n‖2.2  1.坐标数据提取文件添加XYZ"
		  "\n‖     2.坐标更新时Y偏移BUG"
		  "\n‖     3.修复标注中用户按ESC取消命令捕捉设置消失的BUG"
		  "\n‖2.3  1.修复自定义坐标系比例不能保存BUG"
		  "\n‖2.4  1.修改从布局中进入模型图纸标注时出错的BUG"
		  "\n‖2.5  1.添加坐标格网绘制"
		  "\n‖2.52 1.win8.1＋2014 64位版修复"
		  "\n‖2.53 1.修复不能更新坐标的BUG"
		  "\n‖2.54 1.修复捕捉设置丢失的BUG"
		  "\n‖     2.添加中文/english语言切换功能"
		  "\n‖     3.添加坐标编号可改功能" "\n‖"
		  "\n‖                                  "    
		  "\n‖                                  "                           
		  "\n‖                                  "                 
		  "\n‖                                  "                 
		  "\n‖                                  "             
		  "\n‖                     2018.08.08"
		  "\n＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝")
	       )
	     )
      )
      (ALERT HELP_TEXT)
    )
    
    (NBTF_SETLDATA "NBTV_dimzb_ShowBH" 0)
    (VLR-remove-all)
    (VLR-remove-all :VLR-Command-Reactor)
    (SETINI_ZB)
    (if	(= NBTV_DIMZB_CFG 0)
      (PROGN (NBTF_SETLDATA "NBTV_DimZB_CFG" 1) (SET_OTHER))
    )
    (ZBBZ_FIXUP)
    (SETCLOSE_ZB)
    (if	BFGW
      (PROGN (C:NBTC_DRAWZBFGW1))
    )
  )
)
'C:NBT-DIMCOORD
(vl-ACAD-defun (DEFUN C:ZBBZ () (C:NBT-DIMCOORD)))
'C:ZBBZ
(NBTF_GETLDATA '("NBTV_DimZB_FGWX" 10.0))
(NBTF_GETLDATA '("NBTV_DimZB_FGWY" 10.0))
(NBTF_GETLDATA '("NBTV_DimZB_fgw_fzx" "1"))
(NBTF_GETLDATA '("NBTV_DimZB_fgw_dim" "1"))
(NBTF_GETLDATA '("NBTV_DimZB_fgw_qz" "1"))

(DEFUN NBTF_SETUCS (/ OLDUCS OLDUCSXDIR OLDECHO)
  (setq OLDECHO (GETVAR "cmdecho"))
  (SETVAR "cmdecho" 0)
  (setq OLDUCS (GETVAR "UCSORG"))
  (setq OLDUCSXDIR (GETVAR "UCSXDIR"))
  (if (AND (EQUAL (CAR OLDUCS) (CAR NBTV_DIMZB_UCS_PT0) 1.0e-10)
	   (EQUAL (CADR OLDUCS) (CADR NBTV_DIMZB_UCS_PT0) 1.0e-10)
	   (EQUAL (CAR OLDUCSXDIR) (COS NBTV_DIMZB_UCS_ANG) 1.0e-10)
	   (EQUAL (CADR OLDUCSXDIR) (SIN NBTV_DIMZB_UCS_ANG) 1.0e-10)
      )
    (PROGN (PRINC "已经定义！"))
    (PROGN (if (TBLOBJNAME "UCS" "NBTV_old_ucs")
	     (PROGN (VL-CMDF "_.ucs" "S" "NBTV_old_ucs" "Y"))
	     (PROGN (VL-CMDF "_.ucs" "S" "NBTV_old_ucs"))
	   )
	   (VL-CMDF "_.ucs" "")
	   (VL-CMDF "_.ucs" "M" NBTV_DIMZB_UCS_PT0)
	   (VL-CMDF "_.ucs" "N" "Z" (ANGTOS NBTV_DIMZB_UCS_ANG 0 9))
    )
  )
  (SETVAR "cmdecho" OLDECHO)
  (PRINC)  
)

(setq NBTV_ALLFUNSLIST (CONS "NBTF_resetucs" NBTV_ALLFUNSLIST))


(DEFUN NBTF_RESETUCS (/ OLDECHO)
  (setq OLDECHO (GETVAR "cmdecho"))
  (SETVAR "cmdecho" 0)
  (if (TBLOBJNAME "UCS" "NBTV_old_ucs")
    (PROGN (VL-CMDF "_.ucs" "R" "NBTV_old_ucs"))
    (PROGN (PRINC "\n 错误：没有前一个坐标系！"))
  )
  (PRINC)
  (SETVAR "cmdecho" OLDECHO)
)


(DEFUN NBTF_GETPLPTL (ENAME / PLIST PP N I)
  (setq OBJ (vlax-ename->vla-object ENAME))
  (setq	PLIST (vlax-safearray->list
		(vlax-variant-value (vla-get-Coordinates OBJ))
	      )
  )
  (setq N 0)
  (if (= "AcDbPolyline" (vla-get-ObjectName OBJ))
    (PROGN (setq I 2))
    (PROGN (setq I 3))
  )
  (REPEAT (/ (LENGTH PLIST) I)
    (setq PP (APPEND PP (LIST (LIST (NTH N PLIST) (NTH (1+ N) PLIST)))))
    (setq N (+ N I))
  )
  PP
)


(DEFUN NBTF_GETPTLBOX (PTL)
  (LIST	(APPLY 'MAPCAR (CONS 'MIN PTL))
	(APPLY 'MAPCAR (CONS 'MAX PTL))
  )
)


(DEFUN NBTF_VXADDTEXT (TTEXT JUST PT TH / OBJ STYLENAME DBLWID DBLROT)
  (COND
    ((AND (= (TYPE TH) 'LIST)
	  (setq STYLENAME (CDR (ASSOC 7 TH)))
	  (setq DBLROT (CDR (ASSOC 50 TH)))
	  (setq DBLWID (CDR (ASSOC 41 TH)))
	  (setq TH (CDR (ASSOC 40 TH)))
	  (NOT TH)
     )
    )
    ((VL-CATCH-ALL-ERROR-P
       (setq
	 OBJ (VL-CATCH-ALL-APPLY
	       'vla-AddText
	       (LIST (NBTF_VXACTIVESPACE) TTEXT (vlax-3d-point PT) TH)
	     )
       )
     )
     (NBTF_VXOBJERR OBJ)
    )
    (T
     (if (AND STYLENAME (TBLOBJNAME "STYLE" STYLENAME))
       (PROGN (vla-put-StyleName OBJ STYLENAME))
     )
     (COND ((OR (= JUST "L") (NULL JUST))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
	   ((= JUST "C")
	    (vla-put-Alignment OBJ 1)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point PT))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
	   ((= JUST "R")
	    (vla-put-Alignment OBJ 2)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point PT))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
	   ((= JUST "A")
	    (vla-put-Alignment OBJ 3)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point (CADR PT)))
	   )
	   ((= JUST "M")
	    (vla-put-Alignment OBJ 4)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point PT))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
	   ((= JUST "F")
	    (vla-put-Alignment OBJ 5)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point (CADR PT)))
	   )
	   ((= JUST "TL")
	    (vla-put-Alignment OBJ 6)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point PT))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
	   ((= JUST "TC")
	    (vla-put-Alignment OBJ 7)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point PT))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
	   ((= JUST "TR")
	    (vla-put-Alignment OBJ 8)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point PT))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
	   ((= JUST "ML")
	    (vla-put-Alignment OBJ 9)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point PT))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
	   ((= JUST "MC")
	    (vla-put-Alignment OBJ 10)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point PT))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
	   ((= JUST "MR")
	    (vla-put-Alignment OBJ 11)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point PT))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
	   ((= JUST "BL")
	    (vla-put-Alignment OBJ 12)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point PT))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
	   ((= JUST "BC")
	    (vla-put-Alignment OBJ 13)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point PT))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
	   ((= JUST "BR")
	    (vla-put-Alignment OBJ 14)
	    (vla-put-TextAlignmentPoint OBJ (vlax-3d-point PT))
	    (if	DBLWID
	      (PROGN (vla-put-ScaleFactor OBJ DBLWID))
	    )
	    (if	DBLROT
	      (PROGN (vla-put-Rotation OBJ DBLROT))
	    )
	   )
     )
     (vla-Update OBJ)
     (vlax-release-object OBJ)
     (ENTLAST)
    )
  )
)


(DEFUN NBTF_POINTAB->XY
       (PT0 ANG SCALE PT1 / PIX2 I PT MATRIX X Y Z X1 Y1 Z1)
  (setq PIX2 (* PI 2))
  (setq X1 (CAR PT1))
  (setq Y1 (CADR PT1))
  (setq	Z1 (if (CADDR PT1)
	     (PROGN (CADDR PT1))
	     (PROGN 0)
	   )
  )
  (setq	MATRIX (LIST (LIST (* SCALE (COS ANG)) (* SCALE (SIN ANG)) 0)
		     (LIST (* SCALE (SIN (- PIX2 ANG)))
			   (* SCALE (COS (- PIX2 ANG)))
			   0
		     )
		     (LIST 0 0 1)
		     (LIST (CAR PT0) (CADR PT0) 0)
	       )
  )
  (setq I 0)
  (setq PT nil)
  (while (and (< I 3))
    (setq PT (APPEND PT
		     (LIST (+ (* (NTH I (NTH 0 MATRIX)) X1)
			      (* (NTH I (NTH 1 MATRIX)) Y1)
			      (* (NTH I (NTH 2 MATRIX)) Z1)
			      (NTH I (NTH 3 MATRIX))
			   )
		     )
	     )
    )
    (setq I (1+ I))
  )
  PT
)


(vl-ACAD-defun
  (DEFUN C:NBTC_DRAWZBFGW1 (/		 WHAT_NEXT    E
			    PRG_UCS1_2	 PRG_UCS2_1   FUN_RADIOCHG
			    DCL_ID	 FUN_DCLDEF   FUN_CREATBLK
			    FUN_CHKVALUE FUN_TOGGLECHG
			    FUN_DO	 PT1	      PT2
			    ERR_OLD	 FUN_ERRNEW   FUN_SETINI
			    FUN_CLOSE	 OLD_CLAY     OLD_OSMODE
			    DIMZB_SC	 JD	      SSLINE
			   )
    (DEFUN FUN_SETINI ()
      (setq ERR_OLD *ERROR*)
      (setq OLD_CLAY (GETVAR "clayer"))
      (setq *ERROR* FUN_ERRNEW)
      (setq OLD_OSMODE (GETVAR "OSMODE"))
      (setq SSLINE (SSADD))
      (SETVAR "cmdecho" 0)
      (command "_.undo")
      (command "be")
      (if (= NBTV_DIMZB_UCS 2)
	(PROGN (setq DIMZB_SC NBTV_DIMZB_UCS_SC))
	(PROGN (setq DIMZB_SC NBTV_DIMZB_SC))
      )
      (FUN_CREATBLK)
    )
    (DEFUN FUN_ERRNEW (MSG) (PRINC MSG) (FUN_CLOSE))
    (DEFUN FUN_CLOSE ()
      (COND ((= NBTV_DIMZB_UCS 1) (NBTF_RESETUCS))
	    ((= NBTV_DIMZB_UCS 2) (NBTF_RESETUCS))
      )
      (setq *ERROR* ERR_OLD)
      (SETVAR "clayer" OLD_CLAY)
      (SETVAR "OSMODE" OLD_OSMODE)
      (command "_.undo")
      (command "e")
      (SETVAR "cmdecho" 1)
      (if E
	(PROGN (ENTDEL E))
      )
    )
    (DEFUN PRG_UCS2_1 (PT)
      (NBTF_POINTAB->XY
	NBTV_DIMZB_UCS_PT0
	NBTV_DIMZB_UCS_ANG
	DIMZB_SC
	PT
      )
    )
    (DEFUN PRG_UCS1_2 (PT)
      (NBTF_POINTXY->AB
	NBTV_DIMZB_UCS_PT0
	NBTV_DIMZB_UCS_ANG
	DIMZB_SC
	PT
      )
    )
    (DEFUN FUN_CREATBLK	()
      (if (NOT (TBLOBJNAME "block" "_NBTB_ZBGW_Cross"))
	(PROGN
	  (ENTMAKE '((0 . "BLOCK")
		     (8 . "0")
		     (10 0.0 0.0 0.0)
		     (2 . "_NBTB_ZBGW_Cross")
		     (70 . 0)
		     (1 . "")
		    )
	  )
	  (ENTMAKE '((0 . "LINE")
		     (8 . "0")
		     (67 . 0)
		     (10 -0.5 0.0 0.0)
		     (11 0.5 0.0 0.0)
		     (210 0.0 0.0 1.0)
		    )
	  )
	  (ENTMAKE
	    '((0 . "LINE") (8 . "0") (10 0.0 -0.5 0.0) (11 0.0 0.5 0.0))
	  )
	  (ENTMAKE '((0 . "Endblk")))
	)
      )
    )
    (DEFUN FUN_DO (/	   PTL	   PTMIN   PTMAX   DISTI   DISTJ
		   DDX	   DDY	   DX	   DY	   PTNEW   BYAXIS
		   SSTR1   SSTR2   PTSTRX  PTSTRY  SPTX	   SPTY
		   BNEWPT
		  )
      (NBTF_SETCLAYER '("NB_CoordGrid" 3))
      (COND ((= NBTV_DIMZB_UCS 1)
	     (if (TBLOBJNAME "UCS" "NBTV_old_ucs")
	       (PROGN (VL-CMDF "_.ucs" "S" "NBTV_old_ucs" "Y"))
	       (PROGN (VL-CMDF "_.ucs" "S" "NBTV_old_ucs"))
	     )
	     (VL-CMDF "_.ucs" "w")
	    )
	    ((= NBTV_DIMZB_UCS 2) (NBTF_SETUCS))
      )
      (COND ((= NBTV_BZHOME "Xy") (setq SSTR1 "X=") (setq SSTR2 "Y="))
	    ((= NBTV_BZHOME "Ab") (setq SSTR1 "A=") (setq SSTR2 "B="))
	    ((= NBTV_BZHOME "Ne") (setq SSTR1 "N=") (setq SSTR2 "E="))
	    (T (setq SSTR1 "") (setq SSTR2 ""))
      )
      (setq JD 0)
      (if (AND (setq PT1 (GETPOINT "\n拾取格网角点<退出>:"))
	       (VL-CMDF "_.rectang" PT1 PAUSE)
	  )
	(PROGN
	  (SETVAR "OSMODE" 0)
	  (setq E (ENTLAST))
	  (setq PTL (NBTF_GETPLPTL E))
	  (setq PTL (MAPCAR '(LAMBDA (X) (TRANS X 0 1)) PTL))
	  (setq PTL (NBTF_GETPTLBOX PTL))
	  (setq PTMIN (CAR PTL))
	  (setq PTMAX (CADR PTL))
	  (setq DX (/ NBTV_DIMZB_FGWX DIMZB_SC))
	  (setq DY (/ NBTV_DIMZB_FGWY DIMZB_SC))
	  (ENTDEL E)
	  (setq E nil)
	  (COND	((= NBTV_DIMZB_FGW_QZ "1")
		 (setq PTMIN (LIST (* (FIX ((if	(> (CAR PTMIN) 0)
					      (PROGN +)
					      (PROGN -)
					    )
					     (/ (CAR PTMIN) DX)
					     0.5
					   )
				      )
				      DX
				   )
				   (* (FIX ((if	(> (CADR PTMIN) 0)
					      (PROGN +)
					      (PROGN -)
					    )
					     (/ (CADR PTMIN) DY)
					     0.5
					   )
				      )
				      DY
				   )
			     )
		 )
		)
		((= NBTV_DIMZB_FGW_QZ "2")
		 (setq
		   PTMIN (LIST (* (FIX ((if (> (CAR PTMIN) 0)
					  (PROGN +)
					  (PROGN -)
					)
					 (/ (CAR PTMIN) (/ 10 DIMZB_SC))
					 0.5
				       )
				  )
				  (/ 10 DIMZB_SC)
			       )
			       (* (FIX ((if (> (CADR PTMIN) 0)
					  (PROGN +)
					  (PROGN -)
					)
					 (/ (CADR PTMIN) (/ 10 DIMZB_SC))
					 0.5
				       )
				  )
				  (/ 10 DIMZB_SC)
			       )
			 )
		 )
		)
		(T
		 (setq PTMIN (LIST (/ (FIX ((if	(> (CAR PTMIN) 0)
					      (PROGN +)
					      (PROGN -)
					    )
					     (* (CAR PTMIN) DIMZB_SC)
					     0.5
					   )
				      )
				      DIMZB_SC
				   )
				   (/ (FIX ((if	(> (CADR PTMIN) 0)
					      (PROGN +)
					      (PROGN -)
					    )
					     (* (CADR PTMIN) DIMZB_SC)
					     0.5
					   )
				      )
				      DIMZB_SC
				   )
			     )
		 )
		)
	  )
	  (setq DDX (- (CAR PTMAX) (CAR PTMIN)))
	  (setq DDY (- (CADR PTMAX) (CADR PTMIN)))
	  (setq DISTI (- 0 DX))
	  (if (< DDX DX)
	    (PROGN (setq DDX DX) (setq BNEWPT T))
	  )
	  (if (< DDY DY)
	    (PROGN (setq DDY DY) (setq BNEWPT T))
	  )
	  (setq	DDX (* (FIX ((if (> DDX 0)
			       (PROGN +)
			       (PROGN -)
			     )
			      (/ DDX DX)
			      0.5
			    )
		       )
		       DX
		    )
	  )
	  (setq	DDY (* (FIX ((if (> DDY 0)
			       (PROGN +)
			       (PROGN -)
			     )
			      (/ DDY DY)
			      0.5
			    )
		       )
		       DY
		    )
	  )
	  (setq PTMAX (MAPCAR '+ PTMIN (LIST DDX DDY)))
	  (VL-CMDF "_.rectang" PTMIN PTMAX)
	  (setq BYAXIS nil)
	  (while (and (<= (setq DISTI (+ DISTI DX)) DDX))
	    (setq DISTJ (- 0 DY))
	    (while (and (<= (setq DISTJ (+ DISTJ DY)) DDY))
	      (setq PTNEW (MAPCAR '+ PTMIN (LIST DISTI DISTJ)))
	      (VL-CMDF "_.insert"
		       "_NBTB_ZBGW_Cross"
		       PTNEW
		       (/ DX 10)
		       ""
		       ""
	      )
	      (if (AND (NULL BYAXIS) (= NBTV_DIMZB_FGW_FZX "1"))
		(PROGN (NBTF_SETCLAYER '("坐标格网辅助线" 8 NIL T))
		       (VL-CMDF	"_.line"
				(LIST (CAR PTMIN) (+ (CADR PTMIN) DISTJ))
				(LIST (CAR PTMAX) (+ (CADR PTMIN) DISTJ))
				""
		       )
		       (SSADD (ENTLAST) SSLINE)
		       (NBTF_SETCLAYER nil)
		)
	      )
	      (if (AND (NULL BYAXIS) (= NBTV_DIMZB_FGW_DIM "1"))
		(PROGN
		  (setq
		    PTSTRY (RTOS (* (+ (CADR PTMIN) DISTJ) DIMZB_SC)
				 2
				 JD
			   )
		  )
		  (if (= NBTV_DIMZB_XYTOYX 0)
		    (PROGN (setq SPTY (STRCAT "  " SSTR2 PTSTRY "  ")))
		    (PROGN (setq SPTY (STRCAT "  " SSTR1 PTSTRY "  ")))
		  )
		  (NBTF_VXADDTEXT
		    SPTY
		    "MR"
		    (TRANS (LIST (CAR PTMIN) (+ (CADR PTMIN) DISTJ))
			   1
			   0
		    )
		    (LIST (CONS 40 (/ DX 30))
			  (CONS 50 (ANGLE '(0 0) (GETVAR "UCSXDIR")))
		    )
		  )
		  (NBTF_VXADDTEXT
		    SPTY
		    "ML"
		    (TRANS (LIST (CAR PTMAX) (+ (CADR PTMIN) DISTJ))
			   1
			   0
		    )
		    (LIST (CONS 40 (/ DX 30))
			  (CONS 50 (ANGLE '(0 0) (GETVAR "UCSXDIR")))
		    )
		  )
		)
	      )
	    )
	    (setq BYAXIS T)
	    (if	(= NBTV_DIMZB_FGW_FZX "1")
	      (PROGN (NBTF_SETCLAYER '("坐标格网辅助线" 8 NIL T))
		     (VL-CMDF "_.line"
			      (LIST (+ (CAR PTMIN) DISTI) (CADR PTMIN))
			      (LIST (+ (CAR PTMIN) DISTI) (CADR PTMAX))
			      ""
		     )
		     (SSADD (ENTLAST) SSLINE)
		     (NBTF_SETCLAYER nil)
	      )
	    )
	    (if	(= NBTV_DIMZB_FGW_DIM "1")
	      (PROGN
		(setq PTSTRX
		       (RTOS (* (+ (CAR PTMIN) DISTI) DIMZB_SC) 2 JD)
		)
		(if (= NBTV_DIMZB_XYTOYX 0)
		  (PROGN (setq SPTX (STRCAT "  " SSTR1 PTSTRX "  ")))
		  (PROGN (setq SPTX (STRCAT "  " SSTR2 PTSTRX "  ")))
		)
		(NBTF_VXADDTEXT
		  SPTX
		  "ML"
		  (TRANS (LIST (+ (CAR PTMIN) DISTI) (CADR PTMIN)) 1 0)
		  (LIST
		    (CONS 40 (/ DX 30))
		    (CONS
		      50
		      (- (ANGLE '(0 0) (GETVAR "UCSXDIR")) (* PI 0.5))
		    )
		  )
		)
		(NBTF_VXADDTEXT
		  SPTX
		  "MR"
		  (TRANS (LIST (+ (CAR PTMIN) DISTI) (CADR PTMAX)) 1 0)
		  (LIST
		    (CONS 40 (/ DX 30))
		    (CONS
		      50
		      (- (ANGLE '(0 0) (GETVAR "UCSXDIR")) (* PI 0.5))
		    )
		  )
		)
	      )
	    )
	  )
	  (VL-CMDF "_.draworder" SSLINE "" "B")
	)
      )
    )

    
    (DEFUN FUN_DCLDEF ()
      (SET_TILE "fgw_distx" (RTOS NBTV_DIMZB_FGWX 2))
      (SET_TILE "fgw_disty" (RTOS NBTV_DIMZB_FGWY 2))
      (SET_TILE "fgw_fzx" NBTV_DIMZB_FGW_FZX)
      (SET_TILE "fgw_dim" NBTV_DIMZB_FGW_DIM)
      (SET_TILE "fgw_qz1" NBTV_DIMZB_FGW_QZ)
      (COND ((= NBTV_DIMZB_FGW_QZ "1") (SET_TILE "fgw_qz1" "1"))
	    ((= NBTV_DIMZB_FGW_QZ "2") (SET_TILE "fgw_qz2" "1"))
	    (T (SET_TILE "fgw_qz3" "1"))
      )
    )

    
    (DEFUN FUN_CHKVALUE	()
      (NBTF_DCL_VERIFY_DATA $KEY)
      (COND ((= $KEY "fgw_distx")
	     (if (/= (RTOS NBTV_DIMZB_FGWX 2) $VALUE)
	       (PROGN (NBTF_SETLDATA "NBTV_DimZB_FGWX" (ATOF $VALUE)))
	     )
	    )
	    ((= $KEY "fgw_disty")
	     (if (/= (RTOS NBTV_DIMZB_FGWY 2) $VALUE)
	       (PROGN (NBTF_SETLDATA "NBTV_DimZB_FGWy" (ATOF $VALUE)))
	     )
	    )
      )
    )

    
    (DEFUN FUN_TOGGLECHG ()
      (NBTF_SETLDATA (STRCAT "NBTV_DimZB_" $KEY) $VALUE)
    )
    (DEFUN FUN_RADIOCHG	()
      (COND ((= $KEY "fgw_qz1") (NBTF_SETLDATA "NBTV_DimZB_fgw_qz" "1"))
	    ((= $KEY "fgw_qz2") (NBTF_SETLDATA "NBTV_DimZB_fgw_qz" "2"))
	    ((= $KEY "fgw_qz3") (NBTF_SETLDATA "NBTV_DimZB_fgw_qz" "3"))
      )
    )

    
    (FUN_SETINI)
    
    (NBTF_SHOWFREEINFO)
    
    (setq WHAT_NEXT 8)    
    (while (and (< 2 WHAT_NEXT))
      (if (NOT (NEW_DIALOG
		 "SetZBFGW"
		 (setq DCL_ID (LOAD_DIALOG "zbbz.dcl"))
	       )
	  )
	(PROGN (EXIT))
      )
      (FUN_DCLDEF)
      (ACTION_TILE "fgw_distx" "(FUN_ChkValue)")
      (ACTION_TILE "fgw_disty" "(FUN_ChkValue)")
      (ACTION_TILE "fgw_fzx" "(Fun_toggleCHG)")
      (ACTION_TILE "fgw_dim" "(Fun_toggleCHG)")
      (ACTION_TILE "fgw_qz1" "(Fun_radioCHG)")
      (ACTION_TILE "fgw_qz2" "(Fun_radioCHG)")
      (ACTION_TILE "fgw_qz3" "(Fun_radioCHG)")
      (setq WHAT_NEXT (START_DIALOG))
    )
    (if	(= WHAT_NEXT 1)
      (PROGN (FUN_DO))
    )
    (FUN_CLOSE)
  )
)
'C:NBTC_DRAWZBFGW1
(PRINC
  (NBTF_GETLANGUAGESTR
    '("\n[网蜂工具]-坐标标注2.54 快捷命令:ZBBZ  完整命令：NBT-DimCoord   "
      "\n[NetBeeTool]-DimCoord 2.54 shortcut command:ZBBZ  complete command：NBT-DimCoord   "
      "\n[網蜂工具]-座標標注2.54 快捷命令:ZBBZ  完整命令：NBT-DimCoord   "
     )
  )
)
