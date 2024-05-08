;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Augusto Rabbia - Primer Parcial Programación I|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
#|
Primer parcial, Programación I - Ejercicio 6

Alumno: Rabbia Augusto
|#

; Configuración de la Escena
(define ANCHO 500) ; Ancho de la Escena
(define ALTO 250) ; Alto de la Escena
(define ESCENA (rectangle ANCHO ALTO "solid" "black")) ;Escena

(define R 100) ; Radio de la trayectoria

; Centro de la escena
(define CX (/ ANCHO 2))
(define CY (/ ALTO 2))

#|
Estructura para representar el estado del programa.
- ang:  Ángulo de rotación del satélite.
- color:  Color del circulo central.
|#
(define-struct sysSatelite [ang  color])


; interpretar: Estado -> Imágen
; Recibe el estado actual y dibuja el planeta con su satélite en s posición correcta
; Y con su color correcto.
(define (interpretar s)
  (place-image (circle 20 "solid" (sysSatelite-color s))
                         CX CY
               (place-image (circle 10 "solid" "cyan")
                            (X s) (Y s)
                            (place-image (circle R "outline" "red")
                                                   CX CY
                                                   ESCENA))))

;X: Estado -> Number
;Interpreta el estado del programa y devuelve en forma de número
;la posición del satélite en X
(define (X s)
            (+ (* R (cos (sysSatelite-ang s)))  CX))

(check-within (X (make-sysSatelite pi "red")) 150 150) ;Caso en que se el satélite se encuentra hacia la izquierda del circulo central
(check-within (X (make-sysSatelite 0 "red")) 350 350) ;Caso en que se el satélite se encuentra hacia la derecha del circulo central


;Y: Estado -> Number
;Interpreta el estado del programa y devuelve en forma de número
;la posición del satélite en Y
(define (Y s)
            (+ (* R (sin (sysSatelite-ang s))) CY))

(check-within (Y (make-sysSatelite (/ pi 2) "red")) 225 225)  ;Caso en que se el satélite se encuentra por debajo del circulo central
(check-within (Y (make-sysSatelite 0 "red")) 125 125) ;Caso en que se el satélite se encuentra hacia la derecha del circulo central


; manejadorOnTick: Estado -> Estado
; Calcula la posición del satélite ante un nuevo tick del reloj.
(define (manejadorOnTick s)
  (if (equal? (sysSatelite-color s) "red")
                        (make-sysSatelite (+ (sysSatelite-ang s) (* 0.1 pi)) (sysSatelite-color s))
                        (make-sysSatelite (+ (sysSatelite-ang s) (* 0.05 pi)) (sysSatelite-color s))))

(check-within (manejadorOnTick (make-sysSatelite 1 "red")) (make-sysSatelite (+ 1 (* 0.1 pi)) "red") 2) ;Caso en que se mueve el satélite siendo rojo
(check-within (manejadorOnTick (make-sysSatelite 1 "green")) (make-sysSatelite (+ 1 (* 0.05 pi)) "green") 2) ;Caso en que se mueve el satélite siendo verde
  
; manejadorTeclado: Estado String -> Estado
; Al presionarse una tecla, devuelve un estado con el color del circulo central deseado.
(define (manejadorTeclado s k)
  (cond [(equal? k "r") (make-sysSatelite (sysSatelite-ang s) "red")]
        [(equal? k "a") (make-sysSatelite (sysSatelite-ang s) "yellow")]
        [(equal? k "v") (make-sysSatelite (sysSatelite-ang s) "green")]
        [(equal? k "x") (make-sysSatelite (sysSatelite-ang s) "black")]
        [else s]))

(check-expect (manejadorTeclado (make-sysSatelite 0 "red") "x") (make-sysSatelite 0 "black")) ;Caso en que el circulo sea rojo y se lo quiera cambiar a negro
(check-expect (manejadorTeclado (make-sysSatelite 0 "yellow") "r") (make-sysSatelite 0 "red")) ;Caso en que el circulo sea amarillo y se lo quiera cambiar a rojo
(check-expect (manejadorTeclado (make-sysSatelite 0 "green") "v") (make-sysSatelite 0 "green")) ;Caso en que se presione la tecla del color actual (debe devolver el mismo)
(check-expect (manejadorTeclado (make-sysSatelite 0 "red") "a") (make-sysSatelite 0 "yellow")) ;Caso en que el circulo sea rojo y se lo quiera cambiar a amarillo

;Estado inicial del programa
(define INICIAL (make-sysSatelite pi "yellow")) 

; stop?: Estado -> Boolean
; Recibe el estado actual y determina si el programa debe o no terminar
(define (stop? s)
  (if (equal? (sysSatelite-color s) "black") #true #false))

(check-expect (stop? (make-sysSatelite 0 "red")) #false) ;Caso en que el circulo central sea rojo
(check-expect (stop? (make-sysSatelite 0 "green")) #false) ;Caso en que el circulo central sea verde
(check-expect (stop? (make-sysSatelite 0 "yellow")) #false) ;Caso en que el circulo central sea amarillo
(check-expect (stop? (make-sysSatelite 0 "black")) #true) ;Caso en que el circulo central sea negro

(big-bang INICIAL
  [to-draw interpretar]
  [on-tick manejadorOnTick]
  [on-key manejadorTeclado]
  [stop-when stop? interpretar])