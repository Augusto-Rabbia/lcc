;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Crear banderas Practica 1-1|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
(define (Bandera C1 C2 C3 Sentido) (if (string=? Sentido "horizontal") (place-image (rectangle 90 20 "solid" C1)
                             45 10
                             (place-image (rectangle 90 20 "solid" C2)
                                          45 30
                                          (place-image (rectangle 90 20 "solid" C3)
                                                       45 50
                                                       (empty-scene 90 60))))
                                       (place-image (rectangle 30 60 "solid" C1)
                              15 30
                             (place-image (rectangle 30 60 "solid" C2)
                                          45 30
                                          (place-image (rectangle 30 60 "solid" C3)
                                                       75 30
                                                       (empty-scene 90 60))))))
(Bandera "black" "red" "yellow" "horizontal")



(define Sudan (place-image (rotate 270 (triangle 90 "solid" "green")) 15  30
                           (place-image (rectangle 90 20 "solid" "red") 45 10
                                        (place-image (rectangle 90 20 "solid" "white") 45 30
                                                     (place-image (rectangle 90 20 "solid" "black") 45 50
                                                                  (empty-scene 90 60))))))
Sudan

(define Brasil (place-image (circle 13 "solid" "blue") 45 30
                            (place-image (rotate 270 (triangle 45 "solid" "yellow")) 64  30
                                         (place-image (rotate 90 (triangle 45 "solid" "yellow")) 26  30
                                                      (place-image (rectangle 90 60 "solid" "green") 45 30
                                                                                (empty-scene 90 60))))))
Brasil

