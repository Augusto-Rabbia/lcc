;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname TP1-Apellido1-Apellido2-Apellido3) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "batch-io.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "batch-io.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))
#|
Trabajo Práctico 1: Programas interactivos con estructuras

Integrantes:
- [Rabbia, Augusto].
- [Spreutels, Manuel].
- [Llop, Agustín].
|#


#|
Estructura para representar el estado del programa.
- ball-x:  La posición en x de la pelota.
- ball-y:  La posición en y de la pelota.
- bar-y:   La posición en y de la barra.
- ball-vx: La componente x de la velocidad de la pelota.
- ball-vy: La componente y de la velocidad de la pelota.
- points:  La cantidad de puntos acumulados.
|#

(define-struct st [ball-x ball-y bar-y ball-vx ball-vy points])

; Alto de la escena
(define HEIGHT 400)

; Ancho de la escena
(define WIDTH 500)

; Estado inicial
(define START (make-st 100 100 (/ HEIGHT 2) 6 -6 0))

#|
Constantes asociadas a la barra
- BAR-VEL:    La velocidad de la barra.
- BAR-X:      La posición en x de la barra.
- BAR-HEIGHT: Altura de la barra.
- BAR-WIDTH:  Ancho de la barra.
- BAR:        Imagen de la barra.
|#
(define BAR-VEL 8)
(define BAR-X 20)
(define BAR-HEIGHT 50)
(define BAR-WIDTH 20)
(define BAR (rectangle BAR-WIDTH BAR-HEIGHT "solid" "red"))

#|
Constantes asociadas a la pelota
- BALL-RADIUS: Radio de la pelota.
- BALL:        Imagen de la pelota.
|#
(define BALL-RADIUS 20)
(define BALL (circle BALL-RADIUS "solid" "blue"))

; Fondo
(define BACKGROUND (empty-scene WIDTH HEIGHT))

#|
Constantes asociadas al puntaje
- FONT-SIZE:         Tamano de fuente del puntaje.
- PUNTOS-PARA-GANAR: Puntos necesarios para ganar.
|#
(define FONT-SIZE 20)
(define PUNTOS-PARA-GANAR 10)

; draw-points: st -> Image
; Toma un estado y retorna la imagen para la cantidad de puntos anotados
(define (draw-points s) (text (number->string (st-points s)) FONT-SIZE "indigo"))

; draw: COMPLETAR DISEÑO
; La función draw se encarga de dibujar el estado del sistema.
; Ubica a la pelota en la posición dada por el estado
; Ubica a la barra en su posición constante en X, y la dada por el estado en Y
; Ubica al puntaje (su límite derecho y no su centro) a 10 unidades del margen derecho.
; y a 10 unidades del margen superior (su límite superior)
; Si el puntaje y la pelota se superponen, el puntaje se debe seguir viendo
(define (draw s) (place-image (draw-points s) (- WIDTH 10) 10
                                           (place-image BALL (st-ball-x s) (st-ball-y s)
                                           (place-image BAR BAR-X (st-bar-y s)
                                                        BACKGROUND))))

; ball-next: COMPLETAR SIGNATURA
; Calcula la posición de la pelota ante un nuevo click del reloj,
; haciéndola rebotar y manteniéndola en los márgenes permitidos.
; Para ello, primero la mueve con "step" y luego corrige posibles rebotes con "bounce"

;step: Estado -> Estado
;Recibe el estado y mueve la pelota, en X e Y en base a la velocidad de la misma.
(define (step s) (make-st
   (+ (st-ball-x s) (st-ball-vx s))
   (+ (st-ball-y s) (st-ball-vy s))
   (st-bar-y s) (st-ball-vx s) (st-ball-vy s) (st-points s)))

; Hace rebotar la pelota en las paredes o la barra.
(define (bounce s)
  (bounce-y (bounce-x s)))

; bounce-x: st -> st
; Hace rebotar la pelota en la pared derecha o la barra.
(define (bounce-x s)
  (cond
      [(hit-bar? s)        (reflect-ball-x-bar s)]
      [(hit-right-wall? s) (reflect-ball-x-right s)]
      [else                s]
  )
)

; hit-bar? : st -> Boolean
; Decide si la pelota colisionó con la barra
(define (hit-bar? s)
  (and (< (st-ball-x s)  (+ BALL-RADIUS BAR-X (/ BAR-WIDTH 2))) (and (< (- (st-ball-y s) (st-bar-y s)) (+  BALL-RADIUS (/ BAR-HEIGHT 2))) (< (-(- (st-ball-y s) (st-bar-y s))) (+  BALL-RADIUS (/ BAR-HEIGHT 2))))))
(check-expect (hit-bar? (make-st (+ BAR-X (/ BAR-WIDTH 2) BALL-RADIUS) 200 200 -6 6 1)) #f)

; hit-right-wall? 
; Decide si la pelota colisionó con la pared derecha
(define (hit-right-wall? s)
  (> (st-ball-x s) (- WIDTH BALL-RADIUS)))
(check-expect (hit-right-wall? (make-st 481 200 100 6 6 1)) #t)

; bounce-y: st -> st
; Hace rebotar la pelota en la pared superior o inferior.
(define (bounce-y s)
  (cond
      [(hit-top-wall? s) (reflect-ball-y-top s)]
      [(hit-bot-wall? s) (reflect-ball-y-bot s)]
      [else              s]
  )
)

; hit-top-wall? 
; Decide si la pelota colisionó con la pared superior
(define (hit-top-wall? s)
  (< (st-ball-y s) BALL-RADIUS))

; hit-bot-wall? 
; Decide si la pelota colisionó con la pared inferior
(define (hit-bot-wall? s)
  (> (st-ball-y s) (- HEIGHT BALL-RADIUS)))

;reflect-ball-x-right: Estado -> Estado
;Esta función se encargará de convertir un estado con la pelota a una distancia del borde menor que
;su velocidad a uno con su velocidad invertida y a una distancia al borde equivalente a la diferencia entre su velocidad y la distancia inicial
(define (reflect-ball-x-right s)
  (make-st (- (st-ball-x s) (* (- (+ (st-ball-x s) BALL-RADIUS) WIDTH) 2))
           (st-ball-y s)
           (st-bar-y s)
           (-(st-ball-vx s))
           (st-ball-vy s)
           (st-points s)))

;reflect-ball-x-bar: Estado -> Estado
;Esta función se encargará de convertir un estado con la pelota a una distancia del borde menor que
;su velocidad a uno con su velocidad invertida y a una distancia al borde equivalente a la diferencia entre su velocidad y la distancia inicial
(define (reflect-ball-x-bar s)
  (make-st (- (st-ball-x s) (* 2 (- (- (st-ball-x s) BALL-RADIUS) (+ BAR-X (/ BAR-WIDTH 2)))))
           (st-ball-y s)
           (st-bar-y s)
           (-(st-ball-vx s))
           (st-ball-vy s)
           (add-point s)))

;reflect-ball-y-top: Estado -> Estado
;Esta función se encargará de convertir un estado con la pelota a una distancia del borde menor que
;su velocidad a uno con su velocidad invertida y a una distancia al borde equivalente a la diferencia entre su velocidad y la distancia inicial
(define (reflect-ball-y-top s)
  (make-st (st-ball-x s)
           (- (st-ball-y s) (* (- (st-ball-y s) BALL-RADIUS) 2))
           (st-bar-y s)
           (st-ball-vx s)
           (-(st-ball-vy s))
           (st-points s)))

;reflect-ball-y-bot: Estado -> Estado
;Esta función se encargará de convertir un estado con la pelota a una distancia del borde menor que
;su velocidad a uno con su velocidad invertida y a una distancia al borde equivalente a la diferencia entre su velocidad y la distancia inicial
(define (reflect-ball-y-bot s)
  (make-st (st-ball-x s)
           (- (st-ball-y s) (* (- (+ (st-ball-y s) BALL-RADIUS) HEIGHT) 2))
           (st-bar-y s)
           (st-ball-vx s)
           (-(st-ball-vy s))
           (st-points s)))

; ball-next: Estado -> Estado
; Calcula la posición de la pelota ante un nuevo click del reloj,
; haciéndola rebotar y manteniéndola en los márgenes permitidos.
; Para ello, primero la mueve con "step" y luego corrige posibles rebotes con "bounce"
(define (ball-next s)
  (bounce (step s)))

; add-point: st-> st
; Incrementa en 1 el puntaje
(define (add-point s)
  (+ 1 (st-points s)))

(check-expect (add-point (make-st 1 1 1 6 6 1000)) 1001)

; Caso cuando la pelota choca con la pared derecha
(check-expect (ball-next (make-st 475 200 100 6 6 1)) (make-st 479 206 100 -6 6 1))
; Caso cuando la pelota choca con la barra (+ BAR-X (/ BAR-WIDTH 2) BALL-RADIUS) = 50
(check-expect (ball-next (make-st 50 200 200 -6 6 1)) (make-st 56 206 200 6 6 2))

(define (handle-key s k)
  (cond
    [(key=? k "down") (move-bar s BAR-VEL)]
    [(key=? k "up") (move-bar s (- BAR-VEL))]
    [else (move-bar s (* BAR-VEL 0))]))

;(check-expect (handle-key (make-st 1 1 (- HEIGHT (/ BAR-VEL 2)) -1 -1 0) "down")
;              (make-st 1 1 (- HEIGHT (/ BAR-HEIGHT 2)) -1 -1 0))
;En nuestro código es imposible desde un principio que la barra llegue a esta posición ya que no se cumplen las condiciones para move-bar
(check-expect (handle-key (make-st 1 1 300 -1 -1 0) "down")
              (make-st 1 1 (+ 300 BAR-VEL) -1 -1 0))
(check-expect (handle-key (make-st 1 1 375 -1 -1 0) "down")
              (make-st 1 1 375 -1 -1 0))

; move-bar: st Number -> st
; Mueve la barra n unidades, manteniéndola en los márgenes permitidos.
(define (move-bar s n)
  (cond [(and (> n 0) (< (st-bar-y s) (- HEIGHT (/ BAR-HEIGHT 2))))
      (make-st (st-ball-x s)
               (st-ball-y s)
               (+ (st-bar-y s) n)
               (st-ball-vx s)
               (st-ball-vy s)
               (st-points s))]
        [(and (< n 0) (> (st-bar-y s) (/ BAR-HEIGHT 2)))
      (make-st (st-ball-x s)
                        (st-ball-y s)
                        (+ (st-bar-y s) n)
                        (st-ball-vx s)
                        (st-ball-vy s)
                        (st-points s))]
         [else s]))
               
  
;(check-expect (move-bar (make-st 1 1 (- HEIGHT (/ BAR-VEL 2)) -1 -1 0) BAR-VEL)
;              (make-st 1 1 (- HEIGHT (/ BAR-HEIGHT 2)) -1 -1 0))
;Esto sería imposible ya que la barra nunca podría estar dentro del suelo
(check-expect (move-bar (make-st 1 1 100 -1 -1 0) BAR-VEL)
              (make-st 1 1 (+ 100 BAR-VEL) -1 -1 0))

; stop?: COMPLETAR DISEÑO
(define (stop? s)
  (or (<= (- (st-ball-x s) BALL-RADIUS) 0) (= (st-points s) PUNTOS-PARA-GANAR)))

; Imagen que aparece en caso de perder o ganar la partida
(define (GAME-OVER s)
  (cond [(<= (- (st-ball-x s) BALL-RADIUS) 0)
         (place-image (text "Juego terminado" 36 "indigo") (/ WIDTH 2) (/ HEIGHT 2) BACKGROUND)]
        [(>= (st-points s) PUNTOS-PARA-GANAR)
         (place-image (text "Felicidades, ganaste!!!" 36 "indigo") (/ WIDTH 2) (/ HEIGHT 2) BACKGROUND)]))

(big-bang START
  [to-draw draw]
  [on-tick ball-next]
  [on-key handle-key]
  [stop-when stop? GAME-OVER])


