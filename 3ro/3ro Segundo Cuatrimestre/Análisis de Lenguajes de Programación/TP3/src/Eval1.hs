module Eval1
  ( eval
  , Env
  )
where

import           AST
import           Monads
import qualified Data.Map.Strict               as M
import           Data.Maybe
import           Prelude                 hiding ( fst
                                                , snd
                                                )
import           Data.Strict.Tuple
import           Control.Monad                  ( liftM
                                                , ap
                                                )

-- Entornos
type Env = M.Map Variable Int

-- Entorno nulo
initEnv :: Env
initEnv = M.empty

-- Mónada estado
newtype State a = State { runState :: Env -> Pair a Env }

instance Monad State where
  return x = State (\s -> (x :!: s))
  m >>= f = State (\s -> let (v :!: s') = runState m s in runState (f v) s')

-- Para calmar al GHC
instance Functor State where
  fmap = liftM

instance Applicative State where
  pure  = return
  (<*>) = ap

instance MonadState State where
  lookfor v = State (\s -> (lookfor' v s :!: s))
    where lookfor' v s = fromJust $ M.lookup v s
  update v i = State (\s -> (() :!: update' v i s)) where update' = M.insert

-- Ejercicio 1.b: Implementar el evaluador utilizando la monada State

-- Evalua un programa en el estado nulo
eval :: Comm -> Env
eval p = snd (runState (stepCommStar p) initEnv)

-- Evalua multiples pasos de un comando, hasta alcanzar un Skip
stepCommStar :: MonadState m => Comm -> m ()
stepCommStar Skip = return ()
stepCommStar c    = stepComm c >>= \c' -> stepCommStar c'

-- Evalua un paso de un comando
stepComm :: MonadState m => Comm -> m Comm
stepComm Skip = return Skip
stepComm (Let v e) = do e' <- evalExp e -- e = EASSIGN x 5
                        update v e' -- e' = 3
                        return Skip
stepComm (Seq Skip c2) = stepComm c2
stepComm (Seq c1 c2) = do c1' <- stepComm c1
                          return (Seq c1' c2)
stepComm (IfThenElse b c1 c2) = do b' <- evalExp b
                                   if b' then stepComm c1 else stepComm c2
stepComm (Repeat b c) = do b' <- evalExp b
                           if b' then return (Seq c (Repeat b c)) else return Skip

-- Evalua una expresion
evalExp :: MonadState m => Exp a -> m a
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
                       return (a' `div` b')
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

