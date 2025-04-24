x :: Integer
x = 2

doubleIt :: Integer
doubleIt = x * 2

doubleItAgain :: Integer
doubleItAgain = doubleItAgain * 2

main :: IO ()
main = do
  putStrLn "Hello, Haskell!"
