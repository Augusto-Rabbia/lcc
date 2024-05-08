;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname E7-AugustoRabbia) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp") (lib "batch-io.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp") (lib "batch-io.rkt" "teachpack" "2htdp")) #f)))
;Una escuela de natación tiene un plan de entrenamiento y competencia de sus atletas que consiste en 100 metros de crol, 100 de mariposa y 100 de pecho.
;Para presentar sus candidates a las competencias nacionales, la escuela debe elegir sus dos mejores atletas.


(define-struct prueba [nombre crol mariposa pecho]) ;Para cada nadador, mantiene un registro de los tiempos de cada estilo
(define-struct promedios [nombre prom]) ;Promedios de cada nadador

(define LISTA-ATLETAS
  (list (make-prueba "Ana" 5 10 15)
        (make-prueba "Jorge" 7 13 15)
        (make-prueba "Khan" 8 10 15)
        (make-prueba "Pablo" 9 12 10)
        (make-prueba "Andrés" 10 12 17))) ;Lista de atletas

; promedio: prueba -> promedios
; Toma las pruebas de cada nadador y devuelve el promedio de estas.
(define (promedio atleta)
  (/ (+ (prueba-crol atleta) (prueba-mariposa atleta) (prueba-pecho atleta)) 3))


; 1.

; mejor: List -> Number
; Al entregársele la lista de nadadores, devuelve el promedio del mejor nadador
(define (mejor l)
  (cond [(empty? l) 100]
        [(cons? l) (min (promedio (first l)) (mejor (rest l)))]))

(check-expect (mejor LISTA-ATLETAS) 10)

; mejor1: List -> List
; Al entregársele la lista de nadadores, devuelve una lista con el nombre del nadador con el mejor promedio.
(define (mejor1 l)
  (cond [(empty? l) '()]
        [(cons? l) (if (= (promedio (first l)) (mejor l)) (cons (prueba-nombre (first l)) '()) (mejor1 (rest l)))]))

(check-expect (mejor1 LISTA-ATLETAS) (list "Ana"))




;2.
; mejor-T: List -> Number
; Al entregársele la lista de nadadores, devuelve el nombre del nadador con el mejor promedio.

(define (mejor-T l)
  (foldr min 100 (map promedio l)))

(check-expect (mejor-T LISTA-ATLETAS) 10)

; mejorprom?: promedio -> Boolean
; Auxiliar, toma el promedio  de un atleta y lo compara con el mejor tiempo
(define (mejorprom? a)
  (= (promedio a) (mejor-T LISTA-ATLETAS)))

; Obtener-n: prueba -> String
; Auxiliar, toma la estructura prueba de un atleta y devuelve su nombre.
(define (obtener-n a)
  (prueba-nombre a))

; mejor1: List -> List
; Al entregársele la lista de nadadores, devuelve una lista con el nombre del nadador con el mejor promedio. 
(define (mejor-1 l)
  (map obtener-n (filter mejorprom? l)))

(check-expect (mejor-1 LISTA-ATLETAS) (list "Ana"))

