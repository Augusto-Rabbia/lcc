;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Prog Interactivos - Cuadros q cambian de color|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
;Este programa será un programa interactivo que mostrará un cuadrado que alternará entre el color rojo, azul y violeta
;Cada segundo.

;Creación de la escena.
(define ALTO 250) ;alto de la escena
(define ANCHO 500) ;ancho de la escena

(define ESCENA (empty-scene ANCHO ALTO))

(define FigureSize 60) ;tamaño del cuadrado

;--------------------------------------
;Interpretar: Estado -> Imagen
;Recibe el estado (color del cuadrado) y devuelve la interpretación en forma de imágen
;(cuadrado de ese color en la escena)

(define (interpretar estado-color)
  (place-image (square FigureSize "solid" estado-color)
               (/ ANCHO 2) (/ ALTO 2) ESCENA))

;--------------------------------------
;CambiarColor: Estado -> Estado.
;Recibe el color actual y devuelve el color del próximo estado.

(check-expect (CambiarColor "red") "blue")
(check-expect (CambiarColor "blue") "violet")
(check-expect (CambiarColor "violet") "red");testing

(define (CambiarColor estado)
  (cond [(string=? estado "red") "blue"]
        [(string=? estado "blue") "violet"]
        [(string=? estado "violet") "red"]
        [else "red"]))

;--------------------------------------
;Estado inicial del programa

(define EstadoINICIAL "red")

;Adicional: Cambiar luego de X cambios de color?
(define N_Inicial 1)
(define LimCambios 15) ;Número de Cambios de color totales deseados

(define (N_deCambios LimCambios)
      (define N_Actual (+ N_Inicial 1))
        )



(big-bang EstadoINICIAL
  [to-draw interpretar]
  [on-tick CambiarColor N_Cambios 1]
  [stop-when (= N_Actual LimCambios)]
  )










