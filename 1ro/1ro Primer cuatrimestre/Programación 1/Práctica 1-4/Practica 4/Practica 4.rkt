;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Practica 4|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
;Práctica 4

;-----------
" "
"Ejercicio 2"
" "
(define (dist-origen Punto)
  (sqrt (+ (expt (posn-x Punto) 2) (expt (posn-y Punto) 2))))


(+ (dist-origen (make-posn 12 5)) 4)

;-----------
" "
"Ejercicio 3"
" "
(define (simetrico Punto)
  (make-posn (- (posn-x Punto)) (- (posn-y Punto))))

(simetrico (make-posn 10 5))

;-----------
" "
"Ejercicio 4"
" "
(define (dist-origen2 Punto1 Punto2)
  (if (and (posn? Punto1) (posn? Punto2))
  (sqrt (+ (expt (- (posn-x Punto1) (posn-x Punto2)) 2) (expt (- (posn-y Punto1) (posn-y Punto2)) 2)))
  "Tipos incorrectos para la función."))

(dist-origen2 (make-posn 10 50) (make-posn 10 100))
(dist-origen2 #false 5)







