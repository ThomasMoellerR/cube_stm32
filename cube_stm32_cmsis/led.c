#include "LED.h"

uint8_t fft_flag = 0;
uint8_t flickerFlag = 0;
uint8_t fft_MODE = 0;
uint8_t BrightnessFlag = 0;
uint8_t ADC_MODE = 1;
uint8_t CUBE_MODE = 2;							//光立方的显示模式
uint8_t tf_readflag = 0;
uint8_t tf_readRead = 0;
uint8_t tf_readStop = 0;
uint8_t Light_Red = 20, Light_Green = 0, Light_Blue = 0;			//夜灯颜色变量

CUBE_BYTE rgbCUBE12[CUBE_SIZE * CUBE_SIZE * CUBE_SIZE];

const uint8_t DigitChar[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
const uint16_t ScanTimeData[8] = {ScanTime << 0,ScanTime << 1,ScanTime << 2,ScanTime << 3,
								  ScanTime << 4,ScanTime << 5,ScanTime << 6,ScanTime << 7};


void delay_ms(uint32_t num)
{
	uint32_t i,j;
	for(i = 0; i < num; i++)
	{
		for(j = 0; j < 16800; j++)
		{
			if(2 != CUBE_MODE)
				return;
		}
	}
}

void LayerChoose(uint8_t dat)
{
	if(dat & 0x01)
		Layer0_SET; 
	else
		Layer0_RESET; 
	if(dat & 0x02)
		Layer1_SET; 
	else
		Layer1_RESET;
	if(dat & 0x04)
		Layer2_SET; 
	else
		Layer2_RESET; 
	if(dat & 0x08)
		Layer3_SET; 
	else
		Layer3_RESET; 
}

int16_t const LayerSUM_A[12][4][12] = {{{1595,1451,1307,1163,1019,875,731,587,443,299,155,11},{1607,1463,1319,1175,1031,887,743,599,455,311,167,23},{1619,1475,1331,1187,1043,899,755,611,467,323,179,35},{1631,1487,1343,1199,1055,911,767,623,479,335,191,47}},{{1594,1450,1306,1162,1018,874,730,586,442,298,154,10},{1606,1462,1318,1174,1030,886,742,598,454,310,166,22},{1618,1474,1330,1186,1042,898,754,610,466,322,178,34},{1630,1486,1342,1198,1054,910,766,622,478,334,190,46}},{{1593,1449,1305,1161,1017,873,729,585,441,297,153,9},{1605,1461,1317,1173,1029,885,741,597,453,309,165,21},{1617,1473,1329,1185,1041,897,753,609,465,321,177,33},{1629,1485,1341,1197,1053,909,765,621,477,333,189,45}},{{1592,1448,1304,1160,1016,872,728,584,440,296,152,8},{1604,1460,1316,1172,1028,884,740,596,452,308,164,20},{1616,1472,1328,1184,1040,896,752,608,464,320,176,32},{1628,1484,1340,1196,1052,908,764,620,476,332,188,44}},{{1591,1447,1303,1159,1015,871,727,583,439,295,151,7},{1603,1459,1315,1171,1027,883,739,595,451,307,163,19},{1615,1471,1327,1183,1039,895,751,607,463,319,175,31},{1627,1483,1339,1195,1051,907,763,619,475,331,187,43}},{{1590,1446,1302,1158,1014,870,726,582,438,294,150,6},{1602,1458,1314,1170,1026,882,738,594,450,306,162,18},{1614,1470,1326,1182,1038,894,750,606,462,318,174,30},{1626,1482,1338,1194,1050,906,762,618,474,330,186,42}},{{1589,1445,1301,1157,1013,869,725,581,437,293,149,5},{1601,1457,1313,1169,1025,881,737,593,449,305,161,17},{1613,1469,1325,1181,1037,893,749,605,461,317,173,29},{1625,1481,1337,1193,1049,905,761,617,473,329,185,41}},{{1588,1444,1300,1156,1012,868,724,580,436,292,148,4},{1600,1456,1312,1168,1024,880,736,592,448,304,160,16},{1612,1468,1324,1180,1036,892,748,604,460,316,172,28},{1624,1480,1336,1192,1048,904,760,616,472,328,184,40}},{{1587,1443,1299,1155,1011,867,723,579,435,291,147,3},{1599,1455,1311,1167,1023,879,735,591,447,303,159,15},{1611,1467,1323,1179,1035,891,747,603,459,315,171,27},{1623,1479,1335,1191,1047,903,759,615,471,327,183,39}},{{1586,1442,1298,1154,1010,866,722,578,434,290,146,2},{1598,1454,1310,1166,1022,878,734,590,446,302,158,14},{1610,1466,1322,1178,1034,890,746,602,458,314,170,26},{1622,1478,1334,1190,1046,902,758,614,470,326,182,38}},{{1585,1441,1297,1153,1009,865,721,577,433,289,145,1},{1597,1453,1309,1165,1021,877,733,589,445,301,157,13},{1609,1465,1321,1177,1033,889,745,601,457,313,169,25},{1621,1477,1333,1189,1045,901,757,613,469,325,181,37}},{{1584,1440,1296,1152,1008,864,720,576,432,288,144,0},{1596,1452,1308,1164,1020,876,732,588,444,300,156,12},{1608,1464,1320,1176,1032,888,744,600,456,312,168,24},{1620,1476,1332,1188,1044,900,756,612,468,324,180,36}}};
int16_t const LayerSUM_B[12][4][12] = {{{1643,1499,1355,1211,1067,923,779,635,491,347,203,59},{1655,1511,1367,1223,1079,935,791,647,503,359,215,71},{1667,1523,1379,1235,1091,947,803,659,515,371,227,83},{1679,1535,1391,1247,1103,959,815,671,527,383,239,95}},{{1642,1498,1354,1210,1066,922,778,634,490,346,202,58},{1654,1510,1366,1222,1078,934,790,646,502,358,214,70},{1666,1522,1378,1234,1090,946,802,658,514,370,226,82},{1678,1534,1390,1246,1102,958,814,670,526,382,238,94}},{{1641,1497,1353,1209,1065,921,777,633,489,345,201,57},{1653,1509,1365,1221,1077,933,789,645,501,357,213,69},{1665,1521,1377,1233,1089,945,801,657,513,369,225,81},{1677,1533,1389,1245,1101,957,813,669,525,381,237,93}},{{1640,1496,1352,1208,1064,920,776,632,488,344,200,56},{1652,1508,1364,1220,1076,932,788,644,500,356,212,68},{1664,1520,1376,1232,1088,944,800,656,512,368,224,80},{1676,1532,1388,1244,1100,956,812,668,524,380,236,92}},{{1639,1495,1351,1207,1063,919,775,631,487,343,199,55},{1651,1507,1363,1219,1075,931,787,643,499,355,211,67},{1663,1519,1375,1231,1087,943,799,655,511,367,223,79},{1675,1531,1387,1243,1099,955,811,667,523,379,235,91}},{{1638,1494,1350,1206,1062,918,774,630,486,342,198,54},{1650,1506,1362,1218,1074,930,786,642,498,354,210,66},{1662,1518,1374,1230,1086,942,798,654,510,366,222,78},{1674,1530,1386,1242,1098,954,810,666,522,378,234,90}},{{1637,1493,1349,1205,1061,917,773,629,485,341,197,53},{1649,1505,1361,1217,1073,929,785,641,497,353,209,65},{1661,1517,1373,1229,1085,941,797,653,509,365,221,77},{1673,1529,1385,1241,1097,953,809,665,521,377,233,89}},{{1636,1492,1348,1204,1060,916,772,628,484,340,196,52},{1648,1504,1360,1216,1072,928,784,640,496,352,208,64},{1660,1516,1372,1228,1084,940,796,652,508,364,220,76},{1672,1528,1384,1240,1096,952,808,664,520,376,232,88}},{{1635,1491,1347,1203,1059,915,771,627,483,339,195,51},{1647,1503,1359,1215,1071,927,783,639,495,351,207,63},{1659,1515,1371,1227,1083,939,795,651,507,363,219,75},{1671,1527,1383,1239,1095,951,807,663,519,375,231,87}},{{1634,1490,1346,1202,1058,914,770,626,482,338,194,50},{1646,1502,1358,1214,1070,926,782,638,494,350,206,62},{1658,1514,1370,1226,1082,938,794,650,506,362,218,74},{1670,1526,1382,1238,1094,950,806,662,518,374,230,86}},{{1633,1489,1345,1201,1057,913,769,625,481,337,193,49},{1645,1501,1357,1213,1069,925,781,637,493,349,205,61},{1657,1513,1369,1225,1081,937,793,649,505,361,217,73},{1669,1525,1381,1237,1093,949,805,661,517,373,229,85}},{{1632,1488,1344,1200,1056,912,768,624,480,336,192,48},{1644,1500,1356,1212,1068,924,780,636,492,348,204,60},{1656,1512,1368,1224,1080,936,792,648,504,360,216,72},{1668,1524,1380,1236,1092,948,804,660,516,372,228,84}}};
int16_t const LayerSUM_C[12][4][12] = {{{1691,1547,1403,1259,1115,971,827,683,539,395,251,107},{1703,1559,1415,1271,1127,983,839,695,551,407,263,119},{1715,1571,1427,1283,1139,995,851,707,563,419,275,131},{1727,1583,1439,1295,1151,1007,863,719,575,431,287,143}},{{1690,1546,1402,1258,1114,970,826,682,538,394,250,106},{1702,1558,1414,1270,1126,982,838,694,550,406,262,118},{1714,1570,1426,1282,1138,994,850,706,562,418,274,130},{1726,1582,1438,1294,1150,1006,862,718,574,430,286,142}},{{1689,1545,1401,1257,1113,969,825,681,537,393,249,105},{1701,1557,1413,1269,1125,981,837,693,549,405,261,117},{1713,1569,1425,1281,1137,993,849,705,561,417,273,129},{1725,1581,1437,1293,1149,1005,861,717,573,429,285,141}},{{1688,1544,1400,1256,1112,968,824,680,536,392,248,104},{1700,1556,1412,1268,1124,980,836,692,548,404,260,116},{1712,1568,1424,1280,1136,992,848,704,560,416,272,128},{1724,1580,1436,1292,1148,1004,860,716,572,428,284,140}},{{1687,1543,1399,1255,1111,967,823,679,535,391,247,103},{1699,1555,1411,1267,1123,979,835,691,547,403,259,115},{1711,1567,1423,1279,1135,991,847,703,559,415,271,127},{1723,1579,1435,1291,1147,1003,859,715,571,427,283,139}},{{1686,1542,1398,1254,1110,966,822,678,534,390,246,102},{1698,1554,1410,1266,1122,978,834,690,546,402,258,114},{1710,1566,1422,1278,1134,990,846,702,558,414,270,126},{1722,1578,1434,1290,1146,1002,858,714,570,426,282,138}},{{1685,1541,1397,1253,1109,965,821,677,533,389,245,101},{1697,1553,1409,1265,1121,977,833,689,545,401,257,113},{1709,1565,1421,1277,1133,989,845,701,557,413,269,125},{1721,1577,1433,1289,1145,1001,857,713,569,425,281,137}},{{1684,1540,1396,1252,1108,964,820,676,532,388,244,100},{1696,1552,1408,1264,1120,976,832,688,544,400,256,112},{1708,1564,1420,1276,1132,988,844,700,556,412,268,124},{1720,1576,1432,1288,1144,1000,856,712,568,424,280,136}},{{1683,1539,1395,1251,1107,963,819,675,531,387,243,99},{1695,1551,1407,1263,1119,975,831,687,543,399,255,111},{1707,1563,1419,1275,1131,987,843,699,555,411,267,123},{1719,1575,1431,1287,1143,999,855,711,567,423,279,135}},{{1682,1538,1394,1250,1106,962,818,674,530,386,242,98},{1694,1550,1406,1262,1118,974,830,686,542,398,254,110},{1706,1562,1418,1274,1130,986,842,698,554,410,266,122},{1718,1574,1430,1286,1142,998,854,710,566,422,278,134}},{{1681,1537,1393,1249,1105,961,817,673,529,385,241,97},{1693,1549,1405,1261,1117,973,829,685,541,397,253,109},{1705,1561,1417,1273,1129,985,841,697,553,409,265,121},{1717,1573,1429,1285,1141,997,853,709,565,421,277,133}},{{1680,1536,1392,1248,1104,960,816,672,528,384,240,96},{1692,1548,1404,1260,1116,972,828,684,540,396,252,108},{1704,1560,1416,1272,1128,984,840,696,552,408,264,120},{1716,1572,1428,1284,1140,996,852,708,564,420,276,132}}};


void NO_USE(void)
{
	uint8_t i = 0;
	for (i = 0; i < 12; i++)		//发送12个没有使用的数据
	{
		SM16126_CLK_RESET;
		SM16126_CLK_RESET;
		SM16126_DAT_A_RESET,SM16126_DAT_B_RESET,SM16126_DAT_C_RESET;
		SM16126_CLK_SET;
		SM16126_CLK_SET;
	}
}

void OFF_ALL(void)
{
	uint16_t i = 0;
	
	SM16126_STK_RESET;	
	SM16126_STK_RESET;	
	for (i = 0; i < 192; i++)	
	{
		SM16126_CLK_RESET;
		SM16126_CLK_RESET;
		SM16126_DAT_A_RESET,SM16126_DAT_B_RESET,SM16126_DAT_C_RESET;
		SM16126_CLK_SET;
		SM16126_CLK_SET;
	}
	SM16126_STK_SET;
	SM16126_STK_SET;
}


uint8_t NoUSE = 0;
void sendDelay(void)
{
	NoUSE++;
//	__nop();
//	__nop();
//	__nop();
}

void LED_fft_MODE(uint8_t layer, uint8_t byBIT)
{
	int i = 0, j = 0;
	int k = 0;
	
	SM16126_STK_RESET;	
	SM16126_STK_RESET;	
	for (j = 0; j < 4; j++)		//一共4面
	{
		NO_USE();			//发送12个没有使用的数据		
		for (i = 0; i < 12; i++)
		{
			SM16126_CLK_RESET;
			SM16126_CLK_RESET;
			if(rgbCUBE12[LayerSUM_A[layer][j][i]].G & byBIT) 	SM16126_DAT_B_SET;
			else	SM16126_DAT_B_RESET;
			k++;
			
			SM16126_CLK_SET;
			SM16126_CLK_SET;
			
			SM16126_CLK_RESET;
			SM16126_CLK_RESET;
			if(rgbCUBE12[LayerSUM_A[layer][j][i]].B & byBIT) 	SM16126_DAT_B_SET;
			else 	SM16126_DAT_B_RESET;
			k++;
			
			SM16126_CLK_SET;
			SM16126_CLK_SET;
			
			SM16126_CLK_RESET;
			SM16126_CLK_RESET;
			if(rgbCUBE12[LayerSUM_A[layer][j][i]].R & byBIT) 	SM16126_DAT_B_SET;
			else 	SM16126_DAT_B_RESET;
			k++;

			SM16126_CLK_SET;
			SM16126_CLK_SET;
		}
	}
	for (j = 0; j < 4; j++)		//一共4面
	{
		NO_USE();			//发送12个没有使用的数据		
		for (i = 0; i < 12; i++)
		{
			SM16126_CLK_RESET;
			SM16126_CLK_RESET;
			if(rgbCUBE12[LayerSUM_B[layer][j][i]].G & byBIT) 	SM16126_DAT_B_SET;
			else	SM16126_DAT_B_RESET;
			k++;
		
			SM16126_CLK_SET;
			SM16126_CLK_SET;
			
			SM16126_CLK_RESET;
			SM16126_CLK_RESET;
			if(rgbCUBE12[LayerSUM_B[layer][j][i]].B & byBIT) 	SM16126_DAT_B_SET;
			else 	SM16126_DAT_B_RESET;
			k++;
			
			SM16126_CLK_SET;
			SM16126_CLK_SET;
			
			SM16126_CLK_RESET;
			SM16126_CLK_RESET;
			if(rgbCUBE12[LayerSUM_B[layer][j][i]].R & byBIT) 	SM16126_DAT_B_SET;
			else 	SM16126_DAT_B_RESET;

			SM16126_CLK_SET;
			SM16126_CLK_SET;
		}
	}
	
	for (j = 0; j < 4; j++)		//一共4面
	{
		NO_USE();			//发送12个没有使用的数据		
		for (i = 0; i < 12; i++)
		{
			SM16126_CLK_RESET;
			SM16126_CLK_RESET;
			if(rgbCUBE12[LayerSUM_C[layer][j][i]].G & byBIT) 	SM16126_DAT_B_SET;
			else	SM16126_DAT_B_RESET;
			k++;
		
			SM16126_CLK_SET;
			SM16126_CLK_SET;
			
			SM16126_CLK_RESET;
			SM16126_CLK_RESET;
			if(rgbCUBE12[LayerSUM_C[layer][j][i]].B & byBIT) 	SM16126_DAT_B_SET;
			else 	SM16126_DAT_B_RESET;
			k++;
			
			SM16126_CLK_SET;
			SM16126_CLK_SET;
			
			SM16126_CLK_RESET;
			SM16126_CLK_RESET;
			if(rgbCUBE12[LayerSUM_C[layer][j][i]].R & byBIT) 	SM16126_DAT_B_SET;
			else 	SM16126_DAT_B_RESET;
			k++;

			SM16126_CLK_SET;
			SM16126_CLK_SET;
		}
	}
	SM16126_STK_SET;
	SM16126_STK_SET;
}

void LED_Flash(uint8_t layer, uint8_t byBIT)
{
	int i = 0, j = 0;
	
	if(0 == byBIT || 1 == byBIT)
	{
		SM16126_DAT_A_RESET,SM16126_DAT_B_RESET,SM16126_DAT_C_RESET;
		for (i = 0; i < 192; i++)
		{
			SM16126_CLK_RESET;
			SM16126_DAT_A_RESET,SM16126_DAT_B_RESET,SM16126_DAT_C_RESET;
			sendDelay();
			SM16126_CLK_SET;
		}
	}
	else
	{
		byBIT = DigitChar[byBIT];
		
		SM16126_STK_RESET;
		for (j = 0; j < 4; j++)		//一共4面
		{
			SM16126_DAT_A_RESET,SM16126_DAT_B_RESET,SM16126_DAT_C_RESET;
			//发送12个没有使用的数据
			for (i = 0; i < 12; i++)
			{
				SM16126_CLK_RESET;
				SM16126_DAT_A_RESET,SM16126_DAT_B_RESET,SM16126_DAT_C_RESET;
				sendDelay();
				SM16126_CLK_SET;
			}
			
			for (i = 0; i < 12; i++) // changes: layer = i , i = layer
			{
				SM16126_CLK_RESET;
				if(rgbCUBE12[LayerSUM_A[layer][j][i]].G & byBIT) 	SM16126_DAT_A_SET;
				else	SM16126_DAT_A_RESET;
						
				if(rgbCUBE12[LayerSUM_B[layer][j][i]].G & byBIT)	SM16126_DAT_B_SET;
				else	SM16126_DAT_B_RESET;
			
				if(rgbCUBE12[LayerSUM_C[layer][j][i]].G & byBIT)	SM16126_DAT_C_SET;
				else	SM16126_DAT_C_RESET;
				sendDelay();
				SM16126_CLK_SET;
			
				
				if(rgbCUBE12[LayerSUM_A[layer][j][i]].B & byBIT) 	SM16126_DAT_A_SET;
				else 	SM16126_DAT_A_RESET;
						
				if(rgbCUBE12[LayerSUM_B[layer][j][i]].B & byBIT) 	SM16126_DAT_B_SET;
				else	SM16126_DAT_B_RESET;
				SM16126_CLK_RESET;
						
				if(rgbCUBE12[LayerSUM_C[layer][j][i]].B & byBIT) 	SM16126_DAT_C_SET;
				else 	SM16126_DAT_C_RESET;
				sendDelay();
				SM16126_CLK_SET;
				
				
				SM16126_CLK_RESET;
				if(rgbCUBE12[LayerSUM_A[layer][j][i]].R & byBIT) 	SM16126_DAT_A_SET;
				else 	SM16126_DAT_A_RESET;
						
				if(rgbCUBE12[LayerSUM_B[layer][j][i]].R & byBIT) 	SM16126_DAT_B_SET;
				else 	SM16126_DAT_B_RESET;
				SM16126_CLK_RESET;
						
				if(rgbCUBE12[LayerSUM_C[layer][j][i]].R & byBIT) 	SM16126_DAT_C_SET;
				else 	SM16126_DAT_C_RESET;
				sendDelay();
				SM16126_CLK_SET;
			}
		}
		SM16126_STK_SET;
	}
}

void Set_row(uint8_t layer, uint8_t byBIT)
{
	//LED_fft_MODE(layer, DigitChar[byBIT]);
	LED_Flash(layer, byBIT);
}


void ClearRGB(uint8_t rank_r, uint8_t rank_g, uint8_t rank_b)
{
	uint16_t i = 0;
	for(i = 0; i < rgb_SIZE; i++)
	{
		rgbCUBE12[i].R = rank_r;
		rgbCUBE12[i].G = rank_g;
		rgbCUBE12[i].B = rank_b;
	}
}

void SetPoint(uint8_t x, uint8_t y, uint8_t z, uint8_t R, uint8_t G, uint8_t B)
{
	int i = 0, j = 0;
	x %= 12;						//限制X轴不超过12
	y %= 12;						//限制Y轴不超过12
	z %= 12;						//限制Z轴不超过12
	R %= 256;
	G %= 256;
	B %= 256;
	i = x + y * 12;					//计算当前的点在这层的位置
	j = 144 * z;					//计算当前的点在第几层
	
	rgbCUBE12[i + j].R = R;			//写入当前的点的红色
	rgbCUBE12[i + j].G = G;			//写入当前的点的绿色
	rgbCUBE12[i + j].B = B;			//写入当前的点的蓝色	
}

void setPlaneX(uint8_t x, uint8_t R, uint8_t G, uint8_t B)
{
	uint8_t y, z;
	for (y = 0; y < 12; y++)
	{
		for (z = 0; z < 12; z++)
			SetPoint(x, y, z, R, G, B);
	}
}

void setPlaneY(uint8_t y, uint8_t R, uint8_t G, uint8_t B)
{
	uint8_t x, z;
	for (x = 0; x < 12; x++)
	{
		for (z = 0; z < 12; z++)
			SetPoint(x, y, z, R, G, B);
	}
}

void setPlaneZ(uint8_t z, uint8_t R, uint8_t G, uint8_t B)
{
	uint8_t x, y;
	for (y = 0; y < 12; y++)
	{
		for (x = 0; x < 12; x++)
			SetPoint(x, y, z, R, G, B);
	}
}

void setAll(uint8_t R, uint8_t G, uint8_t B)
{
	int i;
	R %= 256;
	G %= 256;
	B %= 256;
	for (i = 0; i < 1728; i++)
	{
		rgbCUBE12[i].R = R;
		rgbCUBE12[i].G = G;
		rgbCUBE12[i].B = B;
	}
}


/*******************************************************************************
 * @name     :int8_t abss(int8_t dat)
 * @param    :dat→需要转换的数据
 * @author   :图腾实验室→图腾
 * @details  :四舍五入，返回十位的数值
*******************************************************************************/
int8_t abss(int8_t dat)
{
	int8_t tpdat = 0;
	tpdat = dat / 10;
	dat = dat - tpdat * 10;
	if (dat >= 5)
		tpdat++;
	return tpdat;
}

/*******************************************************************************
 * @name     :int8_t abs_bj(int8_t dat)
 * @param    :dat→需要转换的数据
 * @author   :图腾实验室→图腾
 * @details  :取数据的绝对值
*******************************************************************************/
int8_t MYabs(int8_t dat)
{
	if (dat <= 0)
		dat = -dat;
	else
		dat = dat;
	return dat;
}

/*******************************************************************************
 * @name     :void Max(int8_t *a,int8_t *b)
 * @param    :*a→需要转换的数据
 * @param    :*b→需要转换的数据
 * @author   :图腾实验室→图腾
 * @details  :比较函数，大的数值置前，指针函数，直接对原数值操作
*******************************************************************************/
void Max(int8_t *a, int8_t *b)
{
	if ((*a) > (*b))
	{
		*a = *a ^ *b;
		*b = *b ^ *a;
		*a = *a ^ *b;
		//*a = *a + *b - (*b = *a);
	}
}

/*******************************************************************************
 * @name     :int8_t maxt(int8_t datA,int8_t datB,int8_t datC)
 * @param    :datA→需要比较的数据
 * @param    :datB→需要比较的数据
 * @param    :datC→需要比较的数据
 * @author   :图腾实验室→图腾
 * @details  :3个数据比较，返回最大值
*******************************************************************************/
int8_t maxt(int8_t datA, int8_t datB, int8_t datC)
{
	if (datA <= datB)
		datA = datB;
	if (datA <= datC)
		datA = datC;
	return datA;
}


/*******************************************************************************
 * @name     :void Line(int8_t x1,int8_t y1,int8_t z1,int8_t x2,int8_t y2,int8_t z2,int8_t red,int8_t green,int8_t blue,int8_t Add_Cut)
 * @param    :red→红色的大小(范围0--255)
 * @param    :green→绿色的大小(范围0--255)
 * @param    :blue→蓝色的大小(范围0--255)
 * @param    :Add_Cut→增加还是减少，0为减少，其他为增加
 * @author   :图腾实验室→图腾
 * @details  :3维坐标中画线
*******************************************************************************/
void line(int8_t x1, int8_t y1, int8_t z1, int8_t x2, int8_t y2, int8_t z2, uint8_t red, uint8_t green, uint8_t blue)
{
	int8_t mDat = 0, SumX = 0, SumY = 0, SumZ = 0, poorX = 0, poorY = 0, poorZ = 0;
	int8_t i = 0;
	poorX = x2 - x1;
	poorY = y2 - y1;
	poorZ = z2 - z1;
	mDat = maxt(MYabs(poorX), MYabs(poorY), MYabs(poorZ));
	SumX = x1 * 10;
	SumY = y1 * 10;
	SumZ = z1 * 10;
	poorX = poorX * 10 / mDat;
	poorY = poorY * 10 / mDat;
	poorZ = poorZ * 10 / mDat;
	for (i = 0; i < mDat; i++)
	{
		SetPoint(abss(SumX), abss(SumY), abss(SumZ), red, green, blue);
		SumX = SumX + poorX;
		SumY = SumY + poorY;
		SumZ = SumZ + poorZ;
	}
	SetPoint(x2, y2, z2, red, green, blue);
}





void NVIC_Configuration(void) 
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);			//使能时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);							//初始化
	
	GPIO_SetBits(GPIOB,GPIO_Pin_13);					//GPIOB13设置高，灯灭

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);			//使能时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | 
								  GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);							//初始化

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);			//使能时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);							//初始化

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);			//使能时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);							//初始化
}


