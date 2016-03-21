Tema 3 APD 2015 - Bluea Ana-Marina 332CA

Topologiile sunt descrise in fisierele topology2, topology3 si topology4. Fisierul
script.sh este scriptul de testare folosit pe cluster.

Tema trece prin 5 etape:

1. Determinarea topologiei.
Acesta etapa se desfasoara in functia build_topology dupa ce s-a citit lista de
vecini din fisierul de topologie. Fiecare nod (in afara de radacina) asteapta
prima notificare de parinte de la un vecin (mesaj cu tagul PARENT_TAG) care este
acceptata, apoi trimite celorlalti vecini ai sai notificari de parinte. Dupa ce
toate nodurile isi trimit notificarile, rejecteaza celelalte notificari de parinte
primite, cu un mesaj cu tagul PARENT_REJECT_TAG. Nodul isi numara rejectiile primite
de la vecini pentru a le scoate din coada de mesaje si pentru a stii cati copii are
pentru pasul urmator. In pasul urmator, nodul primeste topologiie de la fiecare
copil si le combina pentru a-si genera topologia sa. O topologie este descrisa
printr-o matrice de adiacenta si o tabela de rutare (structura NodeTopology).
In final nodul trimite topologia sa la parinte.

Aici se face afisarea tabelei de rutare si matricii de adiacenta la stdout.
formatul este urmatorul:

rank_nod: (destinatie1, ruta1) (destinatie2, ruta2) ...

Si pentru matrice:

m00 m01 m02 ... m0n
m10 m11 m12 ... m1n
...             ...
mn0 mn1 mn1 ... mnn

Daca mij este 1 atunci i este parintele lui j.


2. Determinarea patratului rezolvat de nod.
Fiecare nod isi alege patratul pe baza rankului sa, adica in cazul unui sudoku de
3x3 nodul 0 va rezolva patratul (0, 0), nodul 1 va rezolva (0, 1), nodul 9 va
rezolva patratul (2, 2).

3. Obtinerea solutiilor partiale.
Fiecare nod isi calculeaza solutiile pentru patratul lor cu functia generate.
Aceasta calculeaza coordonatele patratului in matricea sudoku-ului pe baza rankului
si determina numerele din patrat care nu sunt folosite (adica care trebuie
completate). Acestea sunt trimise la functia generate_func care incearca pentru
o patratica libera numerele nefolosite si se reapeleaza recursiv pentru a incearca
pe patratica urmatoare. Cand se completeaza toate patraticile se verifica sudoku-ul
si daca este corect se retine solutia partiala.

Fiecare nod asteapta sa primeasca solutiile partiale de la copii.

4. Combinarea solutiilor partiale.
Toate solutiile partiale de la nodul curent si copii sunt pasate la functia
generate2 care plecand de la sudoku-ul original incearca sa-l combine cu solutiile
partiale ale unui nod. Daca o combinare da un sudoku valid acesta este dat din nou
la functia generate2 pentru a-l combina cu solutiile partiale de la un alt nod.
Cand sudoku-ul este completat cu cate o solutie partiala de la fiecare nod si
este valid aceasta solutie este retinuta intr-un vector.

Cand nodul radacina termina de generat solutiile finale afiseaza prima solutie
in fisierul de output si se opreste aici.

5. Trimiterea solutiilor finale.
Nodul trimite parintelui vectorul de solutii pentru a fi combinate din nou.
