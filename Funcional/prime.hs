isPrime :: Int -> Bool
isPrime n
  | n <= 1 = False
  | otherwise = not (any (\x -> n `mod` x == 0) [2 .. (n - 1)])

main :: IO ()
main = do
  putStrLn "Enter a number to check if it is prime:"
  input <- getLine
  let n = read input :: Int
  if isPrime n
    then putStrLn (show n ++ " its prime")
    else putStrLn (show n ++ " is not prime")
