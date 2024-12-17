{-
Implementar un programa que lee un archivo y guarda sus contenidos pasado a mayúsculas en otro archivo.
Los nombres de los archivos de entrada y salida se daran como argumento en la linea de comandos.
-}
import System.Environment (getArgs)
import System.IO (readFile, writeFile)
import Data.Char (toUpper)

main :: IO ()
main = do
    args <- getArgs -- Obtener argumentos de la linea de comandos
    case args of
        [inputFile, outputFile] -> do
            content <- readFile inputFile
            let upperContent = map toUpper content
                in do writeFile outputFile upperContent
                      putStrLn "Archivo procesado y guardado en mayúsculas."
        _ -> putStrLn "De el archivo de entrada y salida como argumentos!"
