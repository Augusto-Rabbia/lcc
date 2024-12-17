module Eval2
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

-- Mónada estado, con manejo de errores
newtype StateError a =
  StateError { runStateError :: Env -> Either Error ( Pair a Env) }


-- Para calmar al GHC
instance Functor StateError where
  fmap = liftM

instance Applicative StateError where
  pure  = return
  (<*>) = ap

-- Ejercicio 2.a: Dar una instancia de Monad para StateError:
instance Monad StateError where
  return x = StateError (\s -> Right (x :!: s))
  m >>= f = StateError (\s -> case runStateError m s of 
                                Left e -> Left e
                                Right (x :!: s') -> runStateError (f x) s')

-- Ejercicio 2.b: Dar una instancia de MonadError para StateError:
instance MonadError StateError where
  throw e = StateError (\_ -> Left e)

-- Ejercicio 2.c: Dar una instancia de MonadState para StateError:
instance MonadState StateError where
  -- lookfor :: Variable -> m Int
  lookfor v = StateError (\s -> case M.lookup v s of
                                  Just v -> Right (v :!: s)
                                  Nothing -> Left UndefVar)
  -- update :: Variable -> Int -> m ()
  update v i = StateError (\s -> Right (():!: M.insert v i s))

-- Ejercicio 2.d: Implementar el evaluador utilizando la monada StateError.
-- Evalua un programa en el estado nulo
eval :: Comm -> Either Error Env
eval p = (runStateError (stepCommStar p) initEnv) >>= \(_ :!: s) -> Right s

-- Evalua multiples pasos de un comando, hasta alcanzar un Skip
stepCommStar :: (MonadState m, MonadError m) => Comm -> m ()
stepCommStar Skip = return ()
stepCommStar c    = stepComm c >>= \c' -> stepCommStar c'

-- Evalua un paso de un comando
stepComm :: (MonadState m, MonadError m) => Comm -> m Comm
stepComm Skip = return Skip
stepComm (Let v e) = do e' <- evalExp e
                        update v e'
                        return Skip
stepComm (Seq Skip c2) = stepComm c2
stepComm (Seq c1 c2) = do c1' <- stepComm c1
                          return (Seq c1' c2)
stepComm (IfThenElse b c1 c2) = do b' <- evalExp b
                                   if b' then stepComm c1 else stepComm c2
stepComm (Repeat b c) = do b' <- evalExp b
                           if b' then return (Seq c (Repeat b c)) else return Skip



-- Evalua una expresion
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



