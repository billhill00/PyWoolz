dyn.load("../src/libRWlz.so")

WlzVersion <- function()
{
  .Call("RWlzVersion");
}

WlzReadObj <- function(path)
{
  .Call("RWlzReadObj", path);
}

WlzFreeObj <- function(obj)
{
  .Call("RWlzFreeObj", obj);
}

WlzAssignObject <- function(obj)
{
  .Call("WlzAssignObject", obj);
}

WlzFacts <- function(obj, verb)
{
  .Call("RWlzFacts", obj, verb);
}

WlzUnion <- function(objs)
{
  .Call("RWlzUnion", objs);
}

WlzIntersect <- function(objs)
{
  .Call("RWlzIntersect", objs);
}

WlzVolume <- function(objs)
{
  .Call("RWlzVolume", objs);
}

WlzXOR <- function(objs)
{
  .Call("RWlzXOR", objs);
}

WlzIsEmpty <- function(objs) 
{
  .Call("RWlzIsEmpty", objs)
}

WlzRegConCalcRCC <- function(obj0, obj1, noEnc, noOst, maxOst)
{
  .Call("RWlzRegConCalcRCC", obj0, obj1, noEnc, noOst, maxOst)
}

WlzBoundingBox <- function(objs) 
{
  .Call("RWlzBoundingBox", objs)
}

WlzObjectType <- function(objs)
{
  .Call("RWlzObjectType", objs)
}

WlzObjectTypeToString <- function(types)
{
  .Call("RWlzObjectTypeToString", types)
}

