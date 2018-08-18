(vl-load-com)

(defun c:rara()
  (setq acadObject (vlax-get-acad-object ))
  (setq acadDocument (vla-get-ActiveDocument acadObject))
  (setq mSpace (vla-get-ModelSpace acadDocument))

  (setq layColl (vla-get-layers acadDocument))
  (setq layerObj (vla-add layColl "123a-owal" ))


  ;diaplay new layername
  (setq layername (vla-get-name layerObj))
  (princ
    (strcat "new layer name is :"  layername  "\n")
    )

  ;change name
  (vla-put-Name  layerObj  "qq-weixin")
  (setq layername (vla-get-name layerObj))
  (princ
    (strcat "new layer name is :"  layername  "\n")
  )
  
)