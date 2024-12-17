module Eval3
  ( eval
  , Env
  )
where

import           AST
import           Monads
import qualified Data.Map.Strict               as M
import           Data.Maybe
import           Data.Strict.Tuple
import           Control.Monad                  ( liftM
                                                , ap
                                                )

-- Entornos
type Env = M.Map Variable Int


-- Entorno nulo
initEnv :: Env
initEnv = M.empty

-- Ejercicio 3.a: Proponer una nueva m\'onada que  
-- lleve una traza de ejecución (además de manejar errores y estado).
-- y dar su instancia de mónada. Llamarla |StateErrorTrace|. 
-- COMPLETAR

-- Mónada estado, con manejo de errores
newtype StateErrorTrace a =
  StateErrorTrace { runStateErrorTrace :: Env -> Either Error (Pair a (Pair Trace Env)) }


-- Para calmar al GHC
instance Functor StateErrorTrace where
  fmap = liftM

instance Applicative StateErrorTrace where
  pure  = return
  (<*>) = ap

-- Ejercicio 2.a: Dar una instancia de Monad para StateError:
instance Monad StateErrorTrace where
  return x = StateErrorTrace (\s -> Right (x :!: ("" :!: s)))
  m >>= f = StateErrorTrace (\s -> case runStateErrorTrace m s of 
                                Left e -> Left e
                                Right (x :!: (t :!: s')) -> case runStateErrorTrace (f x) s' of
                                                      Left e -> Left e
                                                      Right (y :!: (t' :!: s'')) -> Right (y :!: (t ++ t' :!: s'')))

-- Ejercicio 3.b: Resolver en Monad.hs


-- Ejercicio 3.c: Dar una instancia de MonadTrace para StateErrorTrace.
instance MonadTrace StateErrorTrace where
  addTrace t = StateErrorTrace (\s -> Right (():!: (t :!: s)))

-- Ejercicio 3.d: Dar una instancia de MonadError para StateErrorTrace.
-- COMPLETAR
instance MonadError StateErrorTrace where
  throw e = StateErrorTrace (\s -> Left e)

-- Ejercicio 3.e: Dar una instancia de MonadState para StateErrorTrace.
instance MonadState StateErrorTrace where
  lookfor v  = StateErrorTrace (\s -> case M.lookup v s of
                                        Nothing -> Left UndefVar
                                        Just x -> Right (x :!: ("" :!: s)))
  update v i = StateErrorTrace (\s -> Right (() :!: ("" :!: M.insert v i s)))

-- Ejercicio 3.f: Implementar el evaluador utilizando la monada StateErrorTrace.
-- Evalua un programa en el estado nulo

eval :: Comm -> Either Error (Trace, Env)
eval p = (runStateErrorTrace (stepCommStar p) initEnv) >>= \(_ :!: (t :!: s)) -> Right (t, s)

-- Evalua multiples pasos de un comando, hasta alcanzar un Skip
-- stepCommStar :: [dar el tipo segun corresponda]
stepCommStar Skip = return ()
stepCommStar c    = stepComm c >>= \c' -> stepCommStar c'

-- Evalua un paso de un comando
-- stepComm :: [dar el tipo segun corresponda]
stepComm Skip = return Skip
stepComm (Let v e) = do e' <- evalExp e
                        update v e'
                        addTrace ("let " ++ show v ++ " = " ++ show e' ++ "\n")
                        return Skip
stepComm (Seq Skip c2) = stepComm c2
stepComm (Seq c1 c2) = do c1' <- stepComm c1
                          return (Seq c1' c2)
stepComm (IfThenElse b c1 c2) = do b' <- evalExp b
                                   if b' then stepComm c1 else stepComm c2
stepComm (Repeat b c) = do b' <- evalExp b
                           if b' then return (Seq c (Repeat b c)) else return Skip


-- Evalua una expresion 
-- evalIntExp :: [dar el tipo segun corresponda]
evalExp :: (MonadState m, MonadError m) => Exp a -> m a
-- Exp Int
evalExp (Const x) = return x
evalExp (Var v) = lookfor v
evalExp (UMinus n) = do n' <- evalExp n
                        return (-n')
evalExp (Plus a b) = do a' <- evalExp a
                        b' <- evalExp b
                        return (a' + b')
evalExp (Times a b) = do a' <- evalExp a
                         b' <- evalExp b
                         return (a' * b')
evalExp (Minus a b) = do a' <- evalExp a
                         b' <- evalExp b
                         return (a' - b')
evalExp (Div a b) = do a' <- evalExp a
                       b' <- evalExp b
                       if b' == 0 then throw DivByZero
                                  else return (a' `div` b')
-- Exp Bool
evalExp BTrue = return True
evalExp BFalse = return False
evalExp (And a b) = do a' <- evalExp a
                       b' <- evalExp b
                       return (a' && b')
evalExp (Or a b) = do a' <- evalExp a
                      b' <- evalExp b
                      return (a' || b')
evalExp (Not b) = do b' <- evalExp b
                     return (not b')
evalExp (Lt a b) = do a' <- evalExp a
                      b' <- evalExp b
                      return (a' < b')
evalExp (Gt a b) = do a' <- evalExp a
                      b' <- evalExp b
                      return (a' > b')
evalExp (Eq a b) = do a' <- evalExp a
                      b' <- evalExp b
                      return (a' == b')
evalExp (NEq a b) = do a' <- evalExp a
                       b' <- evalExp b
                       return (a' /= b')
evalExp (EAssgn v e) = do e' <- evalExp e
                          update v e'
                          return e'
evalExp (ESeq a b) = do a' <- evalExp a
                        b' <- evalExp b
                        return b'