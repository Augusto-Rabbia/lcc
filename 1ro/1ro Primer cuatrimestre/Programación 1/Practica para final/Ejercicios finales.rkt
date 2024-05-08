;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname |Ejercicios finales|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp") (lib "batch-io.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp") (lib "batch-io.rkt" "teachpack" "2htdp")) #f)))
;Ej 5

;replicar: list Number -> List
;replica los elementos de la lista la cantidad de veces que se deseen
(define (replicar l n)
  (cond [(empty? l) '()]
        [(cons? l) (append (make-list n (first l)) (replicar (rest l) n))]))

(check-expect (replicar (list 4 6 3 9) 3) (list 4 4 4 6 6 6 3 3 3 9 9 9))

;Ej 6

(define-struct pedidos[Nom Menu Postre])

(define-struct pedido-final[M1 M2 M3 P1 P2])

(define (armar-PF l)
  (cond [(empty? l) 1]
        [(cons? l) (cond [(= 1 (pedidos-Menu (first l))) (if (= 1 (pedidos-Postre (first l))) (+ 10000000000010000000 (armar-PF (rest l))) (+ 10000000000000001000 (armar-PF (rest l))))]
                         [(= 2 (pedidos-Menu (first l))) (if (= 1 (pedidos-Postre (first l))) (+ 00001000000010000000 (armar-PF (rest l))) (+ 00001000000000001000 (armar-PF (rest l))))]
                         [else (if (= 1 (pedidos-Postre (first l))) (+ 00000000100010000000 (armar-PF (rest l))) (+ 00000000100000001000 (armar-PF (rest l))))]
                         )
                   ]))

(define (pedido-a-final ind fin)
  (cond [(= 1 (pedidos-Menu ind)) (if (= 1 (pedidos-Postre ind))
                                             (make-pedido-final (+ 1 (pedido-final-M1 fin)) (pedido-final-M2 fin) (pedido-final-M3 fin) (+ 1 (pedido-final-P1 fin)) (pedido-final-P2 fin))
                                             (make-pedido-final (+ 1 (pedido-final-M1 fin)) (pedido-final-M2 fin) (pedido-final-M3 fin) (pedido-final-P1 fin) (+ 1 (pedido-final-P2 fin))))]
               [(= 2 (pedidos-Menu ind)) (if (= 1 (pedidos-Postre ind))
                                             (make-pedido-final (pedido-final-M1 fin) (+ 1 (pedido-final-M2 fin)) (pedido-final-M3 fin) (+ 1 (pedido-final-P1 fin)) (pedido-final-P2 fin))
                                             (make-pedido-final (pedido-final-M1 fin) (+ 1 (pedido-final-M2 fin)) (pedido-final-M3 fin) (pedido-final-P1 fin) (+ 1 (pedido-final-P2 fin))))]
               [(= 3 (pedidos-Menu ind)) (if (= 1 (pedidos-Postre ind))
                                             (make-pedido-final (pedido-final-M1 fin) (pedido-final-M2 fin) (+ 1 (pedido-final-M3 fin)) (+ 1 (pedido-final-P1 fin)) (pedido-final-P2 fin))
                                             (make-pedido-final (pedido-final-M1 fin) (pedido-final-M2 fin) (+ 1 (pedido-final-M3 fin)) (pedido-final-P1 fin) (+ 1 (pedido-final-P2 fin))))]))
                                                   

;Ej 7

(define (sublistas-vacias? l)
  (cond [(empty? l) #t]
        [(cons? l) (and (empty? (first l)) (sublistas-vacias? (rest l)))]))

(check-expect (sublistas-vacias? (list '() '() '())) #true)
(check-expect (sublistas-vacias? (list '() (list 5 7))) #false)


;Ej 8
(define-struct trabajador [Apellido Estado Hijos Sueldo])

(define (impuesto t)
  (if (not (trabajador? t)) "Tipo de dato invalido"
      (cond [(>= (trabajador-Hijos t) 3) 0]
            [(= (trabajador-Hijos t) 2) (if (or (and (equal? (trabajador-Estado t) "casado") (< (trabajador-Sueldo t) 20000)) (< (trabajador-Sueldo t) 17000)) 0 (* 0.05 (trabajador-Sueldo t)))]
            [(= (trabajador-Hijos t) 1) (if (or (and (equal? (trabajador-Estado t) "casado") (< (trabajador-Sueldo t) 19000)) (< (trabajador-Sueldo t) 16000)) 0 (* 0.05 (trabajador-Sueldo t)))]
            [else (if (or (and (equal? (trabajador-Estado t) "casado") (< (trabajador-Sueldo t) 18000)) (< (trabajador-Sueldo t) 15000)) 0 (* 0.05 (trabajador-Sueldo t)))])))

(check-expect (impuesto (make-trabajador "a" "casado" 5 100000)) 0)
(check-expect (impuesto (make-trabajador "a" "casado" 2 100000)) 5000)
(check-expect (impuesto (make-trabajador "a" "casado" 1 10000)) 0)
(check-expect (impuesto (make-trabajador "a" "casado" 0 100000)) 5000)
(check-expect (impuesto (make-trabajador "a" "casado" 0 10)) 0)
(check-expect (impuesto 15) "Tipo de dato invalido")

;Ej 9

(define (subcadena? l str)
  (cond [(empty? l) '()]
        [(cons? l) (if (string-contains? str (first l))
                       (cons (first l) (subcadena? (rest l) str))
                       (subcadena? (rest l) str))]))

(check-expect (subcadena? (list "cadena" "Helena" "casa") "ena") (list "cadena" "Helena"))
(check-expect (subcadena? (list "Hola" "mundo") "eso") '())
(check-expect (subcadena? '() "altos") '())

;Ej 10

(define (shortest l)
  (cond [(empty? l) (replicate 300 "a")]
        [(cons? l) (if (< (string-length (first l)) (string-length (shortest (rest l)))) (first l) (shortest (rest l)))]))

(check-expect (shortest (list "aaaaa" "aa" "aaa")) "aa")

(define (longest l)
  (cond [(empty? l) ""]
        [(cons? l) (if (> (string-length (first l)) (string-length (longest (rest l)))) (first l) (longest (rest l)))]))

(check-expect (longest (list "aaaaa" "aa" "aaa")) "aaaaa")

(define (shortest-longest l)
  (if (empty? l) "La lista es vacía" (make-posn (shortest l) (longest l))))

(check-expect (shortest-longest (list "a" "acb" "b" "xf" "asdf")) (make-posn "b" "asdf"))
(check-expect (shortest-longest (list "a")) (make-posn "a" "a"))


;Ej 11

(define-struct person[W H]) ;1.

(define (IMC p)
  (/ (person-W p) (sqr (person-H p))))

(define (cant-elementos l)
  (cond [(empty? l) 0]
        [(cons? l) (+ 1 (cant-elementos (rest l)))]))

(check-expect (cant-elementos (list 1 2)) 2)

(define (list->IMC l)
  (cond [(empty? l) '()]
        [(cons? l) (cons (IMC (first l)) (list->IMC (rest l)))]))

(check-expect (list->IMC (list (make-person 1 1) (make-person 2 2))) (list 1 0.5))

(define (sum-elementos l)
  (cond [(empty? l) 0]
        [(cons? l) (+ (first l) (sum-elementos (rest l)))]))

(check-expect (sum-elementos (list 1 2 3 4)) 10)

(define (prom-imc l) ;2.
  (/ (sum-elementos (list->IMC l)) (cant-elementos l)))

(check-within (prom-imc (list (make-person 44 1.55) (make-person 50 1.56))) 19.43 0.01)

(define (clasificacion p)
  (cond [(< 30 (IMC p)) "Obesidad"]
        [(< 25 (IMC p)) "Sobrepeso"]
        [(< 18.5 (IMC p)) "Peso Normal"]
        [else "Bajo Peso"]))

(define (clasif l) ;3
  (cond [(empty? l) '()]
        [(cons? l) (cons (clasificacion (first l)) (clasif (rest l)))]))

(check-expect (clasif (list (make-person 44 1.55) (make-person 50 1.56))) (list "Bajo Peso" "Peso Normal"))

(define (saludable? p)
  (<= 18.5 (IMC p) 25))

(check-expect (saludable? (make-person 50 1.56)) #t)
(check-expect (saludable? (make-person 50 2.06)) #f)


(define (saludables l)
  (cond [(empty? l) '()]
        [(cons? l) (if (saludable? (first l)) (cons (first l) (saludables (rest l))) (saludables (rest l)))]))
                       
(check-expect (saludables (list (make-person 50 1.56) (make-person 50 2.06))) (list (make-person 50 1.56)))

(define (poblacion-sana l) ;4
  (< 0.8 (/ (cant-elementos (saludables l)) (cant-elementos l))))

(check-expect (poblacion-sana (list (make-person 50 1.56) (make-person 50 2.06))) #f)
(check-expect (poblacion-sana (list (make-person 50 1.56) (make-person 70 1.9))) #t)

 










  
                         