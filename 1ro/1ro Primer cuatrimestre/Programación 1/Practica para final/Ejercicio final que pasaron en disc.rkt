;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname |Ejercicio final que pasaron en disc|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp") (lib "batch-io.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp") (lib "batch-io.rkt" "teachpack" "2htdp")) #f)))


(map sin (map cos (map sqr (list pi 0 (/ pi 6) (/ pi 2)))))
(append (map sin (map sqr (list pi 0 (/ pi 6) (/ pi 2)))) (map cos (map sqr (list pi 0 (/ pi 6) (/ pi 2)))))

(define (sumposn p1 p2)
  (make-posn (+ (posn-x p1) (posn-x p2)) (+ (posn-y p1) (posn-y p2))))

(foldr sumposn (make-posn 0 0) (list (make-posn 3 4) (make-posn 7 8) (make-posn -8 11)))

(define (ancho=alto? img)
  (= (image-width img) (image-height img)))

(foldr overlay (empty-scene 0 0) (filter ancho=alto? (list (square 10 "solid" "blue") (rectangle 35 15 "solid" "red") (circle 18 "solid" "yellow"))))


(define (overlay2 img1 img2)
  (place-image img2 (/ (image-width img1) 2) (/ (image-height img1) 2) img1))

(foldr overlay2 (empty-scene 0 0) (filter ancho=alto? (list (square 10 "solid" "blue") (rectangle 35 15 "solid" "red") (circle 18 "solid" "yellow"))))


(define (cant-negativos l)
  (cond [(empty? l) 0]
        [(cons? l) (if (negative? (first l)) (+ 1 (cant-negativos (rest l))) (cant-negativos (rest l)))]))

(check-expect (cant-negativos (list -1 2 0 -3 7 -7)) 3)

(define(todos-positivos? l)
  (cond [(empty? l) #t]
        [(cons? l) (and (positive? (first l)) (todos-positivos? (rest l)))]))

(check-expect (todos-positivos? (list -1 2 0 -3 7 -7)) #false)


(define-struct cahorro [titular saldo limite r-trans])
;Interpretación:
;Un Cahorro (make-Cahorro String Number Number List(Number))
;corresponde a la información disponible sobre una caja de ahorro de un cliente de un banco.
;Donde ,
;-titular: Es el nombre del titular , el dueño, de la cuenta.
;-saldo: el saldo disponible en la cuenta.
;-limite: el límite en el valor a extraer de la cuenta.
;-r-trans: es una lista de números que almacena los depósitos (valores positivos) y las extracciones (valores negativos)
;realizados sobre la cuenta pero aún no efectivizados (aplicado) al saldo de la cuenta.


(define CJULIO
  (make-cahorro "Julio" 20365 5000
                (list -2500 850 -1000 -112)))
(define CJOSE
  (make-cahorro "Josefina" 35222 10000
                (list -22800 -9582 1000 -7458 -568 1000)))
(define CJUANA
  (make-cahorro "Juana" 7458 1000
                (list -728 -253 -185 -547 -125)))

(define (suma-extracciones l)
  (cond [(empty? l) 0]
        [(cons? l) (if (negative? (first l)) (+ (first l) (suma-extracciones (rest l))) (suma-extracciones (rest l)))]))

(define (total-extraido Ca)
  (- (suma-extracciones (cahorro-r-trans Ca))))

(check-expect (total-extraido CJULIO) 3612)

(define (suma-depositos l)
  (cond [(empty? l) 0]
        [(cons? l) (if (positive? (first l)) (+ (first l) (suma-depositos (rest l))) (suma-depositos (rest l)))]))

(define (total-depositado Ca)
  (suma-depositos (cahorro-r-trans Ca)))


(check-expect (total-depositado CJULIO) 850)

;Ej 6
(define (total-movimientos l)
  (cond [(empty? l) 0]
        [(cons? l) (+ (first l) (total-movimientos (rest l)))]))

(define (actualiza-saldo Ca)
  (make-cahorro (cahorro-titular Ca) (+ (cahorro-saldo Ca) (total-movimientos (cahorro-r-trans Ca))) (cahorro-limite Ca) empty))
                         
(check-expect (actualiza-saldo (make-cahorro "Julio" 20365 5000 (list -2500 850 -1000 -112))) (make-cahorro "Julio" 17603 5000 empty))

;Ej 7

(define (operaciones Ca tipo n)
  (if (and (tipo-valido? tipo) (cahorro? Ca) (number? n)) 
      (cond [(= 0 tipo) (modificar-r-trans Ca n)]
            [(= 1 tipo) (modificar-r-trans Ca (- n))]
            [(= 2 tipo) (make-cahorro (cahorro-titular Ca) (cahorro-saldo Ca) n (cahorro-r-trans Ca))])
      "Código Inválido"))

(define (tipo-valido? x)
  (or (equal? x 0) (equal? x 1) (equal? x 2)))

(define (modificar-r-trans Ca n)
  (make-cahorro (cahorro-titular Ca) (cahorro-saldo Ca) (cahorro-limite Ca) (append (cahorro-r-trans Ca) (cons n '()))))

(check-expect (operaciones (make-cahorro "Julio" 15903 5000 empty) 0 1000) (make-cahorro "Julio" 15903 5000 (list 1000)))
(check-expect (operaciones (make-cahorro "Julio" 15903 5000 (list 1000)) 1 700) (make-cahorro "Julio" 15903 5000 (list 1000 -700)))
(check-expect (operaciones (make-cahorro "Julio" 15903 5000 (list 1000 -700)) 2 3000) (make-cahorro "Julio" 15903 3000 (list 1000 -700)))



                         



