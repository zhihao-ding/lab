-- file: WordBreak.hs

import Data.List(isPrefixOf)

str = "dogandcat"
dict = ["do", "d", "gand", "g", "and", "dog", "cat", "c", "at"]

getStarts :: String -> [String]
getStarts [] = []
getStarts str = filter (`isPrefixOf` str) dict

data Breaker = Middle {before :: [String], left :: String} | Result {result :: [String]} | Empty
                deriving (Eq, Show)

isFinished :: [Breaker] -> Bool
isFinished [] = True
isFinished (x:xs) =
    case x of
        Empty -> isFinished xs
        Result r -> isFinished xs
        Middle b l -> False

filterEmpty :: [Breaker] -> [Breaker]
filterEmpty [] = []
filterEmpty (x:xs) =
    case x of
        Empty -> filterEmpty xs
        _ -> x : filterEmpty xs

breakOne :: Breaker -> [Breaker]
breakOne Empty = []
breakOne (Result r) = [Result r]
breakOne (Middle b l)
    | null l = [Result b]
    | otherwise =
        let starts = getStarts l
            in if (length starts) == 0 then [Empty]
               else map (\x -> Middle (b ++ [x]) (drop (length x) l)) starts

breakMore :: [Breaker] -> [Breaker]
breakMore [] = []
breakMore (x:xs) = (breakOne x) ++ (breakMore xs)

breakAll :: [Breaker] -> [Breaker]
breakAll [] = []
breakAll xs =
    if isFinished xs then xs
    else breakAll (breakMore xs)

doBreak :: String -> [Breaker]
doBreak [] = []
doBreak str = filterEmpty (breakAll begin)
    where begin = breakOne (Middle [] str)

wordBreak :: String -> [[String]]
wordBreak [] = []
wordBreak str = map (\x -> result x) (doBreak str)

