;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname TP2-Apellido1-Apellido2-Apellido3) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp") (lib "batch-io.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp") (lib "universe.rkt" "teachpack" "2htdp") (lib "batch-io.rkt" "teachpack" "2htdp")) #f)))
#|
Trabajo Práctico 2: Listas

Integrantes:
- [Rabbia, Augusto].
- [Llop, Agustín].
- [Spreutels, Manuel].
|#

;---------------------Diseño de datos------------------------

; Utilizamos exact-round
(require racket/math)

; Representamos los nombres de los países con String y
; las fechas (año) y el número de habitantes de un país con Number.

(define-struct registro [Fecha Poblacion])
; Registro es (Number, Number)
; Interpretación: un elemento en Registro representa el número
; de habitantes de un país (Poblacion) en una determinada fecha (Fecha).

(define-struct pais [Nombre Registros])
; Pais es (String, List(Registro))
; Interpretación: un elemento en Pais representa un pais de nombre
; Nombre con su lista de registros de censo asociado Registros.

;---------------------Constantes para casos de prueba------------------------

(define ANDORRA (make-pais "Andorra" (list (make-registro 2005 35000000)
                                           (make-registro 2006 -1))))
(define GUATEMALA (make-pais "Guatemala" (list (make-registro 2018 -1)
                                               (make-registro 2019 32500000))))
(define ARGENTINA (make-pais "Angentina" (list (make-registro 2014 35000000)
                                               (make-registro 2015 40000000))))
(define CHINA (make-pais "China" (list (make-registro 2018 999999956)
                                       (make-registro 2019 1000000045))))

;---------------------Preparación de los datos------------------------

; Datos de entrada
(define INPUT-PAISES (read-csv-file "BDPoblacional.csv"))
; Removemos el encabezado
(define DATOS-PAISES (rest INPUT-PAISES))

; Primer registro censo
(define FECHA-INICIO 1960)
; Ultimo registro censo
(define FECHA-FIN 2019)
; Años en los que se realizaron censos [1960,2019]
(define RANGO-FECHA (range FECHA-INICIO (+ FECHA-FIN 1) 1))

; armar-registros: List(Number) List(String) -> List(Registro)
; armar-registros toma una lista de años *lf* y una lista de strings que representan números
; de habitantes *lnh* y devuelve una lista de registros *lr* en donde el i-ésimo
; elemento de *lr* se contruye a partir del i-ésimo elemento de *lf* y
; el i-ésimo elemento de *lnh* convertido a número
(check-expect (armar-registros (list 2005 2006) (list "3" "4"))
              (list (make-registro 2005 3) (make-registro 2006 4)))
(check-expect (armar-registros (list 2005 2006) empty) empty)
(check-expect (armar-registros empty (list (make-registro 2005 3))) empty)
(check-expect (armar-registros empty empty) empty)

(define (armar-registros lf lnh)
  (cond [(empty? lnh) empty]
        [(empty? lf) empty]
        [else (cons (make-registro (first lf) (string->number (first lnh)))
                    (armar-registros (rest lf) (rest lnh)))]))

; armar-lista-registros List(List(String)) List(Number) -> List(List(Registro))
; armar-lista-registros toma una lista de listas de strings que representan cantidad de habitantes
; *lnh* y una lista de años *lf* y devuelve una lista de listas de Registro *lr*
; Los campos Población de los elementos de *lr* coinciden con los elementos de *lnh*
; y su campo fecha se completa con los años de *lf*.
(check-expect (armar-lista-registros empty empty) empty)
(check-expect (armar-lista-registros (list (list "3" "4")) empty) empty)
(check-expect (armar-lista-registros empty (list 2005 2006)) empty)
(check-expect (armar-lista-registros
                  (list (list "3" "4")
                        (list "5" "6")
                        (list "7" "8"))
                  (list 2005 2006))
              (list (list (make-registro 2005 3) (make-registro 2006 4))
                    (list (make-registro 2005 5) (make-registro 2006 6))
                    (list (make-registro 2005 7) (make-registro 2006 8))))

(define (armar-lista-registros lnh lf)
  (cond [(empty? lnh) empty]
        [(empty? lf) empty]
        [else (cons (armar-registros lf (first lnh))
                    (armar-lista-registros (rest lnh) lf))]))

; Datos para construir registros (sin nombres de países)
(define LISTA-DATOS-REGISTRO (map rest DATOS-PAISES))
; Lista de registros de cada país
(define LISTA-REGISTROS (armar-lista-registros LISTA-DATOS-REGISTRO RANGO-FECHA))

; armar-paises: List(String) List(List(Registro)) -> List(Pais)
; armar-paises toma una lista de nombres de países *lnp* y una lista de registros
; poblacionales *lr* y devuelve una lista de estructura Pais *lp* en donde
; el i-ésimo elemento de *lp* tiene de campo Nombre el i-ésimo elemento
; de *lnp* y de campo Registros el i-ésimo elemento de *lr*
(check-expect (armar-paises (list "Angola" "Andorra" "Argentina") empty) empty)
(check-expect (armar-paises empty (list (list (make-registro 2005 3) (make-registro 2006 4)))) empty)
(check-expect (armar-paises (list "Angola" "Andorra" "Argentina")
                            (list (list (make-registro 2005 3) (make-registro 2006 4))
                                  (list (make-registro 2005 5) (make-registro 2006 6))
                                  (list (make-registro 2005 7) (make-registro 2006 8))))
              (list (make-pais "Angola" (list (make-registro 2005 3) (make-registro 2006 4)))
                    (make-pais "Andorra" (list (make-registro 2005 5) (make-registro 2006 6)))
                    (make-pais "Argentina" (list (make-registro 2005 7) (make-registro 2006 8)))))

(define (armar-paises lnp lr)
  (cond [(empty? lnp) empty]
        [(empty? lr) empty]
        [else (cons (make-pais (first lnp) (first lr)) (armar-paises (rest lnp) (rest lr)))]))

; Nombres de países
(define LISTA-NOMBRE-PAISES (map first DATOS-PAISES))
; Lista de países
(define LISTA-PAISES (armar-paises LISTA-NOMBRE-PAISES LISTA-REGISTROS))

;---------------------Funciones de alto orden sobre listas de países------------------------

; transformar-paises: List(Pais) (Pais -> Pais) -> List(Pais)
(define (transformar-paises lp transformacion)
  (cond [(empty? lp) empty]
        [else (cons (transformacion (first lp))
                    (transformar-paises (rest lp) transformacion))]))

; filtrar-paises: List(Pais) (Pais -> Boolean)-> List(Pais)
(define (filtrar-paises lp predicado)
  (cond [(empty? lp) empty]
        [else (if (predicado (first lp))
                  (cons (first lp) (filtrar-paises (rest lp) predicado))
                  (filtrar-paises (rest lp) predicado))]))

; operar-sobre-paises: List(Pais) (Pais X -> X) X -> X
(define (operar-sobre-paises lp operador neutro)
  (cond [(empty? lp) neutro]
        [else (operador (first lp) (operar-sobre-paises (rest lp) operador neutro))]))

;---------------------Consultas y actualizaciones------------------------

;---------------------------------------------
; Sección 1 - Datos incompletos y datos incorrectos
;---------------------------------------------

; Algunos países no tienen datos oficiales completos. Removerlos de la lista de paises.
; Definir una función predicado-registro-incompleto, transformacion-registro-incompleto
; u operacion-registro-incompleto para pasarle como argumento a
; alguno de los patrones de alto orden

; dif-1?: registro Boolean -> Boolean
; Toma los registros de cada año de un país y decide si es o no -1, lo cual significa que no existe.
(check-expect (dif-1? (make-registro 2015 30000) #true) #true)
(check-expect (dif-1? (make-registro 2013 12321421) #false) #f)
(check-expect (dif-1? (make-registro 2015 -1) #true) #false)

(define (dif-1? n b)
  (and (not (= (registro-Poblacion n) -1)) b)) 

; predicado-registro-incompleto: registro -> Boolean
; Toma los registros de un país y, si alguno de ellos es =-1, devuelve #false. En caso contrario, devuelve #true. 
(check-expect (predicado-registro-incompleto (make-pais "Andorra" (list (make-registro 2015 30000)))) #true)
(check-expect (predicado-registro-incompleto (make-pais "Andorra" (list (make-registro 2015 -1)))) #false)

(define (predicado-registro-incompleto x)
  (foldr dif-1? #true (pais-Registros x))) ; Definición de la función auxiliar

(define LISTA-PAISES-REGISTRO-COMPLETO
  (filtrar-paises LISTA-PAISES predicado-registro-incompleto))

; Algunos casos de test para LISTA-PAISES-REGISTRO-COMPLETO
; "West Bank and Gaza" no tiene que estar en el listado de países
; con registro completo pero sí tienen que estar en el listado de países
(check-expect (member "West Bank and Gaza" (map pais-Nombre LISTA-PAISES)) #t)
(check-expect (member "West Bank and Gaza" (map pais-Nombre LISTA-PAISES-REGISTRO-COMPLETO)) #f)
(check-expect (member "Kuwait" (map pais-Nombre LISTA-PAISES)) #t)
(check-expect (member "Eritrea" (map pais-Nombre LISTA-PAISES)) #t)
(check-expect (member "Kuwait" (map pais-Nombre LISTA-PAISES-REGISTRO-COMPLETO)) #f)
(check-expect (member "Eritrea" (map pais-Nombre LISTA-PAISES-REGISTRO-COMPLETO)) #f)
(check-expect (member "China" (map pais-Nombre LISTA-PAISES-REGISTRO-COMPLETO)) #t)

; Los censos del 2014 se calcularon mal. Aumentarlos un 10%.
; Trabajamos con la lista de países con registro completo

; Función auxiliar extra de ayuda, se recomienda utilizar
; recalculo: Registro -> Registro
; recalculo toma un registro e incrementa la población en un 10%
; si la fecha en la que se condujo el censo fue 2014
; En caso de que el incremento resulte en un número no entero,
; se redondea
(check-expect (recalculo (make-registro 2014 10))
              (make-registro 2014 11))
(check-expect (recalculo (make-registro 2016 10))
              (make-registro 2016 10))

(define (recalculo reg)
  (if (= (registro-Fecha reg) 2014)
      (make-registro (registro-Fecha reg) (exact-round (* 1.1 (registro-Poblacion reg))))
      (make-registro (registro-Fecha reg) (registro-Poblacion reg))))

; transformacion-recalcular: Pais -> Pais
; transformacion-recalcular toma un país y aumenta en 10% la población asociada
; al año 2014 en sus registros
(check-expect (transformacion-recalcular ARGENTINA)
              (make-pais "Angentina" (list (make-registro 2014 38500000)
                                           (make-registro 2015 40000000))))
(check-expect (transformacion-recalcular CHINA) CHINA)

(define (transformacion-recalcular pais)
  (make-pais (pais-Nombre pais) (map recalculo (pais-Registros pais))))

(define LISTA-PAISES-RECALCULADA
  (transformar-paises LISTA-PAISES-REGISTRO-COMPLETO transformacion-recalcular))

; Algunos casos de test para LISTA-PAISES-RECALCULADA
; Aruba 2014 original: 103774
; Afghanistan 2014 original: 33370794
(check-expect (member (make-registro 2014 103774)
                      (pais-Registros (first LISTA-PAISES-REGISTRO-COMPLETO))) #t)
(check-expect (member (make-registro 2014 33370794)
                      (pais-Registros (second LISTA-PAISES-REGISTRO-COMPLETO))) #t)
(check-expect (member (make-registro 2014 114151)
                      (pais-Registros (first LISTA-PAISES-RECALCULADA))) #t)
(check-expect (member (make-registro 2014 36707873)
                      (pais-Registros (second LISTA-PAISES-RECALCULADA))) #t)

;---------------------------------------------
; Sección 2 - Países superpoblados
;---------------------------------------------

; Los países con una población mayor a 1000 millones de habitantes en el 2019
; son paises superpoblados
; IMPORTANTE: Trabajamos con la lista de paises con registro completo
; y los valores del 2014 recalculados generada en la sección 1.

; Constante para establecer el mínimo de habitantes
; que hace que un país sea superpoblado
(define MILMILLONES 1000000000)

; Función auxiliar extra de ayuda, se recomienda utilizar
; last: List(X) -> X
; last toma una lista y devuelve el último elemento si es no vacia
; Caso contrario devuelve false
(check-expect (last empty) #f)
(check-expect (last (list "trabajo practico 2")) "trabajo practico 2")
(check-expect (last (list 1 2 3 4 5 6 7)) 7)

(define (last l)
  (cond [(empty? l) #f]
        [(empty? (rest l)) (first l)]
        [else (last (rest l))]))

; predicado-superpoblados Pais -> Boolean
; predicado-superpoblados toma un país y devuelve #t en el caso que haya
; censado más de 1000 millones de habitantes en el 2019
; Suponemos que la última entrada de cada país se corresponde con el año 2019
(check-expect (predicado-superpoblados GUATEMALA) #f)
(check-expect (predicado-superpoblados CHINA) #t)

(define (predicado-superpoblados pais)
 (> (registro-Poblacion (last (pais-Registros pais))) MILMILLONES))

(define LISTA-PAISES-SUPERPOBLADOS
  (filtrar-paises LISTA-PAISES-RECALCULADA predicado-superpoblados))

; Nombres de los países superpoblados
(define NOMBRES-PAISES-SUPERPOBLADOS (map pais-Nombre LISTA-PAISES-SUPERPOBLADOS))

; Cantidad de países sobrepoblados
(define CANT-PAISES-SUPERPOBLADOS (length NOMBRES-PAISES-SUPERPOBLADOS))

; Porcentaje de población en países superpoblados:
; Suma de poblaciones de países superpoblados / población mundial
; Suponemos que la población mundial se calcula a partir de las poblaciones
; de todos los países de los cuales tenemos registro.

; operacion-sumar-poblaciones: registro Number -> Number
; Suma la población de un país a un número n. (esta es una función auxiliar para TASA-POBLACION-SUPERPOBLADOS,
; aquí n será la suma de la población de los países sobre los que ya se operó)

(check-expect (operacion-sumar-poblaciones ARGENTINA 10) (+ 40000000 35000000 10))
(check-expect (operacion-sumar-poblaciones CHINA 94823194) (+ 1000000045 999999956 94823194))

(define (operacion-sumar-poblaciones pais n)
  (+ n (foldr + 0 (map registro-Poblacion (pais-Registros pais)))))


; La TASA-POBLACION-SUPERPOBLADOS se calcula como la población histórica
;(de todos los años registrados) de todos los países superpoblados sobre la población histórica mundial.
(define TASA-POBLACION-SUPERPOBLADOS
  (/ (operar-sobre-paises LISTA-PAISES-SUPERPOBLADOS operacion-sumar-poblaciones 0)
     (operar-sobre-paises LISTA-PAISES operacion-sumar-poblaciones 0)))
