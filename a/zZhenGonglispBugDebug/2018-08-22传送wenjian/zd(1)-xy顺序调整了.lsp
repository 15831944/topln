(princ "\nzd.lsp已经加载。")
(princ "\n输入命令： zd 进行展点。")

(defun c:zd (/ fn f s p)
	
	(setq fn (getfiled "请选择坐标数据文件" "G:/" "txt" 8))
	(if (/= nil fn) 
		(progn
			(setq f (open fn "r"))
			(if (/= nil f)
				(progn
					(while (/= nil (setq s (read-line f)))
						(setq p (cdr (parse2 s ",")) p (list (nth 0 p) (nth 1 p) (nth 2 p)) p (append '(10) (mapcar 'atof p)))
						(entmake (list 
											 '(0 . "POINT") 
											 '(100 . "AcDbEntity") 
											 '(100 . "AcDbPoint") 
											 p 
										 )	
						)
					)	
					(close f)
					(setvar "PDMODE" 35)
					(setvar "PDSIZE" 2)
					(command "zoom" "e")
				)
			)
		)
	)
	(prin1)	
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
