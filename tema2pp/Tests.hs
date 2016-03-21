{-# OPTIONS_GHC -F -pgmF htfpp #-}

module Tests where

import Test.Framework

import {-@ HTF_TESTS @-} BoardTest
import {-@ HTF_TESTS @-} AISimpleTest
import {-@ HTF_TESTS @-} AIMinimaxTest

main :: IO ()
main = htfMain htf_importedTests