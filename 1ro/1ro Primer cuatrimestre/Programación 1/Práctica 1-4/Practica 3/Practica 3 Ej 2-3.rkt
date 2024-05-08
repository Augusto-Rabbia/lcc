;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Practica 3 Ej 2-3|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
;Práctica 2 - Ej 2-3

(define RADIO_INICIAL 100)

(define DIMENSIONES 300)
(define ESCENA (empty-scene DIMENSIONES DIMENSIONES))


(define CRECIMIENTOxTICK 1)
(define TICKSxSEG 20)
(define TIMER (/ 1 TICKSxSEG))

;interpretar: Number -> Image
;Recibe el tamaño del
(define (interpretar s)
  (place-image (circle s "solid" (cond [(< s 51) "blue"]
                                       [(< s 101) "red"]
                                       [else "black"]))
               (/ DIMENSIONES 2) (/ DIMENSIONES 2)
               ESCENA))


;manejadorOnTick: Number -> Number
;Recibe el tamaño del circulo y devuelve un circulo con un tamaño mayor en 1.
;Si el circulo llega a tocar los bordes de la escena, su tamaño se reiniciará y será 0.
(define (manejadorOnTick s)
  (if (> s (- (/ DIMENSIONES 2) 1)) 0 (+ s CRECIMIENTOxTICK)))

(define (manejadorTeclado s k)
  (cond [(number? (string->number k)) (* (string->number k) 5)]
        [(string=? k "down") 1001]
        [else s]))


(define (COND_FINISH s)
  (if (< 1000 s) #true #false))


(big-bang RADIO_INICIAL
  [to-draw interpretar]
  [on-tick manejadorOnTick TIMER]
  [on-key manejadorTeclado]
  [stop-when COND_FINISH])





