-- file: wordbreak.hs

import Data.List(isPrefixOf)

str = "dogandcat"
dict = ["do", "d", "gand", "and", "dog", "cat", "c", "at"]

getStarts :: String -> [String]
getStarts [] = []
getStarts str = filter (`isPrefixOf` str) dict

getEnds :: String -> [Breaker]
getEnds [] = [Empty]
getEnds str 
    | (length (getStarts str)) == 0 = [Empty]
    | otherwise = map (\x -> Node [x] (drop (length x) str)) (getStarts str)

data Breaker = Node {result :: [String], left :: String} | Empty
                deriving (Eq, Show)

middleHelp :: Breaker -> [Breaker]
middleHelp Empty = [Empty]
middleHelp (Node r l)
    | null l = [Node r l]
    | (not (null l)) && (length (getStarts l)) == 0 = [Empty]
    | otherwise = map (\x -> Node (r ++ (result x)) (left x)) (getEnds l)

middleHelpWrapper :: [Breaker] -> [Breaker]
middleHelpWrapper breakers =
    let middles = map middleHelp breakers
    in if (length middles) == 0
       then []
       else clearBreakers (foldr (++) [] middles)

clearBreakers :: [Breaker] -> [Breaker]
clearBreakers [] = []
clearBreakers breakers = filter (\x -> not (x == Empty)) breakers

getMiddle str = middleHelpWrapper (getEnds str)

lastHelp :: [Breaker] -> [Breaker]
lastHelp [] = []
lastHelp breakers =
    let userfull = clearBreakers breakers
    in let unfinished = filter (\x -> (length (left x)) > 0) userfull
       in if (length unfinished) == 0
          then userfull
          else lastHelp (clearBreakers (middleHelpWrapper userfull))

getLast str = lastHelp (getMiddle str)


wordBreak str = map (\x -> result x) (getLast str)

