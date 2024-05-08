;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Practica 3 Ej 8|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
;Práctica 3 - Ejercicio 8


(define ANCHO 800)
(define ALTO 400)
(define ESCENA (empty-scene ANCHO ALTO))

(define (interpretar s) s)


(define (manejadorMouse s x y click)
  (cond [(and (string=? click "button-down") (< (+ x (/ x 10)) ANCHO) (< (+ y (/ x 10)) ALTO) (> (- x (/ x 10)) 0) (> (- y (/ x 10)) 0))
         (place-image (star (/ x 10) "solid" "blue") x y s)]
         [else s]))

(define (manejadorTeclado s k)
  (if (equal? k "\b") ESCENA s))

(big-bang ESCENA
  [to-draw interpretar]
  [on-mouse manejadorMouse]
  [on-key manejadorTeclado])
