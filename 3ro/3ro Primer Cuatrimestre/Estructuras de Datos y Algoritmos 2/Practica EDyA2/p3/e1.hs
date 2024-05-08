-- Color
type ColorRGB = (Float, Float, Float)

-- Mezcla 2 colores
mezclar :: ColorRGB -> ColorRGB -> ColorRGB
mezclar (r1,g1,b1) (r2,g2,b2) = 
    let r = (r1 + r2) / 2
        g = (g1 + g2) / 2
        b = (b1 + b2) / 2
    in (r,g,b)
