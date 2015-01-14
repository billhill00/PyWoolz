#RWoolz#

An R binding to Woolz (https://github.com/ma-tech/Woolz). Woolz is an
efficient image processing system developed mainly for atlas informatics
which has is particularly compute and storage efficient for morphological
and set operations in 2D and 3D.

The binding relies on R and the Woolz libraries which should be build with
position independent code.

To build the binding edit src/Makevars so that the Woolz include and
library files can be found, then

in src/ run
  
    R CMD SHLIB

it should then be possible to run the test R script

    cd Test/
    export LD_LIBRARY_PATH=../src  # or setenv LD_LIBRARY_PATH ../src
    ./test.R  # or R --no-save < test.R

Expected output:

    > source('../R/WlzObject.R')
    > 
    > cat('\n\nWoolz version:\n')


    Woolz version:
    > WlzVersion()
    [1] "1.5.1"
    > 
    > cat('\n\nReading test objects\n')


    Reading test objects
    > lst <- list()
    > lst[[1]] <- WlzReadObj('test0.wlz')
    > lst[[2]] <- WlzReadObj('test1.wlz')
    > lst[[3]] <- WlzReadObj('test2.wlz')
    > 
    > emp <- WlzReadObj('empty.wlz')
    > 
    > cat('\n\nObject types:\n')


    Object types:
    > typ <- WlzObjectType(lst)
    > print(typ)
    [1] 1 1 1
    > cat('... and as strings ...\n')
    ... and as strings ...
    > str <- WlzObjectTypeToString(typ)
    > print(str)
    [1] "WLZ_2D_DOMAINOBJ" "WLZ_2D_DOMAINOBJ" "WLZ_2D_DOMAINOBJ"
    > 
    > cat('\n\nObject types (empty):\n')


    Object types (empty):
    > typ <- WlzObjectType(emp)
    > print(typ)
    [1] 127
    > cat('... and as strings ...\n')
    ... and as strings ...
    > str <- WlzObjectTypeToString(typ)
    > print(str)
    [1] "WLZ_EMPTY_OBJ"
    > 
    > cat('\n\nFacts for empty object:\n')


    Facts for empty object:
    > fac <- WlzFacts(emp, 0)
    > cat(fac)
    Object type: WLZ_EMPTY_OBJ.
    Linkcount: 0.
    > 
    > cat('\n\nFacts for 1st object of the list:\n')


    Facts for 1st object of the list:
    > fac <- WlzFacts(lst[[1]], 0)
    > cat(fac)
    Object type: WLZ_2D_DOMAINOBJ.
    Linkcount: 0.
    Domain type: WLZ_INTERVALDOMAIN_INTVL.
    Linkcount: 1.
    Line bounds: 0 194.
    Column bounds: 0 252.
    No. of intervals: 287
    Values type: WLZ_POINT_VALUES.
    Linkcount: 1.
    Grey type: WLZ_GREY_UBYTE.
    Background type: WLZ_GREY_UBYTE.
    Background value: 0.
    Values line bounds: 0 194
    Values column bounds: 0 252
    Property list NULL.
    > 
    > cat('\n\nBounding box of objects:\n')


    Bounding box of objects:
    > bb <- WlzBoundingBox(lst)
    > print(bb)
      xMin yMin zMin xMax yMax zMax
    0    0    0    0  252  194    0
    1    0    0    0  310  214    0
    2    0    0    0  228  153    0
    > 
    > cat('\n\nVolume of objects:\n')


    Volume of objects:
    > vol <- WlzVolume(lst)
    > print(vol)
    [1] 10099 15434  9971
    > 
    > cat('\n\nVolume of intersection of objects:\n')


    Volume of intersection of objects:
    > isn <- WlzIntersect(lst)
    > vol <- WlzVolume(isn)
    > print(vol)
    [1] 1396
    > 
    > cat('\n\nBounding box of intersection of objects:\n')


    Bounding box of intersection of objects:
    > unn <- WlzIntersect(lst)
    > bb <- WlzBoundingBox(unn)
    > print(bb)
      xMin yMin zMin xMax yMax zMax
    0    2   10    0   41  105    0
    > 
    > cat('\n\nBounding box of union of objects:\n')


    Bounding box of union of objects:
    > unn <- WlzUnion(lst)
    > bb <- WlzBoundingBox(unn)
    > print(bb)
      xMin yMin zMin xMax yMax zMax
    0    0    0    0  310  214    0
    > 
    > cat('\n\nRCC classification of the 1st two object:\n')


    RCC classification of the 1st two object:
    > cls <- WlzRegConCalcRCC(lst[[1]], lst[[2]], FALSE, FALSE, 0)
    > print(cls)
		     set     value
    WLZ_RCC_DC     FALSE 0.0000000
    WLZ_RCC_EC     FALSE 0.0000000
    WLZ_RCC_EQ     FALSE 0.0000000
    WLZ_RCC_PO      TRUE 0.1190831
    WLZ_RCC_TPP    FALSE 0.0000000
    WLZ_RCC_NTPP   FALSE 0.0000000
    WLZ_RCC_TPPI   FALSE 0.0000000
    WLZ_RCC_NTPPI  FALSE 0.0000000
    WLZ_RCC_TSUR   FALSE 0.0000000
    WLZ_RCC_TSURI  FALSE 0.0000000
    WLZ_RCC_NTSUR  FALSE 0.0000000
    WLZ_RCC_NTSURI FALSE 0.0000000
    WLZ_RCC_ENC     TRUE 0.8763244
    WLZ_RCC_ENCI   FALSE 0.0000000
    WLZ_RCC_OST    FALSE 0.0000000



