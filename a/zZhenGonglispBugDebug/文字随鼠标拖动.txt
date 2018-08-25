;;;插入文字;;;;;;;;;;
;autolisp 怎么让生成的图形或者文字随着鼠标移动 然后点击后才固定到屏幕上呢;
(defun C:sx9 ( / #k %k i gr n pt)
 (setq #k "1";文字内容
       %k T  ;循环条件
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
    (cons 40 200)      ;;文字高度
    (cons 41 0.7)      ;;文字倾斜度
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
 (prin1)
);复制到记事本以（SX9.lsp）命名，加载到AutoCAD就可以使用。
;;;;;;;;;;;;说明一下;;
 ;这个是一个插入文字内容为"1"的程序，如果你有文字内容，那么修改为下面的子程序：
(defun sx9 (#k / #k %k i gr n pt)
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
    (cons 40 200)      ;;文字高度
    (cons 41 0.7)      ;;文字倾斜度
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
);原理是写好了文字，由于鼠标移动，那么删除文字并重新写文字，这样就看上去文字在移动
;追问
;哇，，谢谢哦
 ;回答由提问者推荐
; 11   