{-# OPTIONS_GHC -F -pgmF htfpp #-}

module BoardTest where

import Board
import Test.Framework

test_build :: IO ()
test_build = do
    -- board1
    assertEqual ([1 ..  6],  7) $ yourSeeds board1
    assertEqual ([8 .. 13], 14) $ oppsSeeds board1
    assertEqual You $ who board1
    assertBool $ not $ isOver board1
    assertEqual (7, 14) $ scores board1
    -- board2
    assertEqual (replicate 6 0, 100) $ yourSeeds board2
    assertEqual ([8 .. 13], 14) $ oppsSeeds board2
    assertEqual Opponent $ who board2
    assertBool $ isOver board2
    assertEqual (100, 77) $ scores board2
  where
    board1 = build ([1 .. 6], 7) ([8 .. 13], 14) You
    board2 = build (replicate 6 0, 100) ([8 .. 13], 14) Opponent

test_initialBoard :: IO ()
test_initialBoard = do
    assertEqual (replicate 6 4, 0) $ yourSeeds initialBoard
    assertEqual (replicate 6 4, 0) $ oppsSeeds initialBoard
    assertEqual You $ who initialBoard
    assertBool $ not $ isOver initialBoard
    assertEqual (0, 0) $ scores initialBoard

test_move :: IO ()
test_move = do
    -- Mutări triviale
    let yourMove = move 1 trivialBoard
        oppsMove = move 1 yourMove
      in do
        -- Mutarea ta, cu schimbarea rândului
        assertEqual ([0, 1, 0, 0, 0, 0], 0) $ yourSeeds yourMove
        assertEqual ([1, 0, 0, 0, 0, 0], 0) $ oppsSeeds yourMove
        assertEqual Opponent $ who yourMove
        assertBool $ not $ isOver yourMove
        assertEqual (0, 0) $ scores yourMove
        -- Mutarea adversarului, cu menținerea rândului și încheierea jocului
        assertEqual ([0, 1, 0, 0, 0, 0], 0) $ yourSeeds oppsMove
        assertEqual ([0, 0, 0, 0, 0, 0], 1) $ oppsSeeds oppsMove
        assertEqual Opponent $ who oppsMove
        assertBool $ isOver oppsMove
        assertEqual (1, 1) $ scores oppsMove
    -- Înconjurare tablei mai mult de o dată
    let yourMove = move 1 roundaboutBoard in do
        assertEqual ([1, 3, 3, 3, 3, 3], 2) $ yourSeeds yourMove
        assertEqual ([2, 2, 2, 2, 2, 2], 0) $ oppsSeeds yourMove
        assertEqual You $ who yourMove
        assertBool $ not $ isOver yourMove
        assertEqual (2, 0) $ scores yourMove
    -- Captură
    let oppsMove = move 2 captureBoard in do
        assertEqual ([0, 1, 1, 1, 1, 1],  0) $ yourSeeds oppsMove
        assertEqual ([0, 0, 0, 0, 0, 0], 20) $ oppsSeeds oppsMove
        assertEqual You $ who oppsMove
        assertBool $ isOver oppsMove
        assertEqual (5, 20) $ scores oppsMove
    -- Casă eronată
    assertEqual roundaboutBoard $ move 7 roundaboutBoard
    assertEqual roundaboutBoard $ move 0 roundaboutBoard
    -- Casă goală
    assertEqual trivialBoard $ move 2 trivialBoard
    -- Mutare după joc terminat
    let newBoard = move 1 $ move 1 trivialBoard in do
        assertBool $ isOver newBoard
        assertEqual newBoard $ move 1 newBoard
  where
    trivialBoard    = build ([ 1, 0, 0, 0, 0, 0], 0) ([1, 0, 0, 0, 0, 0], 0) You
    roundaboutBoard = build ([19, 1, 1, 1, 1, 1], 0) ([1, 1, 1, 1, 1, 1], 0) You
    captureBoard    = build ([19, 1, 1, 1, 1, 1], 0) ([0, 1, 0, 0, 0, 0], 0) Opponent

test_successors :: IO ()
test_successors = do
    assertListsEqualAsSets [4, 5] $ map fst succs
    let (Just succ) = lookup 4 succs in do
        assertEqual ([0, 0, 0, 0, 2, 1], 1) $ yourSeeds succ
        assertEqual ([1, 0, 0, 0, 0, 0], 0) $ oppsSeeds succ
        assertEqual You $ who succ
        assertBool $ not $ isOver succ
        assertEqual (1, 0) $ scores succ
    let (Just succ) = lookup 5 succs in do
        assertEqual ([0, 0, 0, 3, 0, 1], 0) $ yourSeeds succ
        assertEqual ([1, 0, 0, 0, 0, 0], 0) $ oppsSeeds succ
        assertEqual Opponent $ who succ
        assertBool $ not $ isOver succ
        assertEqual (0, 0) $ scores succ
  where
    board = build ([0, 0, 0, 3, 1, 0], 0) ([1, 0, 0, 0, 0, 0], 0) You
    succs = successors board

test_show :: IO ()
test_show = do
    print initialBoard
    print $ move 4 initialBoard