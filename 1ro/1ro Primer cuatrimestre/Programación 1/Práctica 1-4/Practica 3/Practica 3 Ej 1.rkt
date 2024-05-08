;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Practica 3 Ej 1|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
;Práctica 3 - Ejercicio 1

(define TAM 50)

(define INICIAL "blue")

(define ALTO 300)
(define ANCHO 500)
(define ESCENA (empty-scene ANCHO ALTO))


(define (interpretar s)
  (place-image (circle 30 "solid" s) (/ ANCHO 2) (/ ALTO 2) ESCENA))

(define (manejadorOnTick s)
  (cond [(string=? s "blue") "red"]
        [(string=? s "red") "brown"]
        [(string=? s "brown") "purple"]
        [(string=? s "purple") "yellow"]
        [(string=? s "yellow") "blue"]
        [else "blue"]))

(define (manejadorTeclado s k)
  (cond [(key=? k "n") "black"]
        [(key=? k "down") INICIAL]
        [else s]))



(big-bang INICIAL
  [to-draw interpretar]
  [on-tick manejadorOnTick 2]
  [on-key manejadorTeclado])






