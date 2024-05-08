;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Prog Interactivos - Saludos con ontick onmouse onkey|) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp")) #f)))

(define ANCHO 500)
(define ALTO 250)

(define ESCENA (empty-scene ANCHO ALTO))

(define Size 60) ;Tamaño del texto a mostrar

(define SALUDO1 "Bienvenido")
(define SALUDO2 "Bem-vindo")
(define SALUDO3 "Bienvenue")
(define SALUDO4 "Welcome")

(define CHAU "Chau")

(define (Saludar Palabra Color)
  (place-image (text Palabra Size Color) (/ ANCHO 2) (/ ALTO 2) ESCENA)
  )

(define (Interpretar Estado)
  (cond [(string=? Estado SALUDO1) (Saludar Estado "red")]
        [(string=? Estado SALUDO2) (Saludar Estado "blue")]
        [(string=? Estado SALUDO3) (Saludar Estado "green")]
        [(string=? Estado SALUDO4) (Saludar Estado "violet")]
        [else (Saludar Estado "black")])
  )


(define (manejadorPorPulsos Estado)
  (cond [(string=? Estado SALUDO1) SALUDO2]
        [(string=? Estado SALUDO2) SALUDO3]
        [(string=? Estado SALUDO3) SALUDO4]
        [(string=? Estado SALUDO4) SALUDO1])
  )

(define (manejadorTeclado Estado Tecla)
  (cond [(string=? Tecla "c") SALUDO1]
        [(string=? Tecla "p") SALUDO2]
        [(string=? Tecla "f") SALUDO3]
        [(string=? Tecla "i") SALUDO4]
        [(string=? Tecla "down") CHAU]
        [else Estado])
  )

(define (manejadorMouse Estado x y Evento)
  (if (and (string=? Evento "button-down") (< (/ ANCHO 2) x))
      CHAU Estado))

(define (terminar Estado)
  (if (string=? Estado CHAU) #true
      #false))

(define INICIAL SALUDO1)
  
(big-bang INICIAL
  [on-key manejadorTeclado]
  [on-mouse manejadorMouse]
  [on-tick manejadorPorPulsos 2]
  [to-draw Interpretar]
  [stop-when terminar Interpretar])






















  