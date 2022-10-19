# Laborator 1

## Fisiere: 
    Pentru exercitiile 1-4 am addaugat fiserele example_nr.c, nr corespunzand exercitiului;
    Analog, exercitiul 5 este rezolvat in add_serial_5.c.
    Pentru exercitiul 6 am adaugat explicatii in Readme, precum si o poza cu timpul.

### ex1:
    Functia f este apelata de un nr egal cu numarul de threaduri, nu mereu respectandu-se ordinea data de numar.

### ex3:
    Iteratiile de pe threaduri diferite nu sunt efectuate pe rand, de fiecare thread, ci se intercaleaza.
    De ex, threadul 1 nu va afisa toate cele 100 de mesaje imediat, printre ele putand fi afisate mesaje si 
    din alte threaduri.

### ex5:
    In locul adunarii fiecarui element cu 100, se va desfasura, pe mai multe threaduri adunarea cu 100 pentru un numar mai mic de elemente.
    Dupa calcularea pozitiilor de start si end pentru un thread, acestea sunt salvate intr-o structura si trimise apoi ca parametru pentru functia de thread.

### ex6: 
    Pentru un vector cu 100000000 elemente, in cazul rularii programului nemodificat, se obtine un timp real de 0,619 s. In cazul rularii programului modificat la ex5, se obtine un timp real de 0,461 s. In 
    cazul ambelor rulari, am dezactivat afisarea vectorului inainte si dupa adunarea cu 100, pentru surprinde rezultatul in screenshot.
    Daca rulam programele cu tot cu afisare, obtinem un timp real de 0,891 s pentru programul nemodificat, si 
    un timp real de 0,830 s pentru cel modificat. (am verificat pentru un vector cu 100000 elemente);
