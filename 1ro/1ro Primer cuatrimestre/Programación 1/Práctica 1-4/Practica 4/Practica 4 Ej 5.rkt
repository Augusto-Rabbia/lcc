;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Practica 4 Ej 5|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))


(define ANCHO 600)
(define ALTO 400)
(define ESCENA (empty-scene ANCHO ALTO))

(define CUADRADO (square 50 "solid" "red"))
(define VEL 40)

(define (interpretar s)
  (place-image CUADRADO (posn-x s) (posn-y s) ESCENA))

(define (manejadorTeclado s k)
  (cond [(and (equal? k "up") (> (- (posn-y s) VEL) 0)) (make-posn (posn-x s) (- (posn-y s) VEL))]
        [(and (equal? k "down") (< (+ (posn-y s) VEL) ALTO)) (make-posn (posn-x s) (+ (posn-y s) VEL))]
        [(and (equal? k "right") (< (+ (posn-x s) VEL) ANCHO)) (make-posn (+ (posn-x s) VEL) (posn-y s))]
        [(and (equal? k "left") (> (- (posn-x s) VEL) 0)) (make-posn (- (posn-x s) VEL) (posn-y s))]
        [(equal? k " ") INICIAL]
        [else s]))

(define INICIAL (make-posn (/ ANCHO 2) (/ ALTO 2)))

(big-bang INICIAL
  [to-draw interpretar]
  [on-key manejadorTeclado])