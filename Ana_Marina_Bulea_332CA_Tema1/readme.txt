Tema 1 APD 2015
Bulea Ana-Marina 332CA

1. Super Sampling Anti Aliasing

Structura image contine latimea, inaltimea, valoarea maxima a culorilor,
numarul de culori si matricea de pixeli. Alocarea, dealocarea
si accesarea unui pixel se face prin functiile create_image, delete_image
si get_pixel implementate in homework.c.

Micsorarea imaginii se face prin iterarea peste fiecare pixel din
imaginea de iesire(de dimensiuni latime/resize_factor si inaltime/resize_factor)
si calculand valoarea sa pe baza imaginii de intrare cu ajutorul
functiei pixel_sum. In cazul in care dimensiunile imaginii nu se impart
exact la factorul de scalare imaginea de intrare este 'trunchiata', adica
pixelii care sunt in plus sunt ignorati.

Paralelizarea algoritmului se face in functia resize. Pentru ca acesta
itereaza prin pixelii imaginii de iesire, am paralelizat cele 2 foruri.
Astfel iamginea de iesire este impartita egal tuturor firelor de executie
pentru procesare.

#pragma omp parallel for collapse(2) \
	private(i, j, c, sum, pixel)
for(i = 0; i < out->height; i++){
	for(j = 0; j < out->width; j++){

Programul apeleaza omp_set_num_threads(num_threads) pentru a specifica
numarul de fire folosite.

Testare pe cluster cu o imagine color de 8192x8192:

timp de rulare pentru factor scalare(s) vs nr. threaduri(n)

s\n  1           2           4           8           16
3    1.901750    0.979487    0.492176    0.251627    0.233714
8    1.506994    0.775964    0.386922    0.195247    0.176653

Se observa ca timpul de executie scade semnificativ cu numarul de fire
de executie pana ajunge la 8 fire de unde nu se mai obtin castiguri
semnificative in performanta prin cresterea numarului de fire.


2. Micro renderer

Foloseste aceasi structura image si aceleasi functii ca la 1.
Paralelizarea insa se face in functia render care peste fiecare pixel din
imaginea de iesire si calculeaza distanta de la centrul pixelului la linie.
Daca aceasta este mai mica ca 3 pixelul este negru, altfel este alb.

La fel ca la 1 am paralelizat cele 2 foruri care parcurg imaginea.

Testare pe cluster cu rezolutia=10000:

timp de rulare(t) pentru nr. threaduri(n)

n    1           2           4           8           16
t    2.244293    1.143877    0.567878    0.286728    0.182123

Ca si la 1 timpul de rulare scade semnificativ cu numarul de fire de
executie.


Testele au fost efectuate pe clusterul facultatii pe coada ibm-nehalem.q.