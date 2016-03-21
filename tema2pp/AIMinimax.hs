module AIMinimax where

import Board
import Consecutive
import Interactive

{-
    *** TODO ***

    Implementați tipul 'Tree s a', al arborilor minimax, unde 's' reprezintă
    tipul stărilor, iar 'a', tipul acțiunilor (în cazul de față, al mutărilor).
-}
data Tree s a = Undefined

{-
    *** TODO ***

    Întoarce casa aleasă de o euristică în contextul minimax, alături
    de configurația rezultată.
-}
step :: Board -> (House, Board)
step = undefined

{-
    *** TODO ***

    Construiește un arbore minimax (eventual infinit), pornind de la funcția
    de generare a succesorilor unui nod și de la rădăcină.
-}
expand :: (s -> [(a, s)]) -> s -> Tree s a
expand = undefined

{-
    *** TODO ***

    Limitează un arbore la numărul de niveluri dat ca parametru.
-}
prune :: Int -> Tree s a -> Tree s a
prune = undefined

{-
    *** TODO ***

    Determină valoarea minimax a unui nod MAX. Funcția de evaluare statică
    este dată ca parametru.
-}
maximize :: Consecutive s => (s -> Float) -> Tree s a -> Float
maximize = undefined

{-
    *** TODO ***

    Determină valoarea minimax a unui nod MIN. Funcția de evaluare statică
    este dată ca parametru.
-}
minimize :: Consecutive s => (s -> Float) -> Tree s a -> Float
minimize = undefined

{-
    *** TODO ***

    Întoarce cheia copilului rădăcinii arborelui minimax, ales în conformitate
    cu principiul algoritmului. Funcția de evaluare statică este dată
    ca parametru.
-}
pick :: Consecutive s => (s -> Float) -> Tree s a -> (a, s)
pick = undefined

{-
    Urmărește pas cu pas evoluția jocului, conform strategiei implementate.
-}
userMode :: IO ()
userMode = humanVsAI step
