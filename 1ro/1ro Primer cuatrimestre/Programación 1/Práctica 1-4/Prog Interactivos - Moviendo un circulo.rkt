;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Prog Interactivos - Moviendo un circulo|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
;El estado del programa será la posición en el eje x de la figura en movimiento. El estado será un número.


(define ANCHO 500)
(define ALTO 250) ;Dimenciones de la escena

(define MOV 30);Unidades que se desplazará el OBJ

(define ESCENA (empty-scene ANCHO ALTO))
(define OBJ (circle 30 "solid" "red")) ;Objeto que se desplazará en la ESCENA

;interpretar: Estado -> Image
;Recibe la posición del objeto y devuelve una imágen con el objeto en esta posición
(define (interpretar Estado)
  (place-image OBJ
               Estado (/ ALTO 2) ESCENA))


;manejadorTeclado: Estado String -> Estado
;Si se presiona la tecla deseada, esta función recibirá el estado actual y devolverá un estado nuevo con la imágen en la nueva posición
;Si se presiona cualquier otra tecla se devuelve el estado anterior
(define (manejadorTeclado Estado Tecla)
  (cond [(key=? "right" Tecla) (+ Estado MOV)]
        [(key=? "left" Tecla) (- Estado MOV)]
        [else Estado])
        )

(define INICIAL 30) ;Estado inicial del programa

(big-bang INICIAL
  [to-draw interpretar]
  [on-key manejadorTeclado]
  )




