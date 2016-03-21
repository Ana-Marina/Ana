{-# OPTIONS_GHC -F -pgmF htfpp #-}

module AIMinimaxTest where

import Board
import Consecutive
import AIMinimax
import Data.Function (on)
import Data.Char (ord)
import Test.Framework hiding ((><))

instance Consecutive Player where
    (><) = (==)

instance Consecutive a => Consecutive (a, b) where
    (><) = (><) `on` fst

test_maximize :: IO ()
test_maximize = do
    assertEqual 0 $ maximize snd $ expand (const []) (Opponent, 0)
    assertEqual 2 $ maximize snd $ expand f          (Opponent, 0)
  where
    f (_, 0) = [('1', (You, 1)), ('2', (You, 2))]
    f _      = []

test_minimize :: IO ()
test_minimize = do
    assertEqual 0 $ minimize snd $ expand (const []) (Opponent, 0)
    assertEqual 1 $ minimize snd $ expand f          (Opponent, 0)
  where
    f (_, 0) = [('1', (You, 1)), ('2', (You, 2))]
    f _      = []

test_maxmin_alternating :: IO ()
test_maxmin_alternating = do
    assertEqual 5 $ maximize snd $ expand f (Opponent, 0)
  where
    f (_, 0) = [('1', (You, 1)), ('2', (You, 2))]
    f (_, 1) = [('3', (You, 3)), ('4', (You, 4))]
    f (_, 2) = [('5', (You, 5)), ('6', (You, 6))]
    f _      = []

test_maxmin_consecutive :: IO ()
test_maxmin_consecutive = do
    assertEqual 6 $ maximize snd $ expand f (Opponent, 0)
  where
    f (_, 0) = [('1', (You, 1)), ('2', (Opponent, 2))]
    f (_, 1) = [('3', (You, 3)), ('4', (You, 4))]
    f (_, 2) = [('5', (You, 5)), ('6', (You, 6))]
    f _      = []

test_pick_alternating :: IO ()
test_pick_alternating = do
    assertEqual ('2', (You, '2')) $ pick (fromIntegral . ord . snd) $
                                         expand f (Opponent, '0')
  where
    f (_, '0') = [('1', (You, '1')), ('2', (You, '2'))]
    f (_, '1') = [('3', (You, '3')), ('6', (You, '6'))]
    f (_, '2') = [('4', (You, '4')), ('5', (You, '5'))]
    f _        = []

test_pick_consecutive :: IO ()
test_pick_consecutive = do
    assertEqual ('1', (Opponent, '1')) $ pick (fromIntegral . ord . snd) $
                                              expand f (Opponent, '0')
  where
    f (_, '0') = [('1', (Opponent, '1')), ('2', (You, '2'))]
    f (_, '1') = [('3', (You, '3')), ('6', (You, '6'))]
    f (_, '2') = [('4', (You, '4')), ('5', (You, '5'))]
    f _        = []

test_prune :: IO ()
test_prune = do
    -- Arborele ca la `test_pick_consecutive`, dar alegere diferită
    -- la adâncime inferioară.
    assertEqual ('2', (You, '2')) $ pick (fromIntegral . ord . snd) $
                                         prune 1 $ expand f (Opponent, '0')
  where
    f (_, '0') = [('1', (Opponent, '1')), ('2', (You, '2'))]
    f (_, '1') = [('3', (You, '3')), ('6', (You, '6'))]
    f (_, '2') = [('4', (You, '4')), ('5', (You, '5'))]
    f _        = []