#include "AjyLib.h"

namespace Ajy
{

	/*--------------------copy from others*/

#ifdef __GNUC__
	int32 Clz(size_t x)
	{
#ifdef Ajy_X64
		return __builtin_clzl(x);
#else
		return __builtin_clz(x);
#endif
	}
#elif defined(Ajy_IA) && defined(_MSC_VER)
	int32 Clz(size_t x)
	{
		ulong r = 0;
#ifdef Ajy_X64
		_BitScanReverse64(&r, x);
		return 63 - r;
#else
		_BitScanReverse(&r, x);
		return 31 - r;
#endif
	}
#else
#ifdef Ajy_X64
#error not support
#endif
	int32 Popcnt(size_t x)
	{
		x -= ((x >> 1) & 0x55555555);
		x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
		x = (((x >> 4) + x) & 0x0f0f0f0f);
		x += (x >> 8);
		x += (x >> 16);
		return x & 0x0000003f;
	}
	int32 Clz(size_t x)
	{
		x |= (x >> 1);
		x |= (x >> 2);
		x |= (x >> 4);
		x |= (x >> 8);
		x |= (x >> 16);
		return 32 - Popcnt(x);
	}
#endif

	// 返回刚好大于 x 的 2^n 的值用于内存分配
	// 如果 x 本身 2^n 系 则返回原值
	size_t Round2n(size_t len)
	{
#ifdef Ajy_X64
		int32 bits = 63 - Clz(len);
#else
		int32 bits = 31 - Clz(len);
#endif
		size_t rtv = size_t(1) << bits;
		if (rtv == len) return len;
		return rtv << 1;
	}

	// 算一个数是 2 的几次方
	size_t Calc2n(size_t len)
	{
#ifdef Ajy_X64
		return 63 - Clz(len);
#else
		return 31 - Clz(len);
#endif
	}

	size_t GetHashCode_CS(uint8 const* buf, size_t len)
	{
#ifndef Ajy_IA
		assert((size_t)buf % 4 == 0);
#endif
		if (!len)
		{
			return 0;
		}
		int32 n2 = 0x15051505, n1 = 0x15051505, mod = len % 8, i = 0;
		for (; i < len - mod; i += 8)
		{
			n1 = (((n1 << 5) + n1) + (n1 >> 0x1b)) ^ *(int32*)(buf + i);
			n2 = (((n2 << 5) + n2) + (n2 >> 0x1b)) ^ *(int32*)(buf + i + 4);
		}
		if (mod > 4)
		{
			n1 = (((n1 << 5) + n1) + (n1 >> 0x1b)) ^ *(int32*)(buf + i);
			n2 = (((n2 << 5) + n2) + (n2 >> 0x1b)) ^ (*(int32*)(buf + i + 4) & (0xFFFFFFFF >> ((8 - mod) * 8)));
		}
		else if (mod)
		{
			n1 = (((n1 << 5) + n1) + (n1 >> 0x1b)) ^ (*(int32*)(buf + i) & (0xFFFFFFFF >> ((4 - mod) * 8)));
		}
		return n2 + n1 * 0x5d588b65;
	}

	size_t GetHashCode_Lua(uint8 const* buf, size_t len)
	{
		if (!len)
		{
			return 0;
		}
		size_t seed = 131, hash = 0;
		for (size_t i = 0; i < len; ++i)
		{
			hash = hash * seed + (uint8)buf[i];
		}
		return hash;
	}

	size_t GetHashCode(std::string const& in)
	{
		return GetHashCode_CS((uint8 const*)in.c_str(), in.size());
	}

	size_t GetHashCode(uint64 in)
	{
		return (size_t)((in & 0xFFFFFFFFu) ^ (in >> 32));
	}

	size_t GetHashCode(int64 in)
	{
		return GetHashCode((uint64)in);
	}

	size_t GetHashCode(float in)
	{
		return *(size_t*)(float*)&in;
	}

	size_t GetHashCode(double in)
	{
		return GetHashCode(*(uint64*)(double*)&in);
	}

	size_t GetHashCode(int32 in)
	{
		return (size_t)in;
	}
	size_t GetHashCode(uint32 in)
	{
		return (size_t)in;
	}
	size_t GetHashCode(int8 in)
	{
		return (size_t)in;
	}
	size_t GetHashCode(uint8 in)
	{
		return (size_t)in;
	}
	size_t GetHashCode(int16 in)
	{
		return (size_t)in;
	}
	size_t GetHashCode(uint16 in)
	{
		return (size_t)in;
	}
	size_t GetHashCode(void*	in)
	{
		return ((size_t)(void*)in / (sizeof(size_t) * 2));
	};

	/*--------------------copy from others*/

	// 1453 个 2G 内, 8 - 512  dataSize 亲和的质数表
	static const int32 primes[] =
	{ 7, 11, 13, 17, 19, 23, 31, 37, 43, 47, 53, 59, 67, 71, 73, 79, 83, 89, 97, 103, 107, 109, 113, 131, 139, 151, 157, 163, 167, 173, 179, 181, 191, 193, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 271, 277, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 463, 467, 479, 487, 491, 499, 503, 509, 523, 541, 547, 557, 563, 571, 577, 587, 599, 607, 613, 619, 631, 647, 653, 661, 677, 683, 691, 701, 709, 719, 727, 733, 743, 751, 757, 761, 773, 787, 797, 811, 823, 829, 839, 853, 863, 877, 887, 911, 919, 929, 941, 947, 953, 967, 971, 983, 991, 997, 1013, 1039, 1051, 1069, 1087, 1103, 1117, 1129, 1151, 1163, 1181, 1193, 1213, 1231, 1237, 1259, 1279, 1291, 1307, 1327, 1373, 1381, 1399, 1423, 1439, 1453, 1471, 1487, 1499, 1511, 1531, 1549, 1567, 1583, 1597, 1613, 1627, 1637, 1663, 1669, 1693, 1709, 1723, 1741, 1759, 1789, 1801, 1823, 1831, 1847, 1871, 1879, 1901, 1913, 1933, 1951, 1979, 1999, 2011, 2029, 2069, 2111, 2143, 2161, 2207, 2239, 2269, 2297, 2333, 2357, 2399, 2423, 2459, 2477, 2521, 2557, 2591, 2621, 2647, 2687, 2719, 2749, 2777, 2803, 2843, 2879, 2909, 2939, 2971, 3001, 3037, 3067, 3089, 3121, 3167, 3191, 3229, 3259, 3271, 3323, 3359, 3391, 3413, 3449, 3469, 3517, 3547, 3583, 3613, 3643, 3677, 3709, 3739, 3769, 3803, 3833, 3863, 3889, 3931, 3967, 3989, 4027, 4057, 4159, 4219, 4283, 4349, 4409, 4463, 4523, 4603, 4663, 4733, 4799, 4861, 4919, 4987, 5051, 5119, 5179, 5237, 5309, 5351, 5437, 5503, 5563, 5623, 5693, 5749, 5821, 5881, 5939, 6011, 6079, 6143, 6203, 6271, 6329, 6397, 6451, 6521, 6581,
		6653, 6719, 6781, 6841, 6911, 6971, 7039, 7103, 7159, 7229, 7283, 7351, 7417, 7487, 7549, 7607, 7673, 7741, 7793, 7867, 7933, 7993, 8059, 8123, 8317, 8447, 8573, 8699, 8831, 8951, 9067, 9209, 9343, 9467, 9587, 9721, 9851, 9973, 10111, 10223, 10357, 10487, 10613, 10739, 10867, 11003, 11131, 11261, 11383, 11519, 11633, 11743, 11903, 12011, 12157, 12281, 12413, 12541, 12671, 12799, 12923, 13049, 13183, 13309, 13421, 13567, 13693, 13807, 13933, 14071, 14207, 14327, 14461, 14591, 14717, 14843, 14969, 15101, 15227, 15359, 15473, 15607, 15739, 15859, 15991, 16127, 16253, 16633, 16889, 17137, 17401, 17659, 17911, 18169, 18427, 18679, 18919, 19183, 19447, 19709, 19963, 20219, 20479, 20731, 20983, 21247, 21503, 21757, 22013, 22271, 22511, 22783, 23039, 23293, 23549, 23801, 24061, 24317, 24571, 24821, 25087, 25343, 25589, 25849, 26111, 26357, 26597, 26879, 27127, 27367, 27647, 27901, 28151, 28411, 28669, 28927, 29179, 29437, 29683, 29947, 30203, 30449, 30713, 30971, 31231, 31481, 31741, 31991, 32251, 32507, 33247, 33791, 34303, 34807, 35327, 35839, 36343, 36857, 37369, 37879, 38393, 38903, 39419, 39929, 40433, 40949, 41467, 41983, 42491, 43003, 43517, 44029, 44543, 45053, 45557, 46073, 46591, 47093, 47609, 48121, 48623, 49139, 49663, 50159, 50683, 51199, 51691, 52223, 52733, 53239, 53759, 54269, 54779, 55291, 55807, 56311, 56827, 57331, 57853, 58367, 58831, 59387, 59887, 60413, 60923, 61417, 61949, 62459, 62971, 63487, 63997, 64499, 65011, 66553, 67579, 68597, 69623, 70639, 71671, 72701, 73727, 74747, 75773, 76781, 77813, 78839, 79867, 80863, 81919, 82939, 83939, 84991, 86011, 87037, 88037, 89087, 90107, 91129,
		92153, 93179, 94207, 95231, 96233, 97259, 98299, 99317, 100343, 101363, 102397, 103423, 104417, 105467, 106487, 107509, 108541, 109567, 110587, 111611, 112621, 113657, 114679, 115693, 116731, 117757, 118757, 119797, 120829, 121853, 122869, 123887, 124919, 125941, 126967, 127997, 129023, 130043, 133117, 135151, 137209, 139241, 141311, 143357, 145399, 147451, 149503, 151549, 153589, 155627, 157679, 159739, 161783, 163819, 165887, 167917, 169957, 172031, 174079, 176123, 178169, 180221, 182261, 184309, 186343, 188407, 190409, 192499, 194543, 196597, 198647, 200699, 202751, 204797, 206827, 208891, 210943, 212987, 214993, 217081, 219133, 221173, 223229, 225263, 227303, 229373, 231419, 233437, 235519, 237563, 239611, 241663, 243709, 245759, 247799, 249853, 251903, 253951, 255989, 258031, 260089, 266239, 270329, 274423, 278503, 282617, 286711, 290803, 294911, 298999, 303097, 307189, 311293, 315389, 319483, 323581, 327673, 331769, 335857, 339959, 344053, 348149, 352249, 356351, 360439, 364543, 368633, 372733, 376823, 380917, 385013, 389117, 393209, 397303, 401407, 405499, 409597, 413689, 417773, 421847, 425977, 430061, 434167, 438271, 442367, 446461, 450557, 454637, 458747, 462841, 466919, 471007, 475109, 479231, 483323, 487423, 491503, 495613, 499711, 503803, 507901, 511997, 516091, 520151, 532453, 540629, 548861, 557041, 565247, 573437, 581617, 589811, 598007, 606181, 614387, 622577, 630737, 638971, 647161, 655357, 663547,
		671743, 679933, 688111, 696317, 704507, 712697, 720887, 729073, 737279, 745471, 753659, 761833, 770047, 778237, 786431, 794593, 802811, 810989, 819187, 827389, 835559, 843763, 851957, 860143, 868349, 876529, 884717, 892919, 901111, 909301, 917503, 925679, 933883, 942079, 950269, 958459, 966653, 974837, 982981, 991229, 999389, 1007609, 1015769, 1023991, 1032191, 1040381, 1064957, 1081337, 1097717, 1114111, 1130471, 1146877, 1163263, 1179641, 1196029, 1212401, 1228789, 1245169, 1261567, 1277911, 1294309, 1310719, 1327099, 1343479, 1359871, 1376237, 1392631, 1409017, 1425371, 1441771, 1458169, 1474559, 1490941, 1507321, 1523707, 1540087, 1556473, 1572853, 1589239, 1605631, 1622009, 1638353, 1654739, 1671161, 1687549, 1703903, 1720307, 1736701, 1753069, 1769441, 1785853, 1802239, 1818617, 1835003, 1851391, 1867771, 1884133, 1900543, 1916921, 1933301, 1949657, 1966079, 1982447, 1998839, 2015213, 2031611, 2047993, 2064379, 2080763, 2129903, 2162681, 2195443, 2228221, 2260967, 2293757, 2326517, 2359267, 2392057, 2424827, 2457569, 2490337, 2523133, 2555897, 2588671, 2621431, 2654161, 2686973, 2719741, 2752499, 2785273, 2818043, 2850811, 2883577, 2916343, 2949119, 2981887, 3014653, 3047423, 3080167, 3112943, 3145721, 3178489, 3211213, 3244013, 3276799, 3309563, 3342331, 3375083, 3407857, 3440627, 3473399, 3506171, 3538933, 3571699, 3604451, 3637223, 3670013, 3702757, 3735547, 3768311, 3801073, 3833833, 3866623, 3899383, 3932153, 3964913, 3997673, 4030463, 4063217, 4095991, 4128767, 4161527, 4259837, 4325359, 4390909,
		4456433, 4521977, 4587503, 4653041, 4718579, 4784107, 4849651, 4915171, 4980727, 5046259, 5111791, 5177339, 5242877, 5308379, 5373931, 5439479, 5505023, 5570533, 5636077, 5701627, 5767129, 5832679, 5898209, 5963773, 6029299, 6094807, 6160381, 6225917, 6291449, 6356983, 6422519, 6488023, 6553577, 6619111, 6684659, 6750203, 6815741, 6881269, 6946813, 7012337, 7077883, 7143421, 7208951, 7274489, 7340009, 7405547, 7471099, 7536637, 7602151, 7667711, 7733233, 7798783, 7864301, 7929833, 7995391, 8060891, 8126453, 8191991, 8257531, 8323069, 8519647, 8650727, 8781797, 8912887, 9043967, 9175037, 9306097, 9437179, 9568219, 9699323, 9830393, 9961463, 10092539, 10223593, 10354667, 10485751, 10616831, 10747903, 10878961, 11010037, 11141113, 11272181, 11403247, 11534329, 11665403, 11796469, 11927551, 12058621, 12189677, 12320753, 12451807, 12582893, 12713959, 12845033, 12976121, 13107197, 13238263, 13369333, 13500373, 13631477, 13762549, 13893613, 14024671, 14155763, 14286809, 14417881, 14548979, 14680063, 14811133, 14942197, 15073277, 15204349, 15335407, 15466463, 15597559, 15728611, 15859687, 15990781, 16121849, 16252919, 16383977, 16515067, 16646099, 17039339, 17301463, 17563633, 17825791, 18087899, 18350063, 18612211, 18874367, 19136503, 19398647, 19660799, 19922923, 20185051, 20447191, 20709347, 20971507, 21233651, 21495797, 21757951, 22020091, 22282199, 22544351, 22806521, 23068667, 23330773, 23592937, 23855101, 24117217, 24379391, 24641479, 24903667, 25165813, 25427957, 25690097, 25952243, 26214379, 26476543, 26738687,
		27000817, 27262931, 27525109, 27787213, 28049407, 28311541, 28573673, 28835819, 29097977, 29360087, 29622269, 29884411, 30146531, 30408701, 30670847, 30932987, 31195117, 31457269, 31719409, 31981567, 32243707, 32505829, 32767997, 33030121, 33292283, 34078699, 34602991, 35127263, 35651579, 36175871, 36700159, 37224437, 37748717, 38273023, 38797303, 39321599, 39845887, 40370173, 40894457, 41418739, 41943023, 42467317, 42991609, 43515881, 44040187, 44564461, 45088739, 45613039, 46137319, 46661627, 47185907, 47710207, 48234451, 48758783, 49283063, 49807327, 50331599, 50855899, 51380179, 51904511, 52428767, 52953077, 53477357, 54001663, 54525917, 55050217, 55574507, 56098813, 56623093, 57147379, 57671671, 58195939, 58720253, 59244539, 59768831, 60293119, 60817397, 61341659, 61865971, 62390261, 62914549, 63438839, 63963131, 64487417, 65011703, 65535989, 66060277, 66584561, 68157433, 69205987, 70254563, 71303153, 72351733, 73400311, 74448877, 75497467, 76546039, 77594599, 78643199, 79691761, 80740339, 81788923, 82837501, 83886053, 84934621, 85983217, 87031759, 88080359, 89128939, 90177533, 91226101, 92274671, 93323249, 94371833, 95420401, 96468979, 97517543, 98566121, 99614689, 100663291, 101711839, 102760387, 103809011, 104857589, 105906167, 106954747, 108003323, 109051903, 110100409, 111148963, 112197629, 113246183, 114294721, 115343341, 116391917, 117440509, 118489081, 119537653, 120586231, 121634801, 122683391,
		123731963, 124780531, 125829103, 126877693, 127926263, 128974841, 130023407, 131071987, 132120557, 133169137, 136314869, 138412031, 140509183, 142606333, 144703477, 146800637, 148897789, 150994939, 153092087, 155189239, 157286383, 159383467, 161480689, 163577833, 165674981, 167772107, 169869311, 171966439, 174063611, 176160739, 178257917, 180355069, 182452199, 184549373, 186646517, 188743679, 190840817, 192937933, 195035129, 197132267, 199229419, 201326557, 203423729, 205520881, 207618031, 209715199, 211812331, 213909503, 216006599, 218103799, 220200947, 222298093, 224395253, 226492393, 228589561, 230686697, 232783871, 234881011, 236978171, 239075327, 241172479, 243269627, 245366777, 247463933, 249561083, 251658227, 253755391, 255852511, 257949691, 260046847, 262143977, 264241147, 266338297, 272629759, 276824033, 281018263, 285212659, 289406951, 293601263, 297795541, 301989881, 306184189, 310378493, 314572799, 318767093, 322961399, 327155693, 331349989, 335544301, 339738617, 343932923, 348127231, 352321531, 356515813, 360710137, 364904447, 369098707, 373293049, 377487343, 381681661, 385875929, 390070267, 394264567, 398458859, 402653171, 406847479, 411041767, 415236083, 419430383, 423624673, 427819001, 432013291, 436207613, 440401889, 444596209, 448790519, 452984827, 457179131, 461373431, 465567743, 469762043, 473956339, 478150643, 482344957, 486539257, 490733567, 494927857, 499122163, 503316469, 507510719, 511705069, 515899379,
		520093667, 524287997, 528482263, 532676593, 545259493, 553648103, 562036693, 570425299, 578813951, 587202547, 595591153, 603979769, 612368377, 620756953, 629145593, 637534199, 645922787, 654311423, 662700019, 671088637, 679477243, 687865847, 696254441, 704643067, 713031659, 721420261, 729808889, 738197503, 746586109, 754974691, 763363327, 771751927, 780140521, 788529121, 796917757, 805306357, 813694951, 822083551, 830472127, 838860791, 847249399, 855637957, 864026567, 872415211, 880803809, 889192441, 897581029, 905969653, 914358253, 922746863, 931135487, 939524087, 947912701, 956301169, 964689917, 973078511, 981467119, 989855707, 998244341, 1006632947, 1015021549, 1023410159, 1031798783, 1040187377, 1048575959, 1056964501, 1065353209, 1090519013, 1107296251, 1124073463, 1140850681, 1157627879, 1174405103, 1191182309, 1207959503, 1224736717, 1241513981, 1258291187, 1275068407, 1291845593, 1308622837, 1325400059, 1342177237, 1358954453, 1375731701, 1392508927, 1409286139, 1426063351, 1442840569, 1459617779, 1476394991, 1493172223, 1509949421, 1526726647, 1543503851, 1560281087, 1577058271, 1593835489, 1610612711, 1627389901, 1644167159, 1660944367, 1677721597, 1694498801, 1711276019, 1728053237, 1744830457, 1761607657, 1778384887, 1795162111, 1811939317, 1828716523, 1845493753, 1862270957, 1879048183, 1895825387, 1912602623, 1929379813, 1946157053, 1962934271, 1979711483, 1996488677, 2013265907, 2030043131, 2046820351, 2063597567, 2080374781, 2097151967, 2113929203, 2130706381 
	};

	//  2G 内贴 2^N 的质数
	static const size_t primes2n[] =
	{
		1, 2, 3, 7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521, 131071, 262139, 524287,
		1048573, 2097143, 4194301, 8388593, 16777213, 33554393, 67108859, 134217689, 268435399, 536870909, 1073741789
	};

	bool IsPrime(size_t candidate)
	{
		if ((candidate & 1) != 0)
		{
			size_t limit = size_t(std::sqrt(candidate));
			for (size_t divisor = 3; divisor <= limit; divisor += 2)
			{
				if ((candidate % divisor) == 0)
				{
					return false;
				}
			}
			return true;
		}
		return (candidate == 2);
	}

	//int32 GetPrime(size_t capacity, size_t dataSize)
	//{
	//	size_t memUsage = Round2n(capacity * dataSize);        // 算内存实际用量
	//	size_t maxCapacity = memUsage / dataSize;                     // 内存实际用量下 能放多少个 data

	//	if (maxCapacity == capacity)                                // if maxCapacity 为 2^N, 查表
	//	{
	//		return primes2n[Calc2n(capacity)];
	//	}
	//	if (dataSize >= 8 && dataSize <= 512)                       // 数据长在 查表 范围内的
	//	{
	//		return *std::upper_bound(std::begin(primes), std::end(primes), maxCapacity);
	//	}
	//	for (size_t i = maxCapacity + 1; i <= 0x7fffffff; i += 2)   // maxCapacity 是双数. +1 即为单数
	//	{
	//		if (IsPrime(i))
	//		{
	//			return (int)i;
	//		}
	//	}
	//	assert(false);
	//	return -1;
	//}

	size_t GetPrime(size_t n, bool isLess/* = true*/)
	{
		if (isLess)
		{
			return primes2n[Calc2n(n)];
		}
		else
		{
			for (size_t prime : primes2n)
			{
				if (prime >= n)
				{
					return prime;
				}
			}
			for (std::size_t prime = (n | 1); prime < std::numeric_limits<uint32_t>::max(); prime += 2)
			{
				if (IsPrime(prime) && ((prime - 1) % 101 != 0))
				{
					return prime;
				}
			}
			return n;
		}
	}


	size_t GetToStringMaxLength(uint8   v)
	{
		return 3;
	}
	size_t GetToStringMaxLength(uint16  v)
	{
		return 5;
	}
	size_t GetToStringMaxLength(uint32  v)
	{
		return 10;
	}
	size_t GetToStringMaxLength(uint64  v)
	{
		return 20;
	}
	size_t GetToStringMaxLength(int8    v)
	{
		return 4;
	}
	size_t GetToStringMaxLength(int16   v)
	{
		return 6;
	}
	size_t GetToStringMaxLength(int32   v)
	{
		return 11;
	}
	size_t GetToStringMaxLength(int64   v)
	{
		return 21;
	}
	size_t GetToStringMaxLength(double  v)
	{
		return 19;
	}
	size_t GetToStringMaxLength(float   v)
	{
		return 10;
	}
	size_t GetToStringMaxLength(bool    v)
	{
		return 5;
	}
	size_t GetToStringMaxLength(char    v)
	{
		return 1;
	}
	size_t GetToStringMaxLength(long    v)
	{
#ifdef Ajy_MSVC
		return GetToStringMaxLength((int32)v);
#else
		return GetToStringMaxLength((int3264)v);
#endif
	}
	size_t GetToStringMaxLength(ulong   v)
	{
#ifdef Ajy_MSVC
		return GetToStringMaxLength((uint32)v);
#else
		return GetToStringMaxLength((uint3264)v);
#endif
	}
	size_t GetToStringMaxLength(char const* v)
	{
		return strlen(v);
	}
	size_t GetToStringMaxLength(std::string const& v)
	{
		return v.size();
	}
	size_t GetToStringMaxLength(String const& v)
	{
		return v.Size();
	}


	// the code is copy from following URL:
	// https://github.com/miloyip/itoa-benchmark/tree/master/src

	char const gDigitsLut[200] = {
		'0', '0', '0', '1', '0', '2', '0', '3', '0', '4', '0', '5', '0', '6', '0', '7', '0', '8', '0', '9',
		'1', '0', '1', '1', '1', '2', '1', '3', '1', '4', '1', '5', '1', '6', '1', '7', '1', '8', '1', '9',
		'2', '0', '2', '1', '2', '2', '2', '3', '2', '4', '2', '5', '2', '6', '2', '7', '2', '8', '2', '9',
		'3', '0', '3', '1', '3', '2', '3', '3', '3', '4', '3', '5', '3', '6', '3', '7', '3', '8', '3', '9',
		'4', '0', '4', '1', '4', '2', '4', '3', '4', '4', '4', '5', '4', '6', '4', '7', '4', '8', '4', '9',
		'5', '0', '5', '1', '5', '2', '5', '3', '5', '4', '5', '5', '5', '6', '5', '7', '5', '8', '5', '9',
		'6', '0', '6', '1', '6', '2', '6', '3', '6', '4', '6', '5', '6', '6', '6', '7', '6', '8', '6', '9',
		'7', '0', '7', '1', '7', '2', '7', '3', '7', '4', '7', '5', '7', '6', '7', '7', '7', '8', '7', '9',
		'8', '0', '8', '1', '8', '2', '8', '3', '8', '4', '8', '5', '8', '6', '8', '7', '8', '8', '8', '9',
		'9', '0', '9', '1', '9', '2', '9', '3', '9', '4', '9', '5', '9', '6', '9', '7', '9', '8', '9', '9'
	};

	// Branching for different cases (forward)
	// Use lookup table of two digits

	size_t u32toa_branchlut(uint32 value, char* buffer)
	{
		char* bak = buffer;
		if (value < 10000)
		{
			uint32 const d1 = (value / 100) << 1;
			uint32 const d2 = (value % 100) << 1;

			if (value >= 1000)
				*buffer++ = gDigitsLut[d1];
			if (value >= 100)
				*buffer++ = gDigitsLut[d1 + 1];
			if (value >= 10)
				*buffer++ = gDigitsLut[d2];
			*buffer++ = gDigitsLut[d2 + 1];
		}
		else if (value < 100000000)
		{
			// value = bbbbcccc
			uint32 const b = value / 10000;
			uint32 const c = value % 10000;

			uint32 const d1 = (b / 100) << 1;
			uint32 const d2 = (b % 100) << 1;

			uint32 const d3 = (c / 100) << 1;
			uint32 const d4 = (c % 100) << 1;

			if (value >= 10000000)
				*buffer++ = gDigitsLut[d1];
			if (value >= 1000000)
				*buffer++ = gDigitsLut[d1 + 1];
			if (value >= 100000)
				*buffer++ = gDigitsLut[d2];
			*buffer++ = gDigitsLut[d2 + 1];

			*buffer++ = gDigitsLut[d3];
			*buffer++ = gDigitsLut[d3 + 1];
			*buffer++ = gDigitsLut[d4];
			*buffer++ = gDigitsLut[d4 + 1];
		}
		else
		{
			// value = aabbbbcccc in decimal

			uint32 const a = value / 100000000; // 1 to 42
			value %= 100000000;

			if (a >= 10)
			{
				const unsigned i = a << 1;
				*buffer++ = gDigitsLut[i];
				*buffer++ = gDigitsLut[i + 1];
			}
			else
				*buffer++ = '0' + static_cast<char>(a);

			uint32 const b = value / 10000; // 0 to 9999
			uint32 const c = value % 10000; // 0 to 9999

			uint32 const d1 = (b / 100) << 1;
			uint32 const d2 = (b % 100) << 1;

			uint32 const d3 = (c / 100) << 1;
			uint32 const d4 = (c % 100) << 1;

			*buffer++ = gDigitsLut[d1];
			*buffer++ = gDigitsLut[d1 + 1];
			*buffer++ = gDigitsLut[d2];
			*buffer++ = gDigitsLut[d2 + 1];
			*buffer++ = gDigitsLut[d3];
			*buffer++ = gDigitsLut[d3 + 1];
			*buffer++ = gDigitsLut[d4];
			*buffer++ = gDigitsLut[d4 + 1];
		}
		//*buffer++ = '\0';
		return size_t(buffer - bak);
	}

	size_t i32toa_branchlut(int32 value, char* buffer)
	{
		uint32 u = static_cast<uint32>(value);
		if (value < 0)
		{
			*buffer++ = '-';
			u = ~u + 1;
			return u32toa_branchlut(u, buffer) + 1;
		}
		return u32toa_branchlut(u, buffer);
	}

	size_t u64toa_branchlut(uint64 value, char* buffer)
	{
		char* bak = buffer;
		if (value < 100000000)
		{
			uint32 v = static_cast<uint32>(value);
			if (v < 10000)
			{
				uint32 const d1 = (v / 100) << 1;
				uint32 const d2 = (v % 100) << 1;

				if (v >= 1000)
					*buffer++ = gDigitsLut[d1];
				if (v >= 100)
					*buffer++ = gDigitsLut[d1 + 1];
				if (v >= 10)
					*buffer++ = gDigitsLut[d2];
				*buffer++ = gDigitsLut[d2 + 1];
			}
			else
			{
				// value = bbbbcccc
				uint32 const b = v / 10000;
				uint32 const c = v % 10000;

				uint32 const d1 = (b / 100) << 1;
				uint32 const d2 = (b % 100) << 1;

				uint32 const d3 = (c / 100) << 1;
				uint32 const d4 = (c % 100) << 1;

				if (value >= 10000000)
					*buffer++ = gDigitsLut[d1];
				if (value >= 1000000)
					*buffer++ = gDigitsLut[d1 + 1];
				if (value >= 100000)
					*buffer++ = gDigitsLut[d2];
				*buffer++ = gDigitsLut[d2 + 1];

				*buffer++ = gDigitsLut[d3];
				*buffer++ = gDigitsLut[d3 + 1];
				*buffer++ = gDigitsLut[d4];
				*buffer++ = gDigitsLut[d4 + 1];
			}
		}
		else if (value < 10000000000000000)
		{
			uint32 const v0 = static_cast<uint32>(value / 100000000);
			uint32 const v1 = static_cast<uint32>(value % 100000000);

			uint32 const b0 = v0 / 10000;
			uint32 const c0 = v0 % 10000;

			uint32 const d1 = (b0 / 100) << 1;
			uint32 const d2 = (b0 % 100) << 1;

			uint32 const d3 = (c0 / 100) << 1;
			uint32 const d4 = (c0 % 100) << 1;

			uint32 const b1 = v1 / 10000;
			uint32 const c1 = v1 % 10000;

			uint32 const d5 = (b1 / 100) << 1;
			uint32 const d6 = (b1 % 100) << 1;

			uint32 const d7 = (c1 / 100) << 1;
			uint32 const d8 = (c1 % 100) << 1;

			if (value >= 1000000000000000)
				*buffer++ = gDigitsLut[d1];
			if (value >= 100000000000000)
				*buffer++ = gDigitsLut[d1 + 1];
			if (value >= 10000000000000)
				*buffer++ = gDigitsLut[d2];
			if (value >= 1000000000000)
				*buffer++ = gDigitsLut[d2 + 1];
			if (value >= 100000000000)
				*buffer++ = gDigitsLut[d3];
			if (value >= 10000000000)
				*buffer++ = gDigitsLut[d3 + 1];
			if (value >= 1000000000)
				*buffer++ = gDigitsLut[d4];
			if (value >= 100000000)
				*buffer++ = gDigitsLut[d4 + 1];

			*buffer++ = gDigitsLut[d5];
			*buffer++ = gDigitsLut[d5 + 1];
			*buffer++ = gDigitsLut[d6];
			*buffer++ = gDigitsLut[d6 + 1];
			*buffer++ = gDigitsLut[d7];
			*buffer++ = gDigitsLut[d7 + 1];
			*buffer++ = gDigitsLut[d8];
			*buffer++ = gDigitsLut[d8 + 1];
		}
		else
		{
			uint32 const a = static_cast<uint32>(value / 10000000000000000); // 1 to 1844
			value %= 10000000000000000;

			if (a < 10)
				*buffer++ = '0' + static_cast<char>(a);
			else if (a < 100)
			{
				uint32 const i = a << 1;
				*buffer++ = gDigitsLut[i];
				*buffer++ = gDigitsLut[i + 1];
			}
			else if (a < 1000)
			{
				*buffer++ = '0' + static_cast<char>(a / 100);

				uint32 const i = (a % 100) << 1;
				*buffer++ = gDigitsLut[i];
				*buffer++ = gDigitsLut[i + 1];
			}
			else
			{
				uint32 const i = (a / 100) << 1;
				uint32 const j = (a % 100) << 1;
				*buffer++ = gDigitsLut[i];
				*buffer++ = gDigitsLut[i + 1];
				*buffer++ = gDigitsLut[j];
				*buffer++ = gDigitsLut[j + 1];
			}

			uint32 const v0 = static_cast<uint32>(value / 100000000);
			uint32 const v1 = static_cast<uint32>(value % 100000000);

			uint32 const b0 = v0 / 10000;
			uint32 const c0 = v0 % 10000;

			uint32 const d1 = (b0 / 100) << 1;
			uint32 const d2 = (b0 % 100) << 1;

			uint32 const d3 = (c0 / 100) << 1;
			uint32 const d4 = (c0 % 100) << 1;

			uint32 const b1 = v1 / 10000;
			uint32 const c1 = v1 % 10000;

			uint32 const d5 = (b1 / 100) << 1;
			uint32 const d6 = (b1 % 100) << 1;

			uint32 const d7 = (c1 / 100) << 1;
			uint32 const d8 = (c1 % 100) << 1;

			*buffer++ = gDigitsLut[d1];
			*buffer++ = gDigitsLut[d1 + 1];
			*buffer++ = gDigitsLut[d2];
			*buffer++ = gDigitsLut[d2 + 1];
			*buffer++ = gDigitsLut[d3];
			*buffer++ = gDigitsLut[d3 + 1];
			*buffer++ = gDigitsLut[d4];
			*buffer++ = gDigitsLut[d4 + 1];
			*buffer++ = gDigitsLut[d5];
			*buffer++ = gDigitsLut[d5 + 1];
			*buffer++ = gDigitsLut[d6];
			*buffer++ = gDigitsLut[d6 + 1];
			*buffer++ = gDigitsLut[d7];
			*buffer++ = gDigitsLut[d7 + 1];
			*buffer++ = gDigitsLut[d8];
			*buffer++ = gDigitsLut[d8 + 1];
		}

		//*buffer = '\0';
		return size_t(buffer - bak);
	}

	size_t i64toa_branchlut(int64 value, char* buffer)
	{
		uint64 u = static_cast<uint64>(value);
		if (value < 0)
		{
			*buffer++ = '-';
			u = ~u + 1;
			return u64toa_branchlut(u, buffer) + 1;
		}
		return u64toa_branchlut(u, buffer);
	}

	size_t ToStringCore(bool in, char* out)
	{
		if (in)
		{
			out[0] = 't';
			out[1] = 'r';
			out[2] = 'u';
			out[3] = 'e';
			return 4;
		}
		else
		{
			out[0] = 'f';
			out[1] = 'a';
			out[2] = 'l';
			out[3] = 's';
			out[4] = 'e';
			return 5;
		}
	}

	size_t ToStringCore(double in, char* out)
	{
		return DiyFp::dtoa_milo(in, out);
	}

	size_t ToStringCore(float in, char* out)
	{
		return DiyFp::dtoa_milo(in, out);
	}

	size_t ToString(char* dstBuf, uint8   v)
	{
		return u32toa_branchlut(v, dstBuf);
	}
	size_t ToString(char* dstBuf, uint16  v)
	{
		return u32toa_branchlut(v, dstBuf);
	}
	size_t ToString(char* dstBuf, uint32  v)
	{
		return u32toa_branchlut(v, dstBuf);
	}
	size_t ToString(char* dstBuf, uint64  v)
	{
		return u64toa_branchlut(v, dstBuf);
	}
	size_t ToString(char* dstBuf, int8    v)
	{
		return i32toa_branchlut(v, dstBuf);
	}
	size_t ToString(char* dstBuf, int16   v)
	{
		return i32toa_branchlut(v, dstBuf);
	}
	size_t ToString(char* dstBuf, int32   v)
	{
		return i32toa_branchlut(v, dstBuf);
	}
	size_t ToString(char* dstBuf, int64   v)
	{
		return i64toa_branchlut(v, dstBuf);
	}
	size_t ToString(char* dstBuf, double  v)
	{
		return ToStringCore(v, dstBuf);
	}
	size_t ToString(char* dstBuf, float   v)
	{
		return ToStringCore(v, dstBuf);
	}
	size_t ToString(char* dstBuf, bool    v)
	{
		return ToStringCore(v, dstBuf);
	}
	size_t ToString(char* dstBuf, char v)
	{
		*dstBuf = v;
		return 1;
	}
	size_t ToString(char* dstBuf, long v)
	{
#ifdef Ajy_MSVC
		return ToString(dstBuf, (int32)v);
#else
		return ToString(dstBuf, (int3264)v);
#endif
	}
	size_t ToString(char* dstBuf, ulong v)
	{
#ifdef Ajy_MSVC
		return ToString(dstBuf, (uint32)v);
#else
		return ToString(dstBuf, (uint3264)v);
#endif
	}
	size_t ToString(char* dstBuf, char const* v)
	{
		size_t len = strlen(v);
		memcpy(dstBuf, v, len);
		return len;
	}
	size_t ToString(char * dstBuf, std::string const& v)
	{
		memcpy(dstBuf, v.c_str(), v.size());
		return v.size();
	}
	size_t ToString(char * dstBuf, String const& v)
	{
		memcpy(dstBuf, v.C_str(), v.Size());
		return v.Size();
	}


	static char const Int2HexTable[] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E', 'F'
	};

	size_t ToHexString(char* dstBuf, uint8 v)
	{
		dstBuf[0] = Int2HexTable[v >> 4];
		dstBuf[1] = Int2HexTable[v & 15];
		return 2;
	}

	size_t ToHexString(char* dstBuf, uint16 v)
	{
		ToHexString(dstBuf, uint8(v >> 8));
		ToHexString(dstBuf + 2, (uint8)v);
		return 4;
	}

	size_t ToHexString(char* dstBuf, uint32 v)
	{
		ToHexString(dstBuf, uint16(v >> 16));
		ToHexString(dstBuf + 4, (uint16)v);
		return 8;
	}

	size_t ToHexString(char* dstBuf, uint64 v)
	{
		ToHexString(dstBuf, uint32(v >> 32));
		ToHexString(dstBuf + 8, (uint32)v);
		return 16;
	}

	size_t ToHexString(char* dstBuf, int8 v)
	{
		return ToHexString(dstBuf, (uint8)v);
	}

	size_t ToHexString(char* dstBuf, int16 v)
	{
		return ToHexString(dstBuf, (uint16)v);
	}

	size_t ToHexString(char* dstBuf, int32 v)
	{
		return ToHexString(dstBuf, (uint32)v);
	}

	size_t ToHexString(char* dstBuf, int64 v)
	{
		return ToHexString(dstBuf, (uint64)v);
	}

	size_t ToHexString(char* dstBuf, char v)
	{
		return ToHexString(dstBuf, (uint8)v);
	}

	size_t ToHexString(char* dstBuf, long v)
	{
#ifdef Ajy_MSVC
		return ToString(dstBuf, (int32)v);
#else
		return ToString(dstBuf, (int3264)v);
#endif
	}

	size_t ToHexString(char* dstBuf, ulong v)
	{
#ifdef Ajy_MSVC
		return ToString(dstBuf, (uint32)v);
#else
		return ToString(dstBuf, (uint3264)v);
#endif
	}



	template <class OutType>
	void toInt(char const* in, OutType& out)
	{
		char const* in_ = in;
		if (*in_ == '0')
		{
			out = 0;
			return;
		}
		bool b;
		if (*in_ == '-')
		{
			b = true;
			++in_;
		}
		else b = false;
		OutType r = *in_ - '0';
		char c;
		while ((c = *(++in_))) 
			r = r * 10 + (c - '0');
		out = b ? -r : r;
	}
	template <class OutType>
	void toUInt(char const* in, OutType& out)
	{
		assert(in);
		char const* in_ = in;
		if (*in_ == '0')
		{
			out = 0;
			return;
		}
		OutType r = *(in_)-'0';
		char c;
		while ((c = *(++in_))) 
			r = r * 10 + (c - '0');
		out = r;
	}
	void FromString(uint8& out, char const* in)
	{
		toUInt(in, out);
	}
	void FromString(uint16& out, char const* in)
	{
		toUInt(in, out);
	}
	void FromString(uint32& out, char const* in)
	{
		toUInt(in, out);
	}
	void FromString(uint64& out, char const* in)
	{
		toUInt(in, out);
	}
	void FromString(int8& out, char const* in)
	{
		toInt(in, out);
	}
	void FromString(int16& out, char const* in)
	{
		toInt(in, out);
	}
	void FromString(int32& out, char const* in)
	{
		toInt(in, out);
	}
	void FromString(int64& out, char const* in)
	{
		toInt(in, out);
	}
	void FromString(double& out, char const* in)
	{
		out = strtod(in, nullptr);
	}
	void FromString(float& out, char const* in)
	{
		out = (float)strtod(in, nullptr);
	}
	void FromString(bool& out, char const* in)
	{
		out = (in[0] == '1' || in[0] == 'T' || in[0] == 't');
	}
	void FromString(long& out, char const* in)
	{
#ifdef Ajy_MSVC
		FromString((int32&)out, in);
#else
		FromString((int3264&)out, in);
#endif

	}
	void FromString(ulong& out, char const* in)
	{
#ifdef Ajy_MSVC
		FromString((uint32&)out, in);
#else
		FromString((int3264&)out, in);
#endif
	}


}
