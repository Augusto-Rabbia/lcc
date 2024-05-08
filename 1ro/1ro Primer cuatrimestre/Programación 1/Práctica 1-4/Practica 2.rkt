;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Practica 2-2|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
;Práctica 2

;-----------
" "
"Ejercicio 1"
" "

(define (distancia-origen x y)
  (sqrt (+ (expt x 2) (expt y 2))))

(distancia-origen 1 1)

;-----------
" "
"Ejercicio 2"
" "

(define (distancia-puntos x y a b)
  [sqrt (+ (expt (- x a) 2) (expt (- y b) 2))])

(distancia-puntos -10 -3 -4 5)

;-----------
" "
"Ejercicio 3-4"
" "
(define (vol-cubo L)
  [expt L 3])
(define (area-cubo L)
  [* 6 (expt L 2)])
(vol-cubo 3)
(area-cubo 3)

;-----------
" "
"Ejercicio 5-6-7"
" "
(define (string-insert str i)
  [string-append (substring str 0 i) "-" (substring str i)])
(string-insert "lavamanos" 4)
;-----------
(define (string-last str)
  [substring str (- (string-length str) 1)])
(string-last "lavamanos")
;-----------
(define (string-remove-last str)
  [substring str 0 (- (string-length str) 1)])
(string-remove-last "lavamanos")

;-----------
" "
"Ejercicio 8"
" "
(check-expect (string-remove-last "lavamanos") "lavamano")
(check-expect (string-last "lavamanos") "s")
(check-expect (string-insert "lavamanos" 4) "lava-manos")
(check-expect (vol-cubo 3) 27)
(check-expect (area-cubo 3) 54)

;-----------
" "
"Ejercicio 9"
" "
;La cantidad de personas y meses se representarán como números
;El descuento se representará como un valor que va del 0 al 1 y representan el % de descuento.

(define CUOTA 650) ;Cuota mensual por persona
(define DESCTO_MAXIMO 0.35) ; Descuento máximo aplicable a la cuota mensual.
(define DESCTO_PERSONAS_3_O_MAS 0.2); Descuento correspondiente a la promoción por personas, cuando se anotan 3 o más.
(define DESCTO_PERSONAS_2 0.1) ; Descuento correspondiente a la promoción por personas, cuando se anotan 2.
(define DESCTO_MESES_3_O_MAS 0.25) ; Descuento correspondiente a la promoción por meses, cuando se cancelan 3 o más.
(define DESCTO_MESES_2 0.15) ; Descuento correspondiente a la promoción por meses, cuando se cancelan 2.

; Obtiene el descuento de la promoción por persona.
; Number -> Number
(check-expect (descuento-personas 3) 0.2)
(check-expect (descuento-personas 1) 0)
(check-expect (descuento-personas 2) 0.1)

(define (descuento-personas Personas)
  (cond [(< 2 Personas) DESCTO_PERSONAS_3_O_MAS]
        [(< 1 Personas) DESCTO_PERSONAS_2]
        [else 0]))

; Obtiene el descuento de la promoción por mes pagado.
; Number -> Number
(check-expect (descuento-meses 3) 0.25)
(check-expect (descuento-meses 1) 0)
(check-expect (descuento-meses 2) 0.15)

(define (descuento-meses Meses)
  (cond [(< 2 Meses) DESCTO_MESES_3_O_MAS]
        [(< 1 Meses) DESCTO_MESES_2]
        [else 0]))

; Obtiene el total del descuento aplicado en cada caso.
; Number Number -> Number
(check-expect (descuento-total 3 3) 0.35)
(check-expect (descuento-total 2 1) 0.15)
(check-expect (descuento-total 2 2) 0.25)
(check-expect (descuento-total 3 1) 0.25)
(check-expect (descuento-total 1 3) 0.2)

(define (descuento-total Meses Personas)
  (min (+ (descuento-meses Meses) (descuento-personas Personas)) DESCTO_MAXIMO))

; Obtiene el precio total a pagar por persona una vez aplicado el descuento.
; Number Number -> Number
(check-expect (monto-persona 2 2) 975) ;Pedro y Juan 2 meses: 975
(check-expect (monto-persona 3 3) 1267.5) ;Pedro Paula y Juan 3 meses
(check-expect (monto-persona 5 1) 2437.5) ;José 5 meses

(define (monto-persona Meses Personas)
  (-(* Meses CUOTA) (* (* Meses CUOTA) (descuento-total Meses Personas))))


;-----------
" "
"Ejercicio 9"
" "
;La edad de una persona expresada en meses y la hemoglobina en sangre expresada con un valor numérico que representa los g/dl
;La función devolverá #true si la persona tiene anemia y #false si no tiene.

(define HEMOG-<1M 13) ;Nivel mínimo de hemoglobina normal con hasta un mes.
(define HEMOG-<6M 10) ;Nivel mínimo de hemoglobina normal con hasta 6 meses.
(define HEMOG-<12M 11) ;Nivel mínimo de hemoglobina normal con hasta 12 meses.
(define HEMOG-<60M 11.5) ;Nivel mínimo de hemoglobina normal con hasta 60 meses.
(define HEMOG-<120M 12.6) ;Nivel mínimo de hemoglobina normal con hasta 120 meses.
(define HEMOG-+120M 13) ;Nivel mínimo de hemoglobina normal con más de 120 meses.

;anemia: Number Number -> Boolean
;Muestra si una persona tiene anemia según su edad y hemoglobina en sangre.
(check-expect(anemia 0 12.9) #true)
(check-expect(anemia 180 13) #false)
(check-expect(anemia 6 9.5) #true)
(check-expect(anemia 65 12.3) #true)
(check-expect(anemia 15 14) #false)

(define (anemia Edad Hemog)
  (cond[(<= 120 Edad) (if (< Hemog HEMOG-+120M) #true #false)]
       [(<= 60 Edad) (if (< Hemog HEMOG-+120M) #true #false)]
       [(<= 12 Edad) (if (< Hemog HEMOG-<60M) #true #false)]
       [(<= 6 Edad) (if (< Hemog HEMOG-<12M) #true #false)]
       [(<= 1 Edad) (if (< Hemog HEMOG-<6M) #true #false)]
       [(<= 0 Edad) (if (< Hemog HEMOG-<1M) #true #false)]))















