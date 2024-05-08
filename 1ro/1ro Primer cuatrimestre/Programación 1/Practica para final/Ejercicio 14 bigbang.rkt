;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname |Ejercicio 14 bigbang|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp") (lib "batch-io.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp") (lib "batch-io.rkt" "teachpack" "2htdp")) #f)))

(define-struct canvas[Escena Color Forma])

(define ANCHO 800);ancho de la escena
(define ALTO 500) ;alto de la escena
(define TAM 50) ;Tamaño de las figuras


;manejarOnMouse: Estado Number Number String -> Estado


(define (manejarOnMouse s x y k)
  (if (equal? k "button-down") (make-canvas (place-image (if (equal? (canvas-Forma s) "circle") (circle TAM "solid" (canvas-Color s)) (square TAM "solid" (canvas-Color s))) x y (canvas-Escena s)) (canvas-Color s) (canvas-Forma s)) s))

(define (interpretar s) (canvas-Escena s))

(define (manejarOnKey s k)
  (cond [(equal? k "r") (make-canvas (canvas-Escena s) "red" (canvas-Forma s))]
        [(equal? k "g") (make-canvas (canvas-Escena s) "green" (canvas-Forma s))]
        [(equal? k "b") (make-canvas (canvas-Escena s) "blue" (canvas-Forma s))]
        [(equal? k "s") (make-canvas (canvas-Escena s) (canvas-Color s) "square")]
        [(equal? k "c") (make-canvas (canvas-Escena s) (canvas-Color s) "circle")]
        [else s]
        ))



(define INICIAL
  (make-canvas (empty-scene ANCHO ALTO) "red" "circle"))

(big-bang INICIAL
  [to-draw interpretar]
  [on-mouse manejarOnMouse]
  [on-key manejarOnKey])






