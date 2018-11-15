#include "basefunc.h"
#include "TCHAR.h"


bool
select(ads_name ss)
{
	for(;;)
	{
	    int flag;
		ACHAR result[128];
		flag = acedGetString(0,_T("Select/One layer/<ALL layer>:"),result);
		if(flag != RTNORM) return false;

		if((wcscmp(result,_T("s")) == 0)||(wcscmp(result,_T("S")) == 0))
        {
		    do
			{
		  	     flag = acedSSGet(NULL,NULL,NULL,NULL,ss);
			     if(flag == RTCAN) 
			     {
			         return false;
				 }
			 }while(flag != RTNORM);

			 AcDbLayerTable *lytblptr;
			 acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytblptr, AcDb::kForRead);
			 AcDbLayerTableRecord *lytblrcdptr;

			 long sslen,cal,ssDelNum;
			 long hasLockLayer =0;
			 ads_name ent;
			 acedSSLength(ss,&sslen);
			 for(ssDelNum = cal = 0;cal < sslen;cal++,ssDelNum++)
			 {						
			      AcDbEntity *EntObj;
				  acedSSName(ss,ssDelNum,ent);
				  AcDbObjectId objId;
				  acdbGetObjectId(objId,ent);
				  acdbOpenAcDbEntity(EntObj,objId,AcDb::kForRead);
				  lytblptr->getAt(EntObj->layer(),lytblrcdptr,AcDb::kForRead);
				  if(lytblrcdptr->isLocked())
				  {							
					 acedSSDel(ent,ss);														
					 hasLockLayer++;	
					 ssDelNum--;
				  }
				  EntObj->close();
				  lytblrcdptr->close();
			 }

			 lytblptr->close();												
			 if(hasLockLayer != 0)  
			 acutPrintf(_T("\n有%ld个物体被锁定，无法选中它们"),hasLockLayer);
			 if(ss)
			 {
			 	acedSSLength(ss,&sslen);
				acutPrintf(_T("\n有%ld个物体被选中"),sslen);
			    return true;
			 }
		 }
		 else if(wcscmp(result,_T("A")) == 0 || wcscmp(result,_T("a")) == 0||wcscmp(result,_T("")) == 0)
		 {
		    ads_name ssUnit;
		    long sslen = 0,cal,ssDelNum = 0;
		    long hasLocked = 0;
		    acedSSGet(_T("A"),NULL,NULL,NULL,ss);
		    acedSSLength(ss,&sslen);
		    if(sslen == 0)
			{
				acutPrintf(_T("\n没有选择任何实体!"));return false;
			}
					 
			AcDbLayerTable *lytblPtr;
			acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytblPtr, AcDb::kForRead);
					 
			for(cal = 0;cal < sslen; cal++,ssDelNum++)
			{								 
				 AcDbEntity *EntityPtr;
				 AcDbObjectId EntityId;
				 AcDbLayerTableRecord *lytblrcdPtr;

				 acedSSName(ss,ssDelNum,ssUnit);
				 acdbGetObjectId(EntityId,ssUnit);
				 acdbOpenAcDbEntity(EntityPtr,EntityId,AcDb::kForRead);
				 lytblPtr->getAt(EntityPtr->layer(),lytblrcdPtr,AcDb::kForRead);

				 if(lytblrcdPtr->isLocked()||lytblrcdPtr->isOff()||lytblrcdPtr->isFrozen())
				 {
					acedSSDel(ssUnit,ss);
					hasLocked++;
					ssDelNum--;
				 }
				 EntityPtr->close();							 
				 lytblrcdPtr->close();

			}//for;
				
			lytblPtr->close();

			if(hasLocked > 0)
			{
				acutPrintf(_T("\n%ld个实体被锁定或冻结或关闭显示."),hasLocked);
			}

            if(ss)
			{
				acedSSLength(ss,&sslen);
			}

			if(sslen > 0)
			{
				acutPrintf(_T("\n选中了%ld个实体"),sslen);
				return true;
			}//end else if;
			else if((wcscmp(result,_T("O")) == 0||wcscmp(result,_T("o")) == 0))
			{					
				int flag;
				long sslen;
				ACHAR layername[128];
				bool exsit;						
			    AcDbLayerTable *lytblPtr;
			    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytblPtr, AcDb::kForRead);
				            						  
			    do
				{
					flag = acedGetString(1,_T("\n请输入层名："),layername);
					if(flag == RTCAN)return false;
					else if(flag != RTNORM)return false;
					exsit = lytblPtr->has(layername);
			    }
				while(!exsit);

			    lytblPtr->close();

				AcDbLayerTableRecord *lytblrcdPtr;
				lytblPtr->getAt(layername,lytblrcdPtr,AcDb::kForRead);						
				if(lytblrcdPtr->isLocked()||lytblrcdPtr->isFrozen()||lytblrcdPtr->isOff())
				{
					acutPrintf(_T("\n%s层被锁定或关闭或冻结."),layername);
					lytblrcdPtr->close();
					return false;
				}
				else
				{
					lytblrcdPtr->close();						
				}
				resbuf layerRb;
				layerRb.restype = 8;
				layerRb.resval.rstring = layername;
				layerRb.rbnext = NULL;
				flag = acedSSGet(_T("X"),NULL,NULL,&layerRb,ss);
				if(flag == RTNORM) acedSSLength(ss,&sslen);
				acutPrintf(_T("\n%s层%ld个实体被选中"),layername,sslen);
				return true;
			}
		}
	}//for
}//select();




BUILDLAYER
addLayer(const ACHAR* layername)//the same function to the makeLayer(char *layername)
{
	int num_color = 0;
	numLayers(num_color);	//get the number of the total layers
	AcCmColor  colorLayer;
	colorLayer.setColorIndex(num_color);

	AcDbLayerTable *ptrLaytbl;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(ptrLaytbl,AcDb::kForWrite);

/*	//{{if the layername exist,calculate new layer name.
	int iIndex = 0;
	char szSuffix[16];
	const int iLen = strlen(layername) + 16;
	char *szTmp = (char*)malloc(iLen);
	strcpy(szTmp,layername);
	while(ptrLaytbl->has(szTmp))//此层名存在的时候.
	{
		memset(szTmp,0,iLen);

		strcpy(szTmp,layername);//restore the szTmp

		memset(szSuffix,0,16);

		sprintf(szSuffix,"_%d",iIndex);

		strcat(szTmp,szSuffix);

		iIndex++;
	}//if the layername exist,calculate new layer name.}}
*/

	if(ptrLaytbl->has(layername) != true)//此层名不存在的时候.
	{
		//if(NULL == layername)
		//{
		//	acutPrintf("\nNew layer");
		//}
		//else
		//{
		//	acutPrintf("\nNew layer,name is %s",layername);
		//}

		AcDbLayerTableRecord *pLatblrcd = new AcDbLayerTableRecord();	//new.
		pLatblrcd->setName(layername);
	    pLatblrcd->setColor(colorLayer);

		AcDbObjectId idlytblrcd;
	    ptrLaytbl->add(idlytblrcd,pLatblrcd);

		//delete pLatblrcd;			//delete.
		pLatblrcd->close();
		ptrLaytbl->close();

		return THE_LAYER_NOT_EXISTED;
	}	
	else
	{
		ptrLaytbl->close();		//close it.

		return THE_LAYER_EXISTED;
	}


/*
	AcDbLayerTableRecord *pLatblrcd = new AcDbLayerTableRecord();	//new.
	pLatblrcd->setName(szTmp);
    pLatblrcd->setColor(colorLayer);

	AcDbObjectId idlytblrcd;
    ptrLaytbl->add(idlytblrcd,pLatblrcd);

	free(szTmp);

	//delete pLatblrcd;  	//delete.
	pLatblrcd->close();
	ptrLaytbl->close();
*/

	return THE_LAYER_NOT_EXISTED;
}




/*
void
makeLayer(char* layername)
{
    int num_color;
    numLayers(num_color);
    AcCmColor  colorLayer;
    colorLayer.setColorIndex(num_color);

	AcDbLayerTableRecord *pLatblrcd = new AcDbLayerTableRecord;
	pLatblrcd->setName(layername);
    pLatblrcd->setColor(colorLayer);

	AcDbLayerTable *ptrLaytbl;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(ptrLaytbl,AcDb::kForWrite);

	if(ptrLaytbl->has(layername) == true)
	{
	    AcDbObjectId idlytblrcd;
		ptrLaytbl->getAt(layername,idlytblrcd);
		ptrLaytbl->close();
		pLatblrcd->close();
		return;
	}
	else
	{
		AcDbObjectId idlytblrcd;
	    ptrLaytbl->add(idlytblrcd,pLatblrcd);
	    pLatblrcd->close();
		ptrLaytbl->close();
		return;
    }
}
*/



void 
setCurrentLayer(const ACHAR *layername)
{
    //AcDbObjectId idlayer;
    //idlayer = getlayerId(layername);
    //
    //acdbHostApplicationServices()->
	//workingDatabase()->setClayer(idlayer);

    struct resbuf rb1,rb2;
    acedGetVar(_T("clayer"),&rb1);
	rb2.restype = rb1.restype;

	rb2.resval.rstring = new ACHAR[wcslen(layername)+1];
	wcscpy(rb2.resval.rstring,layername);
	acedSetVar(_T("clayer"),&rb2);
	acutPrintf(_T("\n设置%s层为当前层！"),layername);

	delete[] rb2.resval.rstring;
}




void
upstring(ACHAR *pName)
{
	//bool flag;
	ACHAR *pStr;
	int strlength = wcslen(pName);
	//pStr = (char *)malloc(strlength+1);
	pStr = pName;
		
	for(;pName < pStr+strlength;pName++)
	{
	     if(islower(*pName))
		 {
			 *pName = toupper(*pName);					                 
		 }
	}
	
	//acutPrintf("\n修改后的层名是:%s\n",pName-strlength);
	//free(pStr);
	return;
}




AcDbObjectId
join2database(AcDbEntity *pEnt)
{
	AcDbObjectId objid;
	AcDbBlockTable *pblktbl;
	AcDbBlockTableRecord *pblktblrcd;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pblktbl,AcDb::kForRead);
	pblktbl->getAt(ACDB_MODEL_SPACE,pblktblrcd,AcDb::kForWrite);
	pblktbl->close();
	//pEnt->upgradeOpen();
	pblktblrcd->appendAcDbEntity(objid,pEnt);
	//pEnt->downgradeOpen();
	pblktblrcd->close();
	return objid;
}




/*
*  设置实体的层.
*/
void 
setlayer(AcDbEntity *pent,ACHAR* layername)
{
	//AcDbGroupIterator *iter;
	AcDbLayerTable *plytbl;
	AcDbObjectId lyrId;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(plytbl,AcDb::kForWrite);
	if( (plytbl->getAt(layername,lyrId)) != Acad::eOk)
	{
		acutPrintf(_T("\ndebug point: setlayer()getAt()"));
		plytbl->close();
		return;
	}
	pent->setLayer(lyrId);		
	plytbl->close();
}




/*
*  设置实体的层.
*/
void
setlayer(ads_name ss_unit,ACHAR* layername)
{
   AcDbObjectId objId;
   acdbGetObjectId(objId,ss_unit);
   AcDbEntity* pent;
   acdbOpenAcDbEntity(pent,objId,AcDb::kForWrite);
   //AcDbGroupIterator *iter;
   AcDbLayerTable *plytbl;
   AcDbObjectId lyrId;
   acdbHostApplicationServices()->workingDatabase()->getSymbolTable(plytbl,AcDb::kForWrite);
   plytbl->getAt(layername,lyrId);
   pent->setLayer(lyrId);			  
   pent->close();
   plytbl->close();
}




/*
*	统计数据库有多少层;
*/
void 
numLayers(int& numLayers)
{	 
	numLayers = 0;
	AcDbLayerTable *plyrtbl;
	AcDbLayerTableIterator *plyrtblitr;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(plyrtbl,AcDb::kForWrite);
	plyrtbl->newIterator(plyrtblitr);//???????
	 
	for(plyrtblitr->start(); !plyrtblitr->done(); plyrtblitr->step())
	{
		numLayers++;
	}

	delete plyrtblitr;
	plyrtbl->close();	
}




void
to_upper()
{
	 //acutPrintf("\n将层名改大写!");
     AcDbLayerTable *lytbl;
	 acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lytbl,AcDb::kForRead);
	      
	 //lytbl->getAt(ACDB_MODEL_SPACE,lytbl_rcd,AcDb::kForRead);	 
     
     AcDbLayerTableIterator *lytblrcditr;
	 lytbl->upgradeOpen();
     lytbl->newIterator(lytblrcditr);
	 lytbl->downgradeOpen();
     lytbl->close();
     
	 ACHAR *playername = new ACHAR(128);
	 AcDbLayerTableRecord *lytbl_rcd = new AcDbLayerTableRecord;
	 for(lytblrcditr->start();!lytblrcditr->done();lytblrcditr->step())
     {
	     //acutPrintf("\n你好!");
	     lytblrcditr->getRecord(lytbl_rcd,AcDb::kForRead);
	     lytbl_rcd->getName(playername);
         upstring(playername);
         lytbl_rcd->upgradeOpen();
	     lytbl_rcd->setName(_T("1"));//此处是不得已而为之,非得先改"1"
	     lytbl_rcd->setName(playername);
	     lytbl_rcd->downgradeOpen();

	     lytbl_rcd->close(); 
	     //acutPrintf("\nclose");
	  }
	  //delete[] playername;//no need to free it,pay attention.
	  
	  //delete lytblrcditr; 
	  //delete lytbl_rcd;
	  return;
}



bool 
isameLayer(ads_name ss_old,ads_name ss_new)
{
   //判断两个实体是否在同一层;
   AcDbLine *pline1,*pline2;
   AcDbObjectId id;
   acdbGetObjectId(id,ss_old);
   acdbOpenObject(pline1,id,AcDb::kForRead);
   acdbGetObjectId(id,ss_new);
   acdbOpenObject(pline2,id,AcDb::kForRead);

   ACHAR *layer1,*layer2;   
   layer1 = pline1->layer();
   layer2 = pline2->layer();
   pline1->close();
   pline2->close();
   int flag = wcscmp(layer1,layer2);

   if(flag == 0)
   {
	   return true;
   }
   else
   {
	   return false;
   }
}




/*
*  取得当天电脑日期;
*/
wstring 
getTodayData()
{
	AcDbDate cdate(AcDbDate::kInitLocalTime);//取此时此刻的日期:月-日-年;
	short ryear,rmonth,rday;
	cdate.getDate(rmonth,rday,ryear);

	//acutPrintf("\n%d.%d.%d",ryear,rmonth,rday);

	int len = 0;
	ACHAR str[16];
	len = wsprintf(str,_T("%d"),ryear);
	len += wsprintf(str+len,_T("-"));

	if(rmonth < 10)
	{
		len += wsprintf(str+len,_T("%d"),0);
	}
	len += wsprintf(str+len,_T("%d"),rmonth);

	len += wsprintf(str+len,_T("-"));

	if(rday < 10)
	{
		len += wsprintf(str+len,_T("%d"),0);
	}
	len += wsprintf(str+len,_T("%d"),rday);

	//acutPrintf("\nafter alt,the string is %s",str);

	wstring s(str);

	return s;
}




/*
*  请用户输入以获得流程单号；
*/
wstring 
getErpID()
{
	wchar_t result[133];// = new char(256);

	//memset(result,0,132);

	if(acedGetString(0,_T("\n请输入流程单号:"),result) !=  RTNORM)
	{
		//acutPrintf("\n流程单号输入错误...");
		wcscpy(result,_T("XXXXXXX"));
	}

	wstring s = result;
	
	//acutPrintf("\nstring s is %s",s.c_str());

	//delete[] result;

	return s;
}





/*
*   
*/
void
getnet()
{
	//open a polyline object.
	ads_name ss;//selection set.
	ads_name ssSel;

	int flag;
	do
	{
		flag = acedSSGet(NULL,NULL,NULL,NULL,ss);
		if(flag == RTCAN)
		{
			acedSSFree(ss);
			return;
		}
	}while(flag != RTNORM);

	ads_name ssUnit;
	acedSSName(ss,0,ssUnit);
	
	AcDbObjectId id;
	acdbGetObjectId(id,ssUnit);

	AcDbEntity *pEnt;
	acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);

	struct resbuf *ptList = NULL;
	struct resbuf *rb;

	AcGePoint3d pt3d;
	AcDbPolyline *pPline;
	if(pEnt->isA() == AcDbPolyline::desc())
	{
		pPline = (AcDbPolyline*)pEnt;
		long len = pPline->numVerts();

		for(int i = len-1; i >= 0; i--)
		{
			pPline->getPointAt(i,pt3d);
			rb = acutNewRb(RT3DPOINT);
			rb->resval.rpoint[X] = pt3d.x;
			rb->resval.rpoint[Y] = pt3d.y;
			rb->resval.rpoint[Z] = pt3d.z;

			rb->rbnext = ptList;
			ptList = rb;
		}

	    acedSSGet(_T("F"), ptList, NULL, NULL, ssSel);			
	}

	acutRelRb(ptList);
	pEnt->close();

	long ssLenSel;
	acedSSLength(ssSel,&ssLenSel);

	if(ssLenSel != 0)
	{
		ads_name ssTmp;
		for(int i = 0; i < ssLenSel; i++)
		{
			acedSSName(ssSel,i,ssTmp);
			setlayer(ssTmp,_T("123"));
		}
	}

	acedSSFree(ssSel);
}




//=============================================
//class CExtentOfSS 求实体集的外框extent
//=============================================

//constructor
CExtentOfSS::CExtentOfSS()
{
	m_xLen = 0;
	m_yLen = 0; //y方向总厂;
	m_minX = 0;
	m_minY = 0;
	m_maxX = 0;
	m_maxY = 0; //最大y值;
	m_minPoint.set(0,0,0); 
	m_maxPoint.set(0,0,0);
};


//destructor
CExtentOfSS::~CExtentOfSS()
{
}


//遍历实体集，获取边界坐标;
bool 
CExtentOfSS::getExtents(IN ads_name ss)
{
	int flag;
	long ssNum,cal;
	//double bigx,bigy,smallx,smally,widthx,widthy;
	ads_name ss_swap;
	AcDbObjectId objId;
	AcDbEntity *pent;
	AcGePoint3d minpt,maxpt;
	AcDbExtents extents;

	flag = acedSSLength(ss,&ssNum);
	if(flag != RTNORM)
	{
		acutPrintf(_T("\n选择集长度不对，为空!"));
		return false;
	}
	for(cal = 0; cal < ssNum; cal++)
	{
		acedSSName(ss,cal,ss_swap);
		acdbGetObjectId(objId,ss_swap);
		acdbOpenAcDbEntity(pent,objId,AcDb::kForRead);
		flag = pent->getGeomExtents(extents);
		if(flag != Acad::eOk)
		{
			acutPrintf(_T("\n该实体无法获取改实体框框!"));
			pent->close();
			continue;
		}
		minpt = extents.minPoint();
		maxpt = extents.maxPoint();
		if(cal != 0)
		{
			if(maxpt.x > m_maxX) m_maxX = maxpt.x;
			if(maxpt.y > m_maxY) m_maxY = maxpt.y;
			if(minpt.x < m_minX) m_minX = minpt.x;
			if(minpt.y < m_minY) m_minY = minpt.y;
		}
		else if(cal == 0)
		{
			m_maxX = maxpt.x;
			m_maxY = maxpt.y;
			m_minX = minpt.x;
			m_minY = minpt.y;
		}
		pent->close();
	}
	//acedSSFree(ss_swap);
	m_xLen = m_maxX - m_minX;
	m_yLen = m_maxY - m_minY;

	return true;
}



//对一个实体取extents数据
bool 
CExtentOfSS::getExtents(IN const AcDbEntity* pEnt,OUT AcDbExtents& objExt)   
{
	if(pEnt == NULL)
	{
		return false;
	}

	Acad::ErrorStatus es = pEnt->getGeomExtents(objExt); 
	if(es == Acad::eOk)
	{
		return true;
	}
}



//对多个实体取extent
bool
CExtentOfSS::getExtents(IN ads_name ss,OUT AcDbExtents& objExt)
{
	long nLenSS = 0;
	acedSSLength(ss,&nLenSS);
	if(nLenSS <= 0)
	{
		return false;
	}

	AcDbExtents objExtTemp;
	ads_name ssUnit;
	AcDbObjectId entId;
	AcDbEntity* pEnt;
	for(long i = 0; i < nLenSS; i++)
	{
		acedSSName(ss,i,ssUnit);
		if(acdbGetObjectId(entId,ssUnit) == Acad::eOk)
		{
			acdbOpenAcDbEntity(pEnt,entId,AcDb::kForRead);
			if(pEnt->getGeomExtents(objExtTemp) == Acad::eOk)
			{
				objExt.addExt(objExtTemp);
			}
			else
			{
				acutPrintf(_T("\n CExtentOfSS::getExtents(IN ads_name ss,OUT AcDbExtents& objExt)"));
			}
			pEnt->close();  //close;
		}
	}
	return true;
}


//对实体指针集合取extents;
bool
CExtentOfSS::getExtents(IN const AcDbVoidPtrArray pEntArr,OUT AcDbExtents& objExt)
{
	int nlen = pEntArr.length();
	if(nlen <= 0)
	{
		return false;
	}

	AcDbExtents objExtTemp;
	AcDbEntity* pEnt;
	int ei = 0;
	for(int i = 0; i < nlen; i++)
	{
		void * pvoid = pEntArr.at(i);
		if(pvoid != NULL)
		{
			pEnt = (AcDbEntity*)pvoid;
			if(pEnt->getGeomExtents(objExtTemp) == Acad::eOk)
			{
				objExt.addExt(objExtTemp);
				ei++;  //统计有效个数;
			}
		}
	}

	//check
	if(ei == 0)
	{
		return false;
	}
	return true;
}


//
double
CExtentOfSS::getLenX(IN double& x)   
{
	x = m_xLen;  
	return m_xLen;   
}


//
double CExtentOfSS::getLenY(IN double& y)  
{
	y = m_yLen;
	return m_yLen;
}


//
AcGePoint3d
CExtentOfSS::getMaxPoint(OUT AcGePoint3d& pt3d)
{
	pt3d.set(m_maxX,m_maxY,0);
	return pt3d;
}


//检测AcDbExtents对象是否有效
//只需要检测右上点坐标是否大于左下点坐标即可;
bool
CExtentOfSS::isExtEffect(IN const AcDbExtents& objExt)
{
	if(objExt.minPoint().x >= objExt.maxPoint().x)
	{
		return false;
	}

	if(objExt.minPoint().y >= objExt.maxPoint().y)
	{
		return false;
	}

	return true;
}


//
AcGePoint3d
CExtentOfSS::getMinPoint(OUT AcGePoint3d& pt3d)
{
	pt3d.set(m_minX,m_minY,0);
	return pt3d;
}


//测试
void test_extentes()
{	
	ads_name ss;
	if(select(ss) != true)
	{
		acutPrintf(_T("\nselect nothing\n"));
		return;
	}

	//time
	CTimeElapse objElapsedTime;

	CExtentOfSS objExtent;
	//测试一
	objExtent.getExtents(ss);
	double x = 0;
	x = objExtent.getLenX(x);
	double y = 0;
	y = objExtent.getLenY(y);
	acutPrintf(_T("\n 长度x=%f,宽度y=%f"),x,y);  

	AcGePoint3d ptMin;
	AcGePoint3d ptMax;
	objExtent.getMinPoint(ptMin);
	objExtent.getMaxPoint(ptMax);
	acutPrintf(_T("\n 右上坐标（%f,%f）"),ptMax.x,ptMax.y);
	acutPrintf(_T("\n 左下坐标（%f,%f）"),ptMin.x,ptMin.y);

	//acedSSFree(ss);

	//测试二
	AcDbExtents objExt;
	bool bflag = false;
	bflag = objExtent.getExtents(ss,objExt);  
	if(bflag)
	{		
		acutPrintf(_T("\n 右上坐标（%f,%f）"),objExt.maxPoint().x,objExt.maxPoint().y);
		acutPrintf(_T("\n 左下坐标（%f,%f）"),objExt.minPoint().x,objExt.minPoint().y);
	}


	//测试三：测试实体指针集合;
	AcDbExtents objExt0a;
	AcDbObjectId entId;
	ads_name ssUnit;   
	AcDbEntity* pEnt;
	AcDbVoidPtrArray ptrArr;  
	long nlen = 0;
	acedSSLength(ss,&nlen);
	for(long i = 0; i < nlen; i++)   
	{
		acedSSName(ss,i,ssUnit);
		acdbGetObjectId(entId,ssUnit);
		acdbOpenAcDbEntity(pEnt,entId,AcDb::kForRead);
		ptrArr.append(pEnt);		
	}

	bflag = false;
	bflag = objExtent.getExtents(ptrArr,objExt0a);
	if(bflag)
	{		
		acutPrintf(_T("\n 右上坐标（%f,%f）"),objExt.maxPoint().x,objExt.maxPoint().y);
		acutPrintf(_T("\n 左下坐标（%f,%f）"),objExt.minPoint().x,objExt.minPoint().y); 
	}

	acedSSFree(ss);
}



bool
CSomePublicFun::isCstringDigit(IN const CString &szStr)
{
	for(int i = 0; i < szStr.GetLength(); i++)
	{
		//check +  - sign
		if(0 == i && szStr.GetAt(i) == 0x2B || szStr.GetAt(i) == 0x2D)
		{
			continue;
		}

		//check char
		if(! isdigit(szStr.GetAt(i)) && szStr.GetAt(i) != _T('.'))  
		{
			return false;
		}
	}

	//check 小数点;
	if(szStr.Find(_T('.')) != szStr.ReverseFind(_T('.')))
	{
		return false;
	}

	return true;
}