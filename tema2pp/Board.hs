{-
    Tabla de joc și mutările posibile.

    Modulul exportă numai funcțiile enumerate mai jos, ceea ce înseamnă
    că doar acestea vor fi vizibile din alte module. Justificarea vizează
    prevenirea accesului extern la structura obiectelor 'Board', și a eventualei
    coruperi a consistenței interne a acestora.
-}
module Board
    ( Board
    , Player (..)  -- Exportăm și constructorii de date 'You' și 'Opponent'.
    , House
    , build
    , yourSeeds
    , oppsSeeds
    , who
    , isOver
    , initialBoard
    , move
    , scores
    , successors
    ) where

import Consecutive
import Data.List

{-
    Jucătorul care urmează să mute.
-}
data Player = You | Opponent deriving (Eq, Show)

{-
    Tipul caselor, definit pentru lizibilitate.
-}
type House = Int

{-
    *** TODO ***

    Definiți tipul 'Board', astfel încât să rețină informație despre starea
    jocului, inclusiv tabla de joc.

    Observați că viitorii constructori de date și eventualele câmpuri pe care
    le veți specifica nu vor apărea în lista de funcții exportate de modul
    (vezi explicația de la începutul fișierului).
-}

-- la fel ca argumentele functiei build
data Board = Board ([Int], Int) ([Int], Int) Player deriving Eq

-- o poziitie de pa board
data Position = Store Player | House' House Player

{-
    *** TODO ***

    Instanțiați clasa 'Show' cu tipul 'Board'. Exemplu de reprezentare,
    unde scorurile sunt aferent jucătorilor 'You', respectiv 'Opponent':

       4  4  4  4  4  4
     0                  0    Next: You, Playing, Score: (0,0)
       4  4  4  4  4  4
-}
instance Show Board where
    show board =
        let s1 = yourSeeds board
            s2 = oppsSeeds board
        in " " ++ intercalate " " (map show (fst s2)) ++ "\n" ++ show (snd s2) ++
           "            " ++ show (snd s1) ++ "   " ++ "Next: " ++
           show (who board) ++ ", score: " ++ show (scores board) ++ "\n" ++ " " ++
           intercalate " " (map show (fst s1))

{-
    *** TODO BONUS ***

    Instanțiați clasa 'Consecutive', pentru a putea determina dacă în două
    configurații ale tablei trebuie să mute același jucător.
-}
instance Consecutive Board where
    b1 >< b2 = who b1 == who b2

{-
    *** TODO ***

    Construiește tabla de joc.

    Funcția trebuie să determine dacă jocul este deja încheiat, pe baza
    conținutului caselor.
-}
build :: ([Int], Int)  -- Conținutul caselor și al depozitului utilizatorului
      -> ([Int], Int)  -- Conținutul caselor și al depozitului adversarului
      -> Player        -- Jucătorul aflat la rând
      -> Board         -- Tabla construită
build = Board

{-
    *** TODO ***

    Întoarce conținutul caselor și al depozitului utilizatorului.
-}
yourSeeds :: Board -> ([Int], Int)
yourSeeds (Board s1 _ _) = s1

{-
    *** TODO ***

    Întoarce conținutul caselor și al depozitului adversarului.
-}
oppsSeeds :: Board -> ([Int], Int)
oppsSeeds (Board _ s2 _) = s2

{-
    *** TODO ***

    Întoarce jucătorul aflat la rând.
-}
who :: Board -> Player
who (Board _ _ p) = p

{-
    *** TODO ***

    Întoarce 'True' dacă jocul s-a încheiat.
-}
isOver :: Board -> Bool
-- daca oricare jucator are 0 seminte in casele lui
isOver board = if sum (fst (yourSeeds board)) == 0 || sum(fst (oppsSeeds board)) ==0
    then True
    else False

{-
    *** TODO ***

    Tabla inițială.
-}
initialBoard :: Board
initialBoard = let seeds = [4, 4, 4, 4, 4, 4] in build (seeds, 0) (seeds, 0) You

{-
    *** TODO ***

    Realizează o mutare pornind de la casa furnizată ca parametru, în funcție
    de configurația actuală a tablei și de jucătorul aflat la rând.

    Întoarce aceeași configurație dacă mutarea nu poate fi efectuată
    din diverse motive, precum numărul eronat al casei, sau casa goală.
-}

-- incrementeaza pozitia data in sensul invers al acelor de ceas
step :: Position -> Position
step (Store You) = House' 6 Opponent
step (Store Opponent) = House' 1 You
step (House' 6 You) = Store You
step (House' n You) = House' (n+1) You
step (House' 1 Opponent) = Store Opponent
step (House' n Opponent) = House' (n-1) Opponent

-- adauga s seminte in casa de la pozitia data
addSeeds :: Board -> Int -> Position -> Board

addSeeds (Board (seeds1, store1) s2 p) s (Store You) =
    (Board (seeds1, store1+s) s2 p)

addSeeds (Board s1 (seeds2, store2) p) s (Store Opponent) =
    (Board s1 (seeds2, store2+s) p)

addSeeds (Board (seeds1, store1) s2 p) s (House' house You) =
    (Board (map (\(h, v) -> if h==house then v+s else v ) (zip [1..] seeds1), store1) s2 p)

addSeeds (Board s1 (seeds2, store2) p) s (House' house Opponent) =
    (Board s1 (map (\(h, v) -> if h==house then v+s else v ) (zip [1..] seeds2), store2) p)


-- returneaza numarul de seminte de la o pozitie
getSeeds :: Board -> Position -> Int
getSeeds (Board (_, store1) _ _) (Store You) = store1
getSeeds (Board _ (_, store2) _) (Store Opponent) = store2
getSeeds (Board (seeds1, _) _ _) (House' house You) = seeds1 !! (house - 1)
getSeeds (Board _ (seeds2, _) _) (House' house Opponent) = seeds2 !! (house - 1)


-- seteaza numarul de seminte de la o pozitie
setSeeds :: Board -> Int -> Position -> Board
setSeeds (Board (seeds1, store1) s2 p) s (Store You) =
    (Board (seeds1, s) s2 p)

setSeeds (Board s1 (seeds2, store2) p) s (Store Opponent) =
    (Board s1 (seeds2, s) p)

setSeeds (Board (seeds1, store1) s2 p) s (House' house You) =
    (Board (map (\(h, v) -> if h==house then s else v ) (zip [1..] seeds1), store1) s2 p)

setSeeds (Board s1 (seeds2, store2) p) s (House' house Opponent) =
    (Board s1 (map (\(h, v) -> if h==house then s else v ) (zip [1..] seeds2), store2) p)


-- functie ajutatoare pentru generatePath
generatePathHelper :: Board -> Position -> Int -> [Position]
generatePathHelper board pos 0 = [pos]
generatePathHelper board pos n = if owner pos /= who board && isStore pos
                                 -- se trece prin storeul oponentului, skip
                                 then generatePathHelper board (step pos) n
                                 else pos : (generatePathHelper board (step pos) (n-1))

-- genereaza succesiunea de pozitii prin care se trece la semanarea a n seminte
-- incepand de la casa data
generatePath :: Board -> House -> Int -> [Position]
generatePath board house n = generatePathHelper board (House' house (who board)) n


-- adauga seamana semintele pe baza pathului dat
-- casa initiala este golita, iar restul caselor primes o samanta
applyPath :: Board -> [Position] -> Board
applyPath board (p:ps) = foldl (\board p -> (addSeeds board 1 p)) (setSeeds board 0 p) ps

-- returneaza pozitia opusa pozitiei date
opposite :: Position -> Position
opposite (Store You) = (Store Opponent)
opposite (Store Opponent) = (Store You)
opposite (House' house You) = (House' house Opponent)
opposite (House' house Opponent) = (House' house You)

-- ia semintele de la pozitia data si opusa ei si le pune in magazia jucatorului
takeSeeds :: Board -> Position -> Board
takeSeeds board pos = addSeeds (setSeeds (setSeeds board 0 (opposite pos)) 0 pos)
                      ((getSeeds board pos) + (getSeeds board (opposite pos)))
                      (Store (who board))

-- returneaza jucatorul care detine pozitia data
owner :: Position -> Player
owner (Store p) = p
owner (House' _ p) = p

-- true daca pozitia data este magaize
isStore :: Position -> Bool
isStore (Store _) = True
isStore (House' _ _) = False

-- schimba jucatorul boardului
switchPlayer :: Board -> Board
switchPlayer (Board s1 s2 You) = (Board s1 s2 Opponent)
switchPlayer (Board s1 s2 Opponent) = (Board s1 s2 You)

move :: House -> Board -> Board
move house board =
    let pos = (House' house (who board)) -- pozitia din care se face mutarea
        -- drumul prin care se seamana semintele
        path = generatePathHelper board pos (getSeeds board pos)
        last_pos = last path  -- pozitia ultimei seminte
        board2 = applyPath board path  -- board dupa semanare
        board3 = takeSeeds board2 (last path) -- board dupa takeSeeds
    in if house <1 || house >6 || getSeeds board pos == 0
       then board -- mutare ivnalida
       else if owner last_pos == who board && getSeeds board last_pos == 0 &&
               getSeeds board (opposite last_pos) /= 0 && not(isStore last_pos)
            then switchPlayer board3 -- ultima samata pica intr-o casa goala
            else if owner last_pos == who board && isStore last_pos
                 then board2 -- ultima samanta pica in magazia jucatorului
                 else switchPlayer board2

{-
    *** TODO ***

    Întoarce scorurile (utilizator, adversar).

    Calculul trebuie să țină cont de eventuala încheiere a jocului.
-}
scores :: Board -> (Int, Int)
scores board =
    let s1 = yourSeeds board
        s2 = oppsSeeds board
    in if isOver board
       then (sum (fst s1) + snd s1, sum (fst s2) + snd s2)
       else (snd s1, snd s2)

{-
    *** TODO ***

    Întoarce perechile casă-configurație, reprezentând mutările care pot fi
    făcute într-un singur pas, pornind de la configurația actuală.
-}
successors :: Board -> [(House, Board)]
successors board = [ (house, move house board) | house <- [1..6],
                   getSeeds board (House' house (who board)) /= 0 ]
