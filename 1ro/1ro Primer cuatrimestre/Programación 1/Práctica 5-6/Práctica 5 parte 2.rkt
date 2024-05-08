;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname |Práctica 5 parte 2|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
;Práctica 5 - Parte 2

;-------------------------------------------------------------------------------------------------------------
;2 Aplicando una transformación a los elementos de una lista
;-----------
;Ejercicio 1

(define (pares l)
  (filter even? l))

(check-expect (pares (list 3 5 1 3 2)) (list 2))
(check-expect (pares (list 4 6 3 7 5 0)) (list 4 6 0))

;--
(define (str-corto? str)
  (if (< (string-length str) 5) #t #f))

(define (cortas l)
  (filter str-corto? l))

(check-expect (cortas (list "Lista" "de" "palabras" "sin" "sentido")) (list "de" "sin"))

;--
(define NUMERO 5) ;Número con el que se comparará en la lista
(define (numMayor? n)
  (if (> n NUMERO) #t #f)) ;#t si el número es mayor al determinado arriba

(define (mayores l)
  (filter numMayor? l))

(check-expect (mayores (list 1 5 2 9 60 0 3 6)) (list 9 60 6))

;--
(define MAX 5) ;Distancia máxima del origen
(define (cerca? p)
  (if (< (sqrt (+ (sqr (posn-x p)) (sqr (posn-y p)))) MAX) #t #f))

(define (cerca l)
  (filter cerca? l))
(check-expect (cerca (list (make-posn 3 5) (make-posn 1 2) (make-posn 0 1) (make-posn 5 6))) (list (make-posn 1 2) (make-posn 0 1)))

;-----------
;Ejercicio 2

(define (raices l)
  (map sqrt l))

(check-expect (raices (list 9 16 4)) (list 3 4 2))

;--
(define (distOrigen p)
  (sqrt (+ (sqr (posn-x p)) (sqr (posn-y p)))))

(define (distancias l)
  (map distOrigen l))

(check-expect (distancias (list (make-posn 3 4) (make-posn 0 4) (make-posn 12 5)))  (list 5 4 13))

;--
(define (anchos l)
  (map image-width l))

(check-expect (anchos (list (circle 30 "solid" "red") (rectangle 10 30 "outline" "blue"))) (list 60 10))

;--
(define (sgn2 x) (cond [(< x 0) -1]
                       [(= x 0) 0]
                       [(> x 0) 1]))

(define (signos l)
  (map sgn2 l))

(check-expect (signos (list 45 32 -23 0 12)) (list 1 1 -1 0 1))

;--
(define (longitudes l)
  (map string-length l))

(check-expect (longitudes (list "hola" "cómo" "estás?")) (list 4 4 6))

;--
(define (conversionFC F)
  (* (/ 5 9) (- F 32)))

(define (convertirFC l)
  (map conversionFC l))

(check-expect (convertirFC (list 32 50 104)) (list 0 10 40))

;-----------
;Ejercicio 3

(define (and2 a b)
  (and a b))

(define (todos-verdaderos l)
  (foldr and2 #t l))

(check-expect (todos-verdaderos (cons #true (cons #true (cons #true '())))) #true)
(check-expect (todos-verdaderos (cons #true (cons #false (cons #true '())))) #false)

(define (or2 a b)
  (or a b))


(define (uno-verdadero l)
  (foldr or2 #false l))

(check-expect (uno-verdadero (cons #true (cons #false (cons #false '())))) #true)
(check-expect (uno-verdadero (cons #false (cons #false (cons #false '())))) #false)

;--
(define (=1 o) 1)

(define (elementos->1 l)
  (map =1 l))

(define (sum a b)
  (+ a b))

(define (cant-elementos l)
  (foldr sum 0 (elementos->1 l)))

(check-expect (cant-elementos (list #t #t #f)) 3)
(check-expect (cant-elementos (cons #false '())) 1)

;--
(define (mult a b)
  (* a b))

(define (prod l)
  (foldr mult 1 l))

(check-expect (prod (list 1 2 3 4 5)) 120)

;--
(define (pegar l)
  (foldr string-append "" l))

(check-expect (pegar (list "Las " "lis" "tas " "son " "complicadas" ".")) "Las listas son complicadas.")

;--
(define (max2 a b)
  (max a b))

(define (maximo l)
  (foldr max2 -999999999999 l))

(check-expect (maximo (list 23 543 325 0 75)) 543)

;-----------
;Ejercicio 5
(define (cuadrados l)
  (map sqr l))
(define (sumcuad l)
  (foldr sum 0 (cuadrados l)))

(check-expect (sumcuad (list 1 2 3 4 5)) 55)
(check-expect (sumcuad empty) 0)

;-----------
;Ejercicio 6
(define (sumdist l)
  (foldr sum 0 (distancias l)))

(check-expect (sumdist (list (make-posn 3 4) (make-posn 0 3))) 8)

;-----------
;Ejercicio 7
(define (Positivo? n)
  (if (> n 0) #t #f))
(define (soloPositivos l)
  (filter Positivo? l))

(define (multPos l)
  (foldr mult 1 (soloPositivos l)))

(check-expect (multPos (list -1 3 -2 4 0 1 -5)) 12)

;-----------
;Ejercicio 8
(define (valAbs n)
  (if (> 0 n) (- n) n))

(define (sumAbs l)
  (foldr sum 0 (map valAbs l)))

(check-expect (sumAbs (list 3 -2 4 0 1 -5)) 15)

;-----------
;Ejercicio 9
(define (noNegativo? n)
  (if (< n 0) #f #t))

(define (raicesdePositivos l)
  (map sqrt (filter noNegativo? l)))

(check-expect (raicesdePositivos (list 16 -4 9 0)) (list 4 3 0))

;-----------
;Ejercicio 10


;-----------
;Ejercicio 11
(define (SumaElementos l)
  (foldr sum 0 l))

(define (SumaElementosSublistas l)
  (map SumaElementos l))

(define (algun+? l)
  (foldr or2 #f (map Positivo? (SumaElementosSublistas l))))

(check-expect (algun+? (list (list 1 3 -4 -2) (list 1 2 3 -5) (list 4 -9 -7 8 -3))) #true)
(check-expect (algun+? (list empty (list 1 2 3))) #true)
(check-expect (algun+? (list (list -1 2 -3 4 -5) empty (list -3 -4))) #false)



























































