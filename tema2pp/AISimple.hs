module AISimple where

import Board
import Interactive
import Data.List
{-
    *** TODO ***

    Întoarce tabla rezultată din aplicarea acelei mutări care maximizează
    scorul adversarului.
-}

instance Ord Board where
    compare b1 b2 = compare (snd . scores $ b1) (snd . scores $ b2)

step :: Board -> (House, Board)
-- mutarea cu scorul maxim pentru opponent
step board = last (sortBy (\e1 e2 -> compare (snd e1) (snd e2)) (successors board))

{-
    Urmărește pas cu pas evoluția jocului, conform strategiei implementate.
-}
userMode :: IO ()
userMode = humanVsAI step
