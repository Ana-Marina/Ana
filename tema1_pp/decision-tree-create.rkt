#lang racket

(require test-engine/racket-tests)
(include "decision-tree-test.rkt")

;structura (nume-atribut (val1 val2 val3 ...) (copil1 copil2 copil3 ...))
(struct node (attrib-name attrib-vals childs))

;; pentru frunze:

;; primește un nod; întoarce #t dacă acesta este frunză, #f altfel
(define (is-leaf? node)
  (null? (node-childs node)))

;; primște un nod frunză; întoarce clasa exemplelor din frunză
(define (get-leaf-class node)
  (car (node-attrib-vals node)))

;; pentru frunze speciale (BONUS):

;; primște un nod; întoarce #t dacă nodul este frunză specială (frunză fără exemple sau cu exemple de clase diferite), #f altfel
;; dacă nu implementați bonus, trebuie ca funcția să întoarcă #f
(define is-special-leaf?
  ; TODO
  (λ (node) #f)
  )

;; primște un nod frunză specială; întoarce tipul frunzei speciale (trebuie să fie unul dintre membrii 2 și 3 ai strings
;; clasa exemplelor din frunza specială va fi verificată tot cu get-leaf-class
(define get-special-leaf-type
  ; TODO
  (λ (node) #f)
  )


;; pentru noduri care nu sunt frunze:

;; primște un nod; întoarce numele atributului verificat în nod
(define (get-attribute-name node)
  (node-attrib-name node))

; transforma listele keys si values intr-o lsita de forma ((key1 . val1) (key2 . val2) ...)
(define (map-make keys values)
  (map cons keys values))

; returneaza valoarea asociata cheii
(define (map-lookup key map)
  (cdr (assoc key map)))

;; primește un nod și o valoare a atributului verificat în nod
;; întoarce nodul copil (nod intern sau frunză) corespunzător valorii date

(define (get-child node value)
  (map-lookup value (map-make (node-attrib-vals node) (node-childs node))))


;; opțional: verificare nod
;; primește un argument; întoarce #t dacă argumentul este o reprezentare corectă a unui nod (frunză sau nu) din arbore; #f altfel
(define (is-node? node)
  (node? node))

; asamblare funcții de acces arbore
(define functions (list is-leaf? get-leaf-class is-special-leaf? get-special-leaf-type get-attribute-name get-child is-node?))


;; TASK (pregătitor):
;; scrieți (manual) în formatul ales un arbore de decizie pentru exemple conținând 1 atribut - shape, care are două valori - round și square
;; un exemplu va fi în clasa "yes" dacă este rotund, și în "no" altfel
;; arborele trebuie să fie astfel:
;;    shape
;;     / \
;; round square
;;   /     \
;; yes     no
(define tree-1 
  (node 'shape '(round square) 
        (list 
         (node 'is-round '(yes) '()) 
         (node 'is-round '(no) '()))))

(check-expect (is-node? tree-1) #t)
(check-expect (is-leaf? tree-1) #f)
(check-expect (get-attribute-name tree-1) 'shape)
(check-expect (not (get-child tree-1 'round)) #f)
(check-expect (not (get-child tree-1 'square)) #f)
(check-expect (is-leaf? (get-child tree-1 'round)) #t)
(check-expect (is-leaf? (get-child tree-1 'square)) #t)
(check-expect (is-special-leaf? (get-child tree-1 'round)) #f)
(check-expect (get-leaf-class (get-child tree-1 'round)) 'yes)
(check-expect (get-leaf-class (get-child tree-1 'square)) 'no)

; returneaza true daca lista contine elem
(define (contains? list elem)
  (ormap (λ (x) (equal? x elem)) list))

; calculeaza proportia exemplelor care contin atributul (attrib-name . value)
(define (examples-proportion examples attrib-name value)
  (/ (foldl (λ (example acc) (if (contains? example (cons attrib-name value))
                             (add1 acc)
                             acc))
            0 examples)
     (length examples)))

; pentru fiecare element din values calculeaza proportia exemplor care contin
; (attrib-name . value)
(define (examples-proportions examples attrib-name values)
  (map (λ (x) (examples-proportion examples attrib-name x)) values))

; calculeaza entropia pe baza proportiei
(define (entropy p)
  (if (or (= p 1) (= p 0))
      0
      (* p (log2 p))))

;; TASK
;; scrieți funcția de mai jos pentru a calcula entropia unui set de exemple, fiecare exemplu conținând informație despre clasa sa
;; funcția log2 este implementată în decision-tree-test

;; examples: o listă de exemple (setul S), nevidă, în care fiecare exemplu este o listă de perechi, una dintre ele fiind (<nume-atribut-clasă> <clasă>)
;; class-attribute: o listă de forma (<nume-atribut-clasă> <clasă-1> <clasă-2> <clasă-3>)
;; întoarce: entropia setului de exemple în raport cu clasa, H(S) = - sumă-peste-clase p(clasă)*log2 p(clasă)
;;   unde p(clasă) este numărul de exemple cu clasa <clasă> împărțit la numărul de exemple din setul S

(define (compute-enthropy examples class-attribute)
  (if (null? examples)
      0
      (- (apply + (map entropy 
                       ; pentru fiecare clasa calculeaza proportia exemplelor care o contin
                       (examples-proportions examples (car class-attribute) (cdr class-attribute)))))))

(define tolerance 0.001)
;(check-within (compute-enthropy '() '(classname yes no)) 0 tolerance) ; expect error
(check-within (compute-enthropy '(((shape . round) (classname . yes)) ((shape . square) (classname . yes))) '(classname yes no)) 0 tolerance)
(check-within (compute-enthropy '(((shape . round) (classname . no)) ((shape . square) (classname . no))) '(classname yes no)) 0 tolerance)
(check-within (compute-enthropy '(((shape . round) (classname . yes)) ((shape . square) (classname . no))) '(classname yes no)) 1 tolerance)
(check-within (compute-enthropy '(((shape . round) (classname . yes)) ((shape . square) (classname . no)) ((shape . square) (classname . no))) '(classname yes no maybe)) 0.918 tolerance)
(check-within (compute-enthropy '(((shape . round) (classname . yes)) ((shape . square) (classname . no)) ((shape . square) (classname . maybe))) '(classname yes no maybe)) 1.584 tolerance)

; elimina atributul attrib-name din exemplu
(define (remove-attrib attrib-name example)
  (filter (λ (attrib) (not (equal? (car attrib) attrib-name))) example))

; determina lista exemplelor care contin (attrib-name . attrib-val) si elimina atributul din rezultat
(define (examples-subset examples attrib-name attrib-val)
    (map (λ (example) (remove-attrib attrib-name example)) 
         (filter (λ (example) (contains? example (cons attrib-name attrib-val))) examples)))

; pentru fiecare valoare din attrib-vals determina lista exemplelor care contin (attrib-name . value)
; atributul attrib-name este eliminat din fiecare exemplu
(define (examples-subsets examples attrib-name attrib-vals)
  (map (λ (attrib-val) (examples-subset examples attrib-name attrib-val)) attrib-vals))

; returneaza valoare atributului attrib-name din exemplu
(define (example-get-attrib-val example attrib-name)
  (cdr (findf (λ (attrib) (equal? (car attrib) attrib-name)) example)))

; calculeaza entropia pentru subsetul exemplelor care contin (attrib-name . attrib-val)
(define (compute-subset-gain examples attrib-name attrib-val class-attribute)
  (let ([subset (examples-subset examples attrib-name attrib-val)])
    (* (/ (length subset) (length examples)) (compute-enthropy subset class-attribute))))

;; TASK
;; scrieți funcția de mai jos pentru a calcula câștigul informațional al unui atribut în raport cu clasa, pentru un set de exemple

;; examples: o listă de exemple, nevidă, în care fiecare exemplu este o listă de perechi, una dintre ele fiind (<nume-atribut-clasă> <clasă>)
;; attribute: o listă de forma (<nume-atribut> <valore-1> <valoare-2> <valoare-3>)
;; class-attribute: o listă de forma (<nume-atribut-clasă> <clasă-1> <clasă-2> <clasă-3>)
;; întoarce: câștigul informațional al atributului, G(S, A) = H(S) - sumă-peste-valori-A p(v)*H(Sv)
;;   unde p(v) este numărul de exemple cu valoarea v pentru A împărțit la numărul de exemple din S
;;   iar Sv este mulțimea exemplelor din S care au valoarea v pentru A

(define (compute-gain examples attribute class-attribute)
  (let* ([attrib-name (car attribute)]
         [values (cdr attribute)]
         [subset-gain (λ (val) (compute-subset-gain examples attrib-name val class-attribute))])
    (- (compute-enthropy examples class-attribute) ;H(s)
       (apply + (map subset-gain values)))))       ;Suma p(S-val)H(S-val)

(check-within (compute-gain 
               '(((shape . round) (classname . yes)) ((shape . square) (classname . yes)))
               '(shape round square)
               '(classname yes no)
               ) 0 tolerance)
(check-within (compute-gain 
               '(((shape . round) (classname . no)) ((shape . square) (classname . yes)))
               '(shape round square)
               '(classname yes no)
               ) 1 tolerance)
(check-within (compute-gain 
               '(((shape . round) (classname . no)) ((shape . round) (classname . yes)))
               '(shape round square)
               '(classname yes no)
               ) 0 tolerance)
(check-within (compute-gain 
               '(((shape . round) (size . 1) (classname . yes))
                 ((shape . round) (size . 2) (classname . no))
                 ((shape . square) (size . 1) (classname . yes))
                 ((shape . square) (size . 2) (classname . yes)))
               '(shape round square)
               '(classname yes no)
               ) 0.311 tolerance)
(check-within (compute-gain 
               '(((shape . round) (size . 1) (classname . yes))
                 ((shape . round) (size . 2) (classname . no))
                 ((shape . square) (size . 1) (classname . yes))
                 ((shape . square) (size . 2) (classname . no))
                 ((shape . square) (size . 2) (classname . yes)))
               '(size 1 2)
               '(classname yes no)
               ) 0.419 tolerance)



;; TASK
;; creați un arbore de decizie pentru mulțimea dată de exemple, pentru muțimea dată de atribute, și pentru clasa dată

;; examples: o listă de exemple, nevidă, în care fiecare exemplu este o listă de perechi, una dintre ele fiind (<nume-atribut-clasă> <clasă>)
;; attributes: o listă de liste de forma (<nume-atribut> <valore-1> <valoare-2> <valoare-3>)
;; class-attribute: o listă de forma (<nume-atribut-clasă> <clasă-1> <clasă-2> <clasă-3>)
;; întoarce: un arbore de decizie, în formatul ales, corespunzător cu argumentele date
(define (create-tree examples attributes class-attribute)
  (if (null? attributes)
      ; nu mai sunt atribute, se returneaza frunza cu clasa primului exemplu
      (node (car class-attribute) (list (example-get-attrib-val (car examples) (car class-attribute))) '())
      ;       split-attr=(attrib-name IG) atributul cu information gain-ul maxim, dupa care se face prtitionarea
      (let* ([split-attr (foldl (λ (attrib max-attr)
                                  (let ([IG (compute-gain examples attrib class-attribute)])
                                    (if (> IG (cdr max-attr))
                                        (cons (car attrib) IG)
                                        max-attr))) '(_ . -inf.0) attributes)]
             ; lista de valori ale split-attr
             [split-attr-vals (cdr (findf (λ (x) (equal? (car x) (car split-attr))) attributes))]
             ; elimina split-attr din lista de atribute
             [new-attributes (remove-attrib (car split-attr) attributes)])
        (if (= 0 (cdr split-attr)) ; entropie 0?
            ; da => toate exemplele au aceeasi clasa, se returneaza frunza corespuznatoare
            (node (car class-attribute) (list (example-get-attrib-val (car examples) (car class-attribute))) '())
            ; nu => se creeaza nodul si se apeleaza create-tree recursiv pe partitiile exemplelor dupa split-attr pentru generarea copiilor
            (node (car split-attr) split-attr-vals (map (λ (examples-partition) (create-tree examples-partition new-attributes class-attribute)) 
                                                        (examples-subsets examples (car split-attr) split-attr-vals)))))))


(define I-DID-THE-BONUS #f)

(check-expect (perform-test functions 'food-small create-tree) #t)
(check-expect (perform-test functions 'food-big create-tree) #t)
(check-expect (perform-test functions 'objects create-tree) #t)
(check-expect (perform-test functions 'weather create-tree) #t)

(check-expect (and (perform-test functions 'food-small create-tree) (get-tree-height functions (get-tree 'food-small create-tree) (get-test 'food-small))) 2)
(check-expect (and (perform-test functions 'food-big create-tree)   (get-tree-height functions (get-tree 'food-big create-tree) (get-test 'food-big)))   4)
(check-expect (and (perform-test functions 'weather create-tree)    (get-tree-height functions (get-tree 'weather create-tree) (get-test 'weather)))    3)
(check-expect (and (perform-test functions 'objects create-tree)    (get-tree-height functions (get-tree 'objects create-tree) (get-test 'objects)))    3)

(if I-DID-THE-BONUS (display "BONUS DONE\n") (display "bonus not done\n"))
(check-expect (if I-DID-THE-BONUS (perform-test functions 'bonus create-tree) #t) #t)
(when I-DID-THE-BONUS (display (get-tree 'bonus create-tree)) (newline))



(test)
