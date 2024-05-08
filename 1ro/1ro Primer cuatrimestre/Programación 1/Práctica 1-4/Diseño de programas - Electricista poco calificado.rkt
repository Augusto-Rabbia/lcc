;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Diseño de programas - Electricista poco calificado|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp")) #f)))




(check-expect (luces #true #true #true) "#false #true #false")

(check-expect (luces #true #true #false) "#false #false #true")
(check-expect (luces #true #false #true) "#false #true #false")
(check-expect (luces #false #true #true) "#true #false #false")

(check-expect (luces #false #false #false) "#true #true #false")

(check-expect (luces #true #false #false) "#true #false #false")
(check-expect (luces #false #true #false) "#false #true #false")
(check-expect (luces #false #false #true) "#false #false #true")

(define (imprimir x1 x2 x3)
                  (string-append (boolean->string x1) " "
                                         (boolean->string x2) " "
                                         (boolean->string x3)))

(define (luces l1 l2 l3)
  (cond [(and l1 l2 l3)
      (imprimir (not l1) l2 (not l3))]
      [(or (and (not l1) l2 l3)
           (and l1 (not l2) l3)
           (and l1 l2 (not l3)))
                (imprimir (not l1) (not l2) (not l3))]
      [(and (not l1) (not l2) (not l3))
       (imprimir (not l1) (not l2) l3)]
      (else (imprimir l1 l2 l3))))
