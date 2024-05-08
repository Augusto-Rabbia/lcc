;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Practica 3 Ej 6|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
;Práctica 3 - Ejercicio 6


(define TAM_TXT 20) ;Tamaño del texto

(define LINEAS 3) ;Permite fácilmente cambiar la cantidad de las líneas
(define ALTO (* LINEAS TAM_TXT)) ;Alto de la escena
(define ANCHO 800) ;Ancho de la escena
(define ESCENA (empty-scene ANCHO ALTO)) ;Escena

(define (interpretar s) (place-image/align (text s TAM_TXT "indigo")
                                           0 0 "left" "top" ESCENA))


(define (string-remove-last str)
  [substring str 0 (- (string-length str) 1)])

(define (manejadorTeclado s k)
  (cond  [(equal? k "\b") (string-remove-last s)]
         [(< 1 (string-length k)) s]
         [else (string-append s k)]))


(define INICIAL "")

(big-bang INICIAL
  [to-draw interpretar]
  [on-key manejadorTeclado])