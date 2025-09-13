\* Source Pyomo model name=Transportation Problem *\

min 
eCost:
+0.06 vX(Vigo_Madrid)
+0.12 vX(Vigo_Barcelona)
+0.09 vX(Vigo_Valencia)
+0.05 vX(Algeciras_Madrid)
+0.15 vX(Algeciras_Barcelona)
+0.11 vX(Algeciras_Valencia)

s.t.

c_u_eCapacity(Vigo)_:
+1 vX(Vigo_Madrid)
+1 vX(Vigo_Barcelona)
+1 vX(Vigo_Valencia)
<= 350

c_u_eCapacity(Algeciras)_:
+1 vX(Algeciras_Madrid)
+1 vX(Algeciras_Barcelona)
+1 vX(Algeciras_Valencia)
<= 700

c_e_eDemand(Madrid)_:
+1 vX(Vigo_Madrid)
+1 vX(Algeciras_Madrid)
= 400

c_e_eDemand(Barcelona)_:
+1 vX(Vigo_Barcelona)
+1 vX(Algeciras_Barcelona)
= 450

c_e_eDemand(Valencia)_:
+1 vX(Vigo_Valencia)
+1 vX(Algeciras_Valencia)
= 150

bounds
   0.0 <= vX(Vigo_Madrid) <= +inf
   0.0 <= vX(Vigo_Barcelona) <= +inf
   0.0 <= vX(Vigo_Valencia) <= +inf
   0.0 <= vX(Algeciras_Madrid) <= +inf
   0.0 <= vX(Algeciras_Barcelona) <= +inf
   0.0 <= vX(Algeciras_Valencia) <= +inf
end
