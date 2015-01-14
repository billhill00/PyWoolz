#!/usr/bin/Rscript

source('../R/WlzObject.R')

cat('\n\nWoolz version:\n')
WlzVersion()

cat('\n\nReading test objects\n')
lst <- list()
lst[[1]] <- WlzReadObj('test0.wlz')
lst[[2]] <- WlzReadObj('test1.wlz')
lst[[3]] <- WlzReadObj('test2.wlz')

emp <- WlzReadObj('empty.wlz')

cat('\n\nObject types:\n')
typ <- WlzObjectType(lst)
print(typ)
cat('... and as strings ...\n')
str <- WlzObjectTypeToString(typ)
print(str)

cat('\n\nObject types (empty):\n')
typ <- WlzObjectType(emp)
print(typ)
cat('... and as strings ...\n')
str <- WlzObjectTypeToString(typ)
print(str)

cat('\n\nFacts for empty object:\n')
fac <- WlzFacts(emp, 0)
cat(fac)

cat('\n\nFacts for 1st object of the list:\n')
fac <- WlzFacts(lst[[1]], 0)
cat(fac)

cat('\n\nBounding box of objects:\n')
bb <- WlzBoundingBox(lst)
print(bb)

cat('\n\nVolume of objects:\n')
vol <- WlzVolume(lst)
print(vol)

cat('\n\nVolume of intersection of objects:\n')
isn <- WlzIntersect(lst)
vol <- WlzVolume(isn)
print(vol)

cat('\n\nBounding box of intersection of objects:\n')
unn <- WlzIntersect(lst)
bb <- WlzBoundingBox(unn)
print(bb)

cat('\n\nBounding box of union of objects:\n')
unn <- WlzUnion(lst)
bb <- WlzBoundingBox(unn)
print(bb)

cat('\n\nRCC classification of the 1st two object:\n')
cls <- WlzRegConCalcRCC(lst[[1]], lst[[2]], FALSE, FALSE, 0)
print(cls)

