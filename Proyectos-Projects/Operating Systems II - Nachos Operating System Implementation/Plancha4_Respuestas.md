# Cambio del tamaño de la TLB (Ej2)

## `TLB_SIZE = 4`

### `matmult`

```
Paging: faults 64609
TLB Hits: 750492
 TLB Misses: 64609
Hit-Ratio: 0.920735
```

### `sort`

```
Paging: faults 2640594
TLB Hits: 45121400
TLB Misses: 2640594
Hit-Ratio: 0.944714
```

## `TLB_SIZE=32`

### `matmult`

```
Paging: faults 277
TLB Hits: 712185
TLB Misses: 277
Hit-Ratio: 0.999611
```

### `sort`

```
Paging: faults 6340
TLB Hits: 44058760
TLB Misses: 6340
Hit-Ratio: 0.999856
```

## `TLB_SIZE = 64`

### `matmult`

```
Paging: faults 214
TLB Hits: 712132
TLB Misses: 214
Hit-Ratio: 0.999700
```

### `sort`

```
Paging: faults 197
TLB Hits: 44053350
TLB Misses: 197
Hit-Ratio: 0.999996
```


Para estos casos, se observa el poder de la localidad de referencia, con tan sólo 4 páginas, más del 90% de los accesos a memoria se resuelven directamente con la TLB, sin necesidad de cambiar sus entradas.

Sin embargo, utilizar una TLB de tamaño 32 casi erradica por completo los fallos de página (Miss ratio < 0.1%), por lo que podría resultar interesante tener una TLB de este tamaño, aunque cabe considerar al aumentar el tamaño, como al menos en Nachos las entradas son verificadas de manera secuencial, se cuadriplica el tiempo para ver si una entrada no está en la TLB, por lo que cabría considerar si el trade-off es beneficioso. Probablemente lo sea debido a que los accesos a TLB son mucho más rápidos que los accesos a la memoria principal.

Por último, con un tamaño de 64, se observan rendimientos decrecientes, ya que el porcentaje de TLB hits casi no difiere respecto de la TLB de 32 páginas. En un sistema real,  este pequeño beneficio probablemente no justifique el costo que supondría tener una TLB de este tamaño.


# Comparación entre FIFO y CLOCK (Ej6)




## `matmult`


### `FIFO`

```
Paging: faults 355
TLB Hits: 713676
TLB Misses: 355
Hit-Ratio: 0.999503
```

### `CLOCK`

```
Paging: faults 355
TLB Hits: 713676
TLB Misses: 355
Hit-Ratio: 0.999503
```


## `sort`

### `FIFO`

```
Paging: faults 6337
TLB Hits: 44058760
TLB Misses: 6337
Hit-Ratio: 0.999856
```

### `CLOCK`

```
Paging: faults 6340
TLB Hits: 44058760
TLB Misses: 6340
Hit-Ratio: 0.999856
```

Notemos que el hit-ratio al correr `matmult` es exactamente igual cuandoo se usa FIFO y cuando se usa el algoritmo del reloj y es muy similar con ambos algoritmos al correr `sort`.

