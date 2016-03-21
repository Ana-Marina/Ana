{-# OPTIONS_GHC -F -pgmF htfpp #-}

module AISimpleTest where

import Board
import AISimple
import Test.Framework

test_step :: IO ()
test_step = do
    assertEqual 2 house
    assertEqual ([0, 0, 0, 0, 0, 0], 0) $ yourSeeds succ
    assertEqual ([0, 0, 3, 0, 0, 0], 2) $ oppsSeeds succ
    assertEqual You $ who succ
    assertBool $ isOver succ
    assertEqual (0, 5) $ scores succ
  where
    board         = build ([1, 0, 0, 0, 0, 0], 0) ([0, 1, 3, 0, 0, 0], 0) Opponent
    (house, succ) = step board