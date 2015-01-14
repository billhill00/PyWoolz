#include <R.h>
#include <Rinternals.h>
#include <Wlz.h>

typedef enum _RWlzNObjOpType
{
  RWLZ_NOBJOP_BBOX,
  RWLZ_NOBJOP_EMPTY,
  RWLZ_NOBJOP_INTERSECT,
  RWLZ_NOBJOP_OBJECTTYPE,
  RWLZ_NOBJOP_UNION,
  RWLZ_NOBJOP_VOLUME,
  RWLZ_NOBJOP_XOR
} RWlzNObjOpType;

static void			RWlzFreeObject(
				  SEXP rObj)
{
  WlzObject	*obj;

  if((obj = (WlzObject *)R_ExternalPtrAddr(rObj)) != NULL)
  {
    (void )WlzFreeObj(obj);
    R_ClearExternalPtr(rObj);
  }
}

static void			RWlzError(
				  WlzErrorNum errNum)
{
  const char	*errMsgStr;

  if(errNum != WLZ_ERR_NONE)
  {
    (void )WlzStringFromErrorNum(errNum, &errMsgStr);
    error("Woolz Error -  %s", errMsgStr);
  }
}

static SEXP			RWlzNObjOp(
				  SEXP rObjs,
				  RWlzNObjOpType opt)
{
  int		n = 0;
  SEXP		rtn = R_NilValue;
  WlzObject	*obj = NULL;
  WlzObject	**objs = NULL;
  WlzErrorNum 	errNum = WLZ_ERR_NONE;

  switch(TYPEOF(rObjs))
  {
    case EXTPTRSXP:
      n = 1;
      if((objs = (WlzObject **)AlcMalloc(sizeof(WlzObject *) * n)) == NULL)
      {
        errNum = WLZ_ERR_MEM_ALLOC;
      }
      else
      {
        objs[0] = (WlzObject *)R_ExternalPtrAddr(rObjs);
      }
      break;
    case VECSXP:
      if((n = LENGTH(rObjs)) < 1)
      {
        errNum = WLZ_ERR_PARAM_DATA;
      }
      else if((objs = (WlzObject **)AlcMalloc(sizeof(WlzObject *) * n)) == NULL)
      {
        errNum = WLZ_ERR_MEM_ALLOC;
      }
      else
      {
        int	i;

	for(i = 0; i < n; ++i)
	{
	  objs[i] = (WlzObject *)R_ExternalPtrAddr(VECTOR_ELT(rObjs, i));
	}
      }
      break;
    default:
      errNum = WLZ_ERR_PARAM_DATA;
      break;
  }
  if(errNum == WLZ_ERR_NONE)
  {
    switch(opt)
    {
      case RWLZ_NOBJOP_BBOX:
        {
	  int	i;
	  SEXP	cls,
	  	nam,
		row,
	      	xMin,
		yMin,
		zMin,
	  	xMax,
		yMax,
		zMax;

	  PROTECT(rtn = allocVector(VECSXP, 6));
          PROTECT(cls = allocVector(STRSXP, 1));
	  PROTECT(nam = allocVector(STRSXP, 6));
	  PROTECT(row = allocVector(STRSXP, n));
	  PROTECT(xMin = allocVector(REALSXP, n));
	  PROTECT(yMin = allocVector(REALSXP, n));
	  PROTECT(zMin = allocVector(REALSXP, n));
	  PROTECT(yMax = allocVector(REALSXP, n));
	  PROTECT(xMax = allocVector(REALSXP, n));
	  PROTECT(zMax = allocVector(REALSXP, n));
	  for(i = 0; i < n; ++i)
	  {
	    WlzDBox3 bb;

	    bb = WlzBoundingBox3D(objs[i], &errNum);
	    if(errNum == WLZ_ERR_NONE)
	    {
	      char buf[16];

	      (void )sprintf(buf, "%d", i);
	      SET_STRING_ELT(row, i, mkChar(buf));
	      REAL(xMin)[i] = bb.xMin;
	      REAL(yMin)[i] = bb.yMin;
	      REAL(zMin)[i] = bb.zMin;
	      REAL(xMax)[i] = bb.xMax;
	      REAL(yMax)[i] = bb.yMax;
	      REAL(zMax)[i] = bb.zMax;
	    }
	    else
	    {
	      break;
	    }
	  }
	  if(errNum == WLZ_ERR_NONE)
	  {
            SET_STRING_ELT(cls, 0, mkChar("data.frame"));
	    SET_VECTOR_ELT(rtn, 0, xMin);
            SET_STRING_ELT(nam, 0, mkChar("xMin"));
	    SET_VECTOR_ELT(rtn, 1, yMin);
            SET_STRING_ELT(nam, 1, mkChar("yMin"));
	    SET_VECTOR_ELT(rtn, 2, zMin);
            SET_STRING_ELT(nam, 2, mkChar("zMin"));
	    SET_VECTOR_ELT(rtn, 3, xMax);
            SET_STRING_ELT(nam, 3, mkChar("xMax"));
	    SET_VECTOR_ELT(rtn, 4, yMax);
            SET_STRING_ELT(nam, 4, mkChar("yMax"));
	    SET_VECTOR_ELT(rtn, 5, zMax);
            SET_STRING_ELT(nam, 5, mkChar("zMax"));
            classgets(rtn, cls);
	    namesgets(rtn, nam);
	    setAttrib(rtn, R_RowNamesSymbol, row);
	  }
	  UNPROTECT(10);
        }
	break;
      case RWLZ_NOBJOP_EMPTY:
	{
	  int	i;

	  PROTECT(rtn = allocVector(LGLSXP, n));
	  for(i = 0; (i < n) && (errNum == WLZ_ERR_NONE); ++i)
	  {
	    LOGICAL(rtn)[i] = WlzIsEmpty(objs[i], &errNum);
	  }
	  UNPROTECT(1);
	}
        break;
      case RWLZ_NOBJOP_OBJECTTYPE:
	{
	  int	i;

	  PROTECT(rtn = allocVector(INTSXP, n));
	  for(i = 0; (i < n) && (errNum == WLZ_ERR_NONE); ++i)
	  {
	    INTEGER(rtn)[i] = (objs[i])? (objs[i])->type: WLZ_NULL;
	  }
	  UNPROTECT(1);
	}
        break;
      case RWLZ_NOBJOP_UNION:
        obj = WlzUnionN(n, objs, 0, &errNum);
	rtn = PROTECT(R_MakeExternalPtr(obj, R_NilValue, R_NilValue));
	R_RegisterCFinalizerEx(rtn, RWlzFreeObject, TRUE);
	UNPROTECT(1);
	break;
      case RWLZ_NOBJOP_INTERSECT:
        obj = WlzIntersectN(n, objs, 0, &errNum);
	rtn = PROTECT(R_MakeExternalPtr(obj, R_NilValue, R_NilValue));
	R_RegisterCFinalizerEx(rtn, RWlzFreeObject, TRUE);
	UNPROTECT(1);
	break;
      case RWLZ_NOBJOP_VOLUME:
	{
	  int	i;

	  PROTECT(rtn = allocVector(REALSXP, n));
	  for(i = 0; (i < n) && (errNum == WLZ_ERR_NONE); ++i)
	  {
	    REAL(rtn)[i] = WlzVolume(objs[i], &errNum);
	  }
	  UNPROTECT(1);
	}
        break;
      case RWLZ_NOBJOP_XOR:
	if(n != 2)
	{
	  errNum = WLZ_ERR_PARAM_DATA;
	}
	else
	{
          obj = WlzXORDom(objs[0], objs[1], &errNum);
	}
	rtn = PROTECT(R_MakeExternalPtr(obj, R_NilValue, R_NilValue));
	R_RegisterCFinalizerEx(rtn, RWlzFreeObject, TRUE);
	UNPROTECT(1);
	break;
    }
  }
  AlcFree(objs);
  RWlzError(errNum);
  return(rtn);
}

SEXP				RWlzVersion()
{
  SEXP		rVer;

  PROTECT(rVer = allocVector(STRSXP, 1));
  SET_STRING_ELT(rVer, 0, mkChar(WlzVersion()));
  UNPROTECT(1);
  return(rVer);
}

SEXP 				RWlzFreeObj(
				  SEXP rObj)
{
  RWlzFreeObject(rObj);
  return(R_NilValue);
}

SEXP				RWlzAssignObject(
				  SEXP rObj)
{
  WlzObject     *obj;
  SEXP		nObj;

  if((obj = (WlzObject *)R_ExternalPtrAddr(rObj)) != NULL)
  {
    (void )WlzAssignObject(obj, NULL);
  }
  nObj = PROTECT(R_MakeExternalPtr(obj, R_NilValue, R_NilValue));
  R_RegisterCFinalizerEx(nObj, RWlzFreeObject, TRUE);
  UNPROTECT(1);
  return(nObj);
}

SEXP				RWlzReadObj(
				  SEXP rPath)
{
  const char 	*cPath = NULL;
  FILE		*fP = NULL;
  WlzObject	*obj = NULL;
  WlzErrorNum 	errNum = WLZ_ERR_NONE;
  SEXP		rObj;

  if(TYPEOF(rPath) == RAWSXP)
  {
    cPath = (const char* )RAW(rPath);
  }
  else if((TYPEOF(rPath) != STRSXP) || (LENGTH(rPath) < 1))
  {
    errNum = WLZ_ERR_FILE_OPEN;
  }
  else
  {
    cPath = CHAR(STRING_ELT(rPath, 0));
  }
  if(errNum == WLZ_ERR_NONE)
  {
    if((fP = fopen(cPath, "rb")) == NULL)
    {
      errNum = WLZ_ERR_FILE_OPEN;
    }
    else
    {
      obj = WlzReadObj(fP, &errNum);
      (void )fclose(fP);
    }
  }
  RWlzError(errNum);
  rObj = PROTECT(R_MakeExternalPtr(obj, R_NilValue, R_NilValue));
  R_RegisterCFinalizerEx(rObj, RWlzFreeObject, TRUE);
  UNPROTECT(1);
  return(rObj);
}
				  
SEXP				RWlzFacts(
				  SEXP rObj,
				  SEXP rVerb)
{
  char		*str = NULL;
  WlzObject	*obj = NULL;
  SEXP		rStr;
  WlzErrorNum 	errNum = WLZ_ERR_NONE;

  if((obj = (WlzObject *)R_ExternalPtrAddr(rObj)) != NULL)
  {
    int		verb;

    verb = asInteger(rVerb);
    errNum = WlzObjectFacts(obj, NULL, &str, verb);
  }
  PROTECT(rStr = allocVector(STRSXP, 1));
  SET_STRING_ELT(rStr, 0, mkChar(str));
  UNPROTECT(1);
  (void )AlcFree(str);
  RWlzError(errNum);
  return(rStr);
}

SEXP				RWlzUnion(
				  SEXP rObjs)
{
  SEXP		rObj;

  rObj = RWlzNObjOp(rObjs, RWLZ_NOBJOP_UNION);
  return(rObj);
}

SEXP				RWlzIntersect(
				  SEXP rObjs)
{
  SEXP		rObj;

  rObj = RWlzNObjOp(rObjs, RWLZ_NOBJOP_INTERSECT);
  return(rObj);
}

SEXP				RWlzVolume(
				  SEXP rObjs)
{
  SEXP		rObj;

  rObj = RWlzNObjOp(rObjs, RWLZ_NOBJOP_VOLUME);
  return(rObj);
}

SEXP				RWlzXOR(
				  SEXP rObjs)
{
  SEXP		rObj;

  rObj = RWlzNObjOp(rObjs, RWLZ_NOBJOP_XOR);
  return(rObj);
}

SEXP				RWlzIsEmpty(
				  SEXP rObjs)
{
  SEXP		rObj;

  rObj = RWlzNObjOp(rObjs, RWLZ_NOBJOP_EMPTY);
  return(rObj);
}

SEXP				RWlzRegConCalcRCC(
				  SEXP rObj0,
				  SEXP rObj1,
				  SEXP rNoEnc,
				  SEXP rNoOst,
				  SEXP rMaxOst)
{
  int		noEnc,
  		noOst,
		maxOst;
  double	*nrmVol = NULL;
  SEXP		rtn = R_NilValue;
  WlzObject	*obj[2];
  WlzRCCClass	cfn = WLZ_RCC_EMPTY;
  WlzErrorNum 	errNum = WLZ_ERR_PARAM_DATA;

  if(((obj[0] = (WlzObject *)R_ExternalPtrAddr(rObj0)) != NULL) &&
     ((obj[1] = (WlzObject *)R_ExternalPtrAddr(rObj1)) != NULL))
  {
    noEnc = asLogical(rNoEnc);
    noOst = asLogical(rNoOst);
    maxOst = ALG_NINT(asReal(rMaxOst));
    cfn = WlzRegConCalcRCC(obj[0], obj[1], noEnc, noOst, maxOst,
                           NULL, &nrmVol, &errNum);
  }
  if(errNum == WLZ_ERR_NONE)
  {
    int		i;
    SEXP	cls,
		nam,
		row,
		log,
		val;

    PROTECT(rtn = allocVector(VECSXP, 2));
    PROTECT(cls = allocVector(STRSXP, 1));
    PROTECT(nam = allocVector(STRSXP, 2));
    PROTECT(row = allocVector(STRSXP, WLZ_RCCIDX_CNT));
    PROTECT(log = allocVector(LGLSXP, WLZ_RCCIDX_CNT));
    PROTECT(val = allocVector(REALSXP, WLZ_RCCIDX_CNT));
    for(i = 0; i < WLZ_RCCIDX_CNT; ++i)
    {
      unsigned int m;

      m = 1<<i;
      if((m & cfn) == 0)
      {
	LOGICAL(log)[i] = 0;
	REAL(val)[i] = 0.0;
      }
      else
      {
        LOGICAL(log)[i] = 1;
	REAL(val)[i] = nrmVol[i];
      }
      SET_STRING_ELT(row, i, mkChar(WlzStringFromRCC((WlzRCCClass )m, NULL)));
    }
    SET_STRING_ELT(cls, 0, mkChar("data.frame"));
    SET_VECTOR_ELT(rtn, 0, log);
    SET_VECTOR_ELT(rtn, 1, val);
    SET_STRING_ELT(nam, 0, mkChar("set"));
    SET_STRING_ELT(nam, 1, mkChar("value"));
    classgets(rtn, cls);
    namesgets(rtn, nam);
    setAttrib(rtn, R_RowNamesSymbol, row);
    UNPROTECT(6);
  }
  AlcFree(nrmVol);
  RWlzError(errNum);
  return(rtn);
}

SEXP				RWlzBoundingBox(
				  SEXP rObjs)
{
  SEXP		rtn;

  rtn = RWlzNObjOp(rObjs, RWLZ_NOBJOP_BBOX);
  return(rtn);
}

SEXP				RWlzObjectType(
				  SEXP rObjs)
{
  SEXP		rtn;

  rtn = RWlzNObjOp(rObjs, RWLZ_NOBJOP_OBJECTTYPE);
  return(rtn);
}

SEXP				RWlzObjectTypeToString(
				  SEXP rTypes)
{
  int		n;
  SEXP		rtn = R_NilValue;
  WlzErrorNum	errNum = WLZ_ERR_NONE;

  switch(TYPEOF(rTypes))
  {
    case INTSXP:
      if((n = LENGTH(rTypes)) < 1)
      {
        errNum = WLZ_ERR_PARAM_DATA;
      }
      break;
    default:
      errNum = WLZ_ERR_PARAM_DATA;
      break;
  }
  if(errNum == WLZ_ERR_NONE)
  {

    int		t;

    PROTECT(rtn = allocVector(STRSXP, n));
    if(n == 1)
    {
      t = asInteger(rTypes);
      SET_STRING_ELT(rtn, 0, mkChar(WlzStringFromObjTypeValue(t, NULL)));
    }
    else
    {
      int	i,
      		t;
      
      for(i = 0; i < n; ++i)
      {
	t = INTEGER(rTypes)[i];
	SET_STRING_ELT(rtn, i, mkChar(WlzStringFromObjTypeValue(t, NULL)));
      }
    }
    UNPROTECT(1);
  }
  RWlzError(errNum);
  return(rtn);
}

