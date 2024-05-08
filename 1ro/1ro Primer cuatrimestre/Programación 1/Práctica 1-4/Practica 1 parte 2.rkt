;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Practica 2|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
;------------
"Ejercicio 4"

(define ANCHO 100)
(define ALTO 100)

(define rectángulo (rectangle ANCHO ALTO "solid" "red"))
(place-image rectángulo 200 150 (empty-scene 400 300))


(define Clasificar_Figura
  (cond [(< ANCHO (/ ALTO 2)) "muy flaca"]
        [(< ALTO (/ ANCHO 2)) "muy gorda"]
        [(< ANCHO ALTO) "flaca"]
        [(< ALTO ANCHO) "gorda"]
        [else "cuadrada"]))

Clasificar_Figura

;------------
"Ejercicio 5"

(define TEMP -5)

(define reportaje
  (cond [(< TEMP 0) "No Olvidar Bufanda (NOB)"]
        [(< 30 TEMP) "Realmente Caluroso (RC)" ]
        [(<= TEMP 15) "Frío (F)"]
        [(< 15 TEMP) "Agradable (A)"]))
reportaje

;------------
"Ejercicio 6"

(define (sgn2 x) (cond [(< x 0) -1]
                       [(= x 0) 0]
                       [(> x 0) 1]))


(define (sgn3 x) (cond [(number? x) (sgn2 x)]
                       [(string? x) (sgn2 (string->number x))]
                       [(boolean? x) (if (boolean=? x #false) 0 1)]))



(sgn3 #true)

;------------
"Ejercicio 7-8"

(define ANCHO7 40)
(define ALTO7 70)
(define Rectangulo7 (rectangle ANCHO7 ALTO7 "outline" "red"))


(define (sgn4 x) (cond [(number? x) (sgn2 x)]
                       [(string? x) (if (number? (string->number x)) (sgn2 (string->number x)) "La cadena no se puede convertir a un número")]
                       [(boolean? x) (if (boolean=? x #false) 0 1)]
                       [(image? x) (cond [(< (image-width x) (image-height x)) 1]
                                         [(< (image-height x) (image-width x)) -1]
                                         [else 0])]
                       [else "clase no soportada por la función"]))
(sgn4 Rectangulo7)







