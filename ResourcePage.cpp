// ResourcePage.cpp : implementation file
//

#include "stdafx.h"
#include "PalCheator.h"
#include "ResourcePage.h"

#include "GridCellCheck.h"
#include "GridCellNumeric.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern GameSaveInfo g_SaveInfo;

/////////////////////////////////////////////////////////////////////////////
// ResourcePage dialog


ResourcePage::ResourcePage(CWnd* pParent /*=NULL*/)
	: CDialog(ResourcePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(ResourcePage)
	m_TotalResource = 0;
	//}}AFX_DATA_INIT
}


void ResourcePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ResourcePage)
		DDX_GridControl(pDX, IDC_RESOURCE_GRID, m_Grid);
	DDX_Text(pDX, IDC_EDIT_TOTAL_RESOURCE, m_TotalResource);
	//}}AFX_DATA_MAP
}

void ResourcePage::OnOK( )
{
}

void ResourcePage::OnCancel( )
{
}

BEGIN_MESSAGE_MAP(ResourcePage, CDialog)
	//{{AFX_MSG_MAP(ResourcePage)
	ON_BN_CLICKED(IDC_BUTTON_ELITE_RESOURCE, OnButtonEliteResource)
	ON_BN_CLICKED(IDC_BUTTON_FULL_RESOURCE, OnButtonFullResource)
	ON_BN_CLICKED(IDC_BUTTON_RESET_RESOURCE, OnButtonResetResource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ResourcePage message handlers

static const char *ColName[] =
{
	"名称", "有否", "数量", "编号", "剧情相关", "物品描述"
};

enum RESOURCE_TYPE
{
	CRITICAL_RESOURE,           //剧情发展所需要的道具
	ELITE___RESOURCE   ,        //极品装备
	TRIVIAL_RESOURCE            //非极品装备
};

struct ResourceInfo
{
	char          *m_ResName;             //物品名称
	short          m_ResID;               //物品编号
	RESOURCE_TYPE  m_ResType;             //剧情需要的物品
	char           *m_ResDesc;            //物品描述信息
};

static const short ResStartID = 0x003D;

static const struct ResourceInfo AllResource[] =
{
	{ "观音符",     0x003D, TRIVIAL_RESOURCE, "以观音圣水书写的灵符。体力+150" },
	{ "圣灵符",     0x003E, TRIVIAL_RESOURCE, "具有女娲神强大灵力的符咒。全体体力+300" },
	{ "金刚符",     0x003F, TRIVIAL_RESOURCE, "使用后如有金钟铁罩护身。增加防御七回合" },
	{ "净衣符",     0x0040, TRIVIAL_RESOURCE, "具有袪病、御邪的法力，可解赤毒、尸毒、瘴毒。" },
	{ "灵心符",     0x0041, TRIVIAL_RESOURCE, "具有宁神、驱邪的灵效，可解疯魔、定身、昏睡、咒封。" },
	{ "天师符",     0x0042, TRIVIAL_RESOURCE, "茅山道士用来对付妖怪的符咒。" },
	{ "风灵符",     0x0043, TRIVIAL_RESOURCE, "产生风系法术的符咒。" },
	{ "雷灵符",     0x0044, TRIVIAL_RESOURCE, "产生雷系法术的符咒。" },
	{ "水灵符",     0x0045, TRIVIAL_RESOURCE, "产生冰系法术的符咒。" },
	{ "火灵符",     0x0046, TRIVIAL_RESOURCE, "产生火系法术的符咒。" },
	{ "土灵符",     0x0047, TRIVIAL_RESOURCE, "产生土系法术的符咒。" },
	{ "舍利子",     0x0048, TRIVIAL_RESOURCE, "得道高僧佛身火化后，结成如珠状的东西。真气最大值+3" },
	{ "玉菩提",     0x0049, TRIVIAL_RESOURCE, "墨玉菩提树的种籽。真气最大值+5" },
	{ "银杏子",     0x004A, TRIVIAL_RESOURCE, "银杏树所结的白色核果。体力最大值+3" },
	{ "糯米",       0x004B, TRIVIAL_RESOURCE, "糯稻的米，富于黏性，可解尸毒，听说还可以制殭尸。" },
	{ "糯米糕",     0x004C, TRIVIAL_RESOURCE, "糯米加麦芽、甜豆所煮的米糕，可解尸毒，听说还可以制殭尸。体力+25" },
	{ "盐巴",       0x004D, TRIVIAL_RESOURCE, "取海水煎熬或曝晒而成，用来调味，有时可解毒。" },
	{ "茶叶蛋",     0x004E, TRIVIAL_RESOURCE, "鸡蛋水煮后，以茶叶入味。体力真气+15" },
	{ "鸡蛋",       0x004F, TRIVIAL_RESOURCE, "便宜而常见的食物。体力真气+10" },
	{ "糖葫芦",     0x0050, TRIVIAL_RESOURCE, "以竹签串李子，裹上麦芽糖，形如葫芦，故称「糖葫芦」。体力真气+22" },
	{ "蜡烛",       0x0051, CRITICAL_RESOURE, "用蜡制成来点火照明的东西，酒剑仙作法用的" },
	{ "符纸",       0x0052, CRITICAL_RESOURE, "泛黄的空白符纸，酒剑仙作法用的" },
	{ "檀香",       0x0053, CRITICAL_RESOURE, "含有檀香料的一种香，点燃用以熏衣、熏室内驱逐恶气。酒剑仙作法用的"},
	{ "大蒜",       0x0054, TRIVIAL_RESOURCE, "具有除秽、袪病、护身等功能，可以入药。战斗中服食可增加避毒率" },
	{ "黑狗血",     0x0055, TRIVIAL_RESOURCE, "" },
	{ "酒",         0x0056, CRITICAL_RESOURE, "以米加酒曲酿制而成，可解赤毒。体力真气+15。酒剑仙要喝的" },
	{ "雄黄",       0x0057, TRIVIAL_RESOURCE, "天然产的矿物，块状、色黄，可解赤毒。" },
	{ "雄黄酒",     0x0058, TRIVIAL_RESOURCE, "一点点的雄黄，撒在酒中，习俗在端午节喝这种酒；可解赤毒、瘴毒。" },
	{ "九节菖蒲",   0x0059, TRIVIAL_RESOURCE, "一种水草，叶子狭长如剑，可解赤毒、尸毒、瘴毒、毒丝。" },
	{ "驱魔香",     0x005A, ELITE___RESOURCE, "点燃后发出魔物厌恶的气味，使魔物不敢接近，使用时无法记录。" },
	{ "十里香",     0x005B, TRIVIAL_RESOURCE, "点燃后发出吸引魔物的香味，使用时无法记录。" },
	{ "水果",       0x005C, TRIVIAL_RESOURCE, "养颜美容、帮助消化。体力真气+20" },
	{ "烧肉",       0x005D, TRIVIAL_RESOURCE, "以炭火熏烤的酱汁猪肉。体力真气+30" },
	{ "腌肉",       0x005E, TRIVIAL_RESOURCE, "用盐巴腌渍的猪肉。体力+85" },
	{ "还魂香",     0x005F, TRIVIAL_RESOURCE, "点燃后会散发奇异的香气，能牵引离体魂魄回归躯体。复活，体力回复百分之十" },
	{ "赎魂灯",     0x0060, TRIVIAL_RESOURCE, "以莲灯作法与鬼差交涉，赎回死者魂魄。复活，体力回复百分之三十" },
	{ "孟婆汤",     0x0061, TRIVIAL_RESOURCE, "消除死者罪孽业障，使死者复活。复活，体力回复一半" },
	{ "天香续命露", 0x0062, ELITE___RESOURCE, "以大量珍贵秘药精炼而成，具有肉白骨、药死人之奇效。复活，体力全部回复" },
	{ "止血草",     0x0063, TRIVIAL_RESOURCE, "嚼碎后敷在伤口上，可迅速止血。体力+50" },
	{ "行军丹",     0x0064, TRIVIAL_RESOURCE, "活血顺气的药丸。体力+100" },
	{ "金创药",     0x0065, TRIVIAL_RESOURCE, "上等刀伤药，去腐生肌。体力+200" },
	{ "蟠果",       0x0066, TRIVIAL_RESOURCE, "西王母蟠桃园遗种，籽小肉厚汁液香甜。体力+450" },
	{ "紫菁玉蓉膏", 0x0067, ELITE___RESOURCE, "依宫廷秘方，采珍贵药材炼制，是疗伤药的极品。体力全满" },
	{ "鼠儿果",     0x0068, TRIVIAL_RESOURCE, "产于山间野地，多为鼠类所食，经人发现移种平地。真气+36" },
	{ "还神丹",     0x0069, TRIVIAL_RESOURCE, "宁神醒脑的药丸。真气+50" },
	{ "龙涎草",     0x006A, TRIVIAL_RESOURCE, "经蛟龙唾涎灌溉而生，具有补中益气之疗效。真气+110" },
	{ "灵山仙芝",   0x006B, CRITICAL_RESOURE, "寄生于枯木上的菌类，俗称瑞草，具有养气培元之神效。真气+250" },
	{ "雪莲子",     0x006C, TRIVIAL_RESOURCE, "白玉雪莲之莲子，其形珠圆玉润，服食者真气充盈，经脉通畅。真气+400" },
	{ "天仙玉露",   0x006D, TRIVIAL_RESOURCE, "观音菩萨净瓶甘露水，人间难求的仙界圣药。真气+700" },
	{ "神仙茶",     0x006E, TRIVIAL_RESOURCE, "神仙广成子养生延寿之秘方。体力真气+440" },
	{ "灵葫仙丹",   0x006F, TRIVIAL_RESOURCE, "修道隐士所炼丹药。体力真气+250" },
	{ "试炼果",     0x0070, TRIVIAL_RESOURCE, "药王神农氏尝百草时，最早发现的珍药。灵力+3" },
	{ "女娲石",     0x0071, TRIVIAL_RESOURCE, "女神娲皇炼石补天后，所遗之五色石。防御+3" },
	{ "八仙石",     0x0072, TRIVIAL_RESOURCE, "八仙石洞中所采集之丹矿。防御+2" },
	{ "蜂巢",       0x0073, TRIVIAL_RESOURCE, "蜜蜂的窝，可以拿来丢敌人。" },
	{ "尸腐肉",     0x0074, TRIVIAL_RESOURCE, "沾染尸毒的腐败肉块。尸毒" },
	{ "毒蛇卵",     0x0075, TRIVIAL_RESOURCE, "" },
	{ "毒蝎卵",     0x0076, TRIVIAL_RESOURCE, "" },
	{ "毒蟾卵",     0x0077, TRIVIAL_RESOURCE, "" },
	{ "蜘蛛卵",     0x0078, TRIVIAL_RESOURCE, "" },
	{ "蜈蚣卵",     0x0079, TRIVIAL_RESOURCE, "" },
	{ "鹤顶红",     0x007A, ELITE___RESOURCE, "七大毒蛊，中毒后每回合损血，至死方休。" },
	{ "孔雀胆",     0x007B, TRIVIAL_RESOURCE, "七大毒蛊，中毒后每回合耗损真气，至死方休。" },
	{ "血海棠",     0x007C, TRIVIAL_RESOURCE, "七大毒蛊，中毒后每回合损血，至死方休。" },
	{ "断肠草",     0x007D, TRIVIAL_RESOURCE, "七大毒蛊，中毒后每回合损血，至死方休。" },
	{ "醍醐香",     0x007E, TRIVIAL_RESOURCE, "紫叶小白花，散发浓郁香气，闻到香气，便如酒醉一般。定身五回合" },
	{ "忘魂花",     0x007F, TRIVIAL_RESOURCE, "青蓝色小花，散发淡淡香气，闻到香气，便会昏睡三回合。" },
	{ "紫罂粟",     0x0080, TRIVIAL_RESOURCE, "服食者会产生幻觉，敌我不分。疯魔三回合" },
	{ "鬼枯藤",     0x0081, TRIVIAL_RESOURCE, "具毒性的黑褐色野生藤蔓，可解赤毒、尸毒、瘴毒、毒丝。" },
	{ "腹蛇涎",     0x0082, TRIVIAL_RESOURCE, "腹蛇的毒涎。瘴毒" },
	{ "蜂王蜜",     0x0083, TRIVIAL_RESOURCE, "蜜蜂所酿最好的蜜。体力真气+150" },
	{ "雪蛤蟆",     0x0084, TRIVIAL_RESOURCE, "生长于天山极寒之地，仅铜钱般大小。武术、灵力、防御+2" },
	{ "赤蝎粉",     0x0085, TRIVIAL_RESOURCE, "以整只赤尾蝎研磨成的粉末，可使敌方全体中赤毒。" },
	{ "化尸水",     0x0086, TRIVIAL_RESOURCE, "碰到伤口血水，便腐蚀溃烂，受伤者沾染立毙。" },
	{ "迷魂香",     0x0087, TRIVIAL_RESOURCE, "点燃蒙汗药散发迷香，可使敌人昏睡五回合。" },
	{ "九阴散",     0x0088, TRIVIAL_RESOURCE, "服食前若已中毒，可补满体力，但无法解毒；服食前若没中毒，即刻毙命。" },
	{ "无影毒",     0x0089, ELITE___RESOURCE, "七大毒蛊，中毒后立即发作，耗损一半体力。" },
	{ "三尸蛊",     0x008A, TRIVIAL_RESOURCE, "七大毒蛊；中毒后，潜伏片刻即会发作，毒性非常猛烈。" },
	{ "金蚕蛊",     0x008B, TRIVIAL_RESOURCE, "七大毒蛊，中毒后每回合损失金钱，至死方休。" },
	{ "幻蛊",       0x008C, TRIVIAL_RESOURCE, "分泌的毒液会影响人的脑部，使人敌我不分。疯魔五回合" },
	{ "隐蛊",       0x008D, TRIVIAL_RESOURCE, "形如带刺甲虫，将其身体捏破，散发之烟雾可助我方隐匿形迹。全体隐形三回合。" },
	{ "冰蚕蛊",     0x008E, TRIVIAL_RESOURCE, "以雪山冰洞内所产冰蚕培养的蛊虫，可做为攻击道具。" },
	{ "火蚕蛊",     0x008F, TRIVIAL_RESOURCE, "以麒麟炎洞内所产火蚕所培养的蛊虫，可做为攻击道具。" },
	{ "食妖虫",     0x0090, TRIVIAL_RESOURCE, "寄生宿主吸取灵气，九回合后，可炼成灵蛊。" },
	{ "灵蛊",       0x0091, TRIVIAL_RESOURCE, "以稀有药物喂养的雌蛊。全体真气+250" },
	{ "爆烈蛊",     0x0092, ELITE___RESOURCE, "预先将法力灌输在蛊虫体内，再投掷敌人产生强烈爆炸。" },
	{ "碧血蚕",     0x0093, TRIVIAL_RESOURCE, "寄生宿主吸取血液，九回合后，可炼成赤血蚕。" },
	{ "蛊",         0x0094, TRIVIAL_RESOURCE, "巫师施法所需的材料。" },
	{ "赤血蚕",     0x0095, ELITE___RESOURCE, "吸血维生的毒蛊，使服食者体力完全恢复。体力全满" },
	{ "金蚕王",     0x0096, ELITE___RESOURCE, "蛊中之王，月夜散发金色磷光，服食后可提升修行。修行+1" },
	{ "引路蜂",     0x0097, ELITE___RESOURCE, "跟随引路蜂而行，可回到迷宫起点。" },
	{ "傀儡虫",     0x0098, CRITICAL_RESOURE, "湘西云贵巫师用以控制尸体，可使死者继续攻击九回合。" },
	{ "梅花镖",     0x0099, TRIVIAL_RESOURCE, "形如梅花的暗器。" },
	{ "袖里剑",     0x009A, TRIVIAL_RESOURCE, "暗藏在衣袖中的飞剑。" },
	{ "透骨钉",     0x009B, TRIVIAL_RESOURCE, "精铁打造、三寸长的铁针，是很锋利的暗器。" },
	{ "雷火珠",     0x009C, TRIVIAL_RESOURCE, "填充火药的铁珠，投掷撞击后会爆裂伤人。" },
	{ "毒龙砂",     0x009D, TRIVIAL_RESOURCE, "以腹蛇毒炼制成的细砂，可使敌方全体中瘴毒。" },
	{ "吸星锁",     0x009E, TRIVIAL_RESOURCE, "铁链钢抓，尾端系以灵蛊蚕丝，可吸取敌人体力。吸取敌人体力100" },
	{ "缠魂丝",     0x009F, TRIVIAL_RESOURCE, "千年蜘蛛的毒丝。毒丝" },
	{ "捆仙绳",     0x00A0, TRIVIAL_RESOURCE, "施有咒术的粗麻绳，可令妖怪动弹不得。定身五回合" },
	{ "无影神针",   0x00A1, TRIVIAL_RESOURCE, "细如牛毛，伤人于无形。" },
	{ "血玲珑",     0x00A2, TRIVIAL_RESOURCE, "红色铁球，四周装有锋利刀片。" },
	{ "长鞭",       0x00A3, TRIVIAL_RESOURCE, "生牛皮制的七尺软鞭，攻击敌方全体。武术+20、身法+20" },
	{ "九截鞭",     0x00A4, TRIVIAL_RESOURCE, "以铁节铁环组成的九节软鞭，攻击敌方全体。武术+66、身法+33" },
	{ "金蛇鞭",     0x00A5, ELITE___RESOURCE, "以蛇皮绞以金丝，编织成九尺软鞭，攻击敌方全体。武术+99、身法+60" },
	{ "木剑",       0x00A6, CRITICAL_RESOURE, "用木材雕刻的剑，小孩玩具。武术+2、身法+3" },
	{ "短刀",       0x00A7, TRIVIAL_RESOURCE, "一尺半长的钝刀，可用来劈砍木材。武术+6、身法－5" },
	{ "铁剑",       0x00A8, TRIVIAL_RESOURCE, "一般铁匠大量生产的剑，打造得颇为粗劣。武术+10、防御+3" },
	{ "大刀",       0x00A9, TRIVIAL_RESOURCE, "刀身宽而长，刃部锋利，背部厚重。武术+16、防御+1" },
	{ "仙女剑",     0x00AA, TRIVIAL_RESOURCE, "一尺长的双手剑，适合女子使用，可发出两次攻击。武术+8、防御+5" },
	{ "长剑",       0x00AB, TRIVIAL_RESOURCE, "一般铁匠接受订造的剑，比铁剑精致锋利。武术+25" },
	{ "红缨刀",     0x00AC, TRIVIAL_RESOURCE, "精钢打造，背厚刃薄，刀柄饰以红色长穗。武术+38" },
	{ "越女剑",     0x00AD, TRIVIAL_RESOURCE, "剑身宽仅两指，专为女子打造。武术+22、身法+18" },
	{ "戒刀",       0x00AE, TRIVIAL_RESOURCE, "佛门中人练武所用之刀，严禁伤生染血。武术+55、灵力+10、防御+5" },
	{ "玄铁剑",     0x00AF, TRIVIAL_RESOURCE, "以珍贵的黑色铁矿打造而成，坚韧锋利但极笨重。武术+70、灵力－15、防御+9" },
	{ "芙蓉刀",     0x00B0, TRIVIAL_RESOURCE, "百花派独门兵器双手弯刀，可发出两次攻击。武术+16、身法+8" },
	{ "柳月刀",     0x00B1, TRIVIAL_RESOURCE, "细长铁制双刀，形如柳叶新月，可发出两次攻击。防御+3、武术+28、吉运+12" },
	{ "青锋剑",     0x00B2, TRIVIAL_RESOURCE, "名家精心打造的剑，轻薄锋利。武术+75、身法+26" },
	{ "苗刀",       0x00B3, TRIVIAL_RESOURCE, "苗族战士所惯用的佩刀。武术+70、身法+32" },
	{ "凤鸣刀",     0x00B4, TRIVIAL_RESOURCE, "出鞘之声有如凤鸣，故称「凤鸣刀」。武术+124、灵力+16、防御+9" },
	{ "双龙剑",     0x00B5, TRIVIAL_RESOURCE, "与一般剑长度相同的双手剑，可发出两次攻击。武术+62、防御+9、身法+9" },
	{ "玉女剑",     0x00B6, TRIVIAL_RESOURCE, "鸳鸯双剑中的雌剑，与金童剑为一对。武术+100、灵力+15、身法+20" },
	{ "金童剑",     0x00B7, TRIVIAL_RESOURCE, "鸳鸯双剑中的雄剑，与玉女剑为一对。武术+100、灵力+15、防御+3" },
	{ "龙泉剑",     0x00B8, TRIVIAL_RESOURCE, "龙泉的水质非常适合造剑，当地生产的剑叫龙泉剑。武术+88、身法+20" },
	{ "鬼牙刀",     0x00B9, TRIVIAL_RESOURCE, "苗刀的一种，刀尖倒钩，又称「勾魂刀」。武术+90、身法+26、吉运－9" },
	{ "七星剑",     0x00BA, TRIVIAL_RESOURCE, "剑身镶嵌七颗金黄宝石，可吸取北斗七星之精气。武术+120、灵力+50、防御+7" },
	{ "玄冥宝刀",   0x00BB, ELITE___RESOURCE, "可连续攻击敌方全体两次，传说是魔族的邪异兵器。效果不明" },
	{ "巫月神刀",   0x00BC, ELITE___RESOURCE, "苗族拜月教镇教之宝。效果不明" },
	{ "磐龙剑",     0x00BD, TRIVIAL_RESOURCE, "铸剑宗师欧冶子所炼宝剑，剑身铸有青龙盘柱。武术+135、灵力+37、防御+8" },
	{ "太极剑",     0x00BE, TRIVIAL_RESOURCE, "道祖张陵之随身配剑，天师仗以降妖伏魔。武术+158、灵力+90、防御+35" },
	{ "无尘剑",     0x00BF, ELITE___RESOURCE, "上古神剑，指天天崩、划地地裂。武术+200、防御+20、身法+77、吉运+33" },
	{ "青蛇杖",     0x00C0, TRIVIAL_RESOURCE, "雕刻双蛇缠绕的绿玉杖。武术+50、灵力+62、防御+6" },
	{ "鬼头杖",     0x00C1, TRIVIAL_RESOURCE, "苗族巫师役鬼炼蛊之法器，头颅中囚禁四十九条生魂。武术+70、灵力+88" },
	{ "冥蛇杖",     0x00C2, TRIVIAL_RESOURCE, "来自冥界之魔杖，号令群邪，镶嵌千年蛇王内丹。武术+88、灵力+120" },
	{ "天蛇杖",     0x00C3, ELITE___RESOURCE, "女神娲皇炼化五色石所用法杖。武术+100、灵力+150、防御+33" },
	{ "头巾",       0x00C4, TRIVIAL_RESOURCE, "以剩余布料缝制的头巾。防御+1" },
	{ "青丝巾",     0x00C5, TRIVIAL_RESOURCE, "青色的丝织发带。防御+2" },
	{ "发饰",       0x00C6, TRIVIAL_RESOURCE, "锡制的女子头饰。防御+3" },
	{ "银钗",       0x00C7, TRIVIAL_RESOURCE, "纯银的发钗。防御+5" },
	{ "翠玉金钗",   0x00C8, TRIVIAL_RESOURCE, "镶有绿翡翠的黄金发钗。防御+9" },
	{ "皮帽",       0x00C9, TRIVIAL_RESOURCE, "羊皮缝制的帽子，非常保暖。防御+4" },
	{ "珍珠冠",     0x00CA, TRIVIAL_RESOURCE, "以珍珠缝缀的红色锦冠。防御+13" },
	{ "天师帽",     0x00CB, TRIVIAL_RESOURCE, "道士做法时所戴的帽子。灵力+3、防御+11" },
	{ "紫金冠",     0x00CC, TRIVIAL_RESOURCE, "紫金冠以薄铜片铸成，外壳以紫饰金而成。防御+18" },
	{ "天蚕丝带",   0x00CD, ELITE___RESOURCE, "以极珍贵的天蚕丝织成，轻薄柔韧。防御+25、身法+8" },
	{ "凤凰羽毛",   0x00CE, ELITE___RESOURCE, "金翅凤凰腹部的银色羽毛。防御+7、身法+24、吉运+9" },
	{ "冲天冠",     0x00CF, ELITE___RESOURCE, "天兵神将遗留的护头金盔，顶插双雉尾羽。灵力+3、防御+28、身法+5、吉运+3" },
	{ "布袍",       0x00D0, TRIVIAL_RESOURCE, "粗布缝制的交领长袖白袍。防御+3" },
	{ "藤甲",       0x00D1, TRIVIAL_RESOURCE, "以荆藤编制的护甲。防御+7" },
	{ "丝衣",       0x00D2, TRIVIAL_RESOURCE, "以蚕丝纺织而成，轻柔透气。防御+3、身法+4" },
	{ "铁锁衣",     0x00D3, TRIVIAL_RESOURCE, "以铁环扣锁制成的护甲。防御+13、身法－10" },
	{ "夜行衣",     0x00D4, ELITE___RESOURCE, "暗黑色的紧身衣靠，便于隐匿夜色之中。防御+18、身法+12、吉运+12" },
	{ "青铜甲",     0x00D5, TRIVIAL_RESOURCE, "青铜制的兽面纹胸护甲。防御+22、身法－13" },
	{ "罗汉袍",     0x00D6, TRIVIAL_RESOURCE, "修行得道的和尚所穿的衣袍。灵力+10、防御+10、吉运+10" },
	{ "铁鳞甲",     0x00D7, TRIVIAL_RESOURCE, "以鱼鳞形甲片编缀而成的铠甲。防御+28、身法－4" },
	{ "天师道袍",   0x00D8, TRIVIAL_RESOURCE, "天师道祖修行时所穿的法衣。灵力+28、防御+33" },
	{ "精铁战甲",   0x00D9, TRIVIAL_RESOURCE, "以椭圆形的精铁片编缀而成，光亮照人，又称「光明铠」。防御+40、身法－7" },
	{ "金缕衣",     0x00DA, TRIVIAL_RESOURCE, "以金线穿玉片编制而成，又称「金缕玉衣」。防御+47、身法－10" },
	{ "鬼针胄",     0x00DB, ELITE___RESOURCE, "布满倒刺的铜制盔甲。武术+9、防御+55、身法－6" },
	{ "天蚕宝衣",   0x00DC, TRIVIAL_RESOURCE, "以极珍贵的天蚕丝织成，轻薄柔韧。防御+66" },
	{ "青龙宝甲",   0x00DD, ELITE___RESOURCE, "龙鳞编缀而成，世间绝顶战甲。防御+90" },
	{ "白虎之铠",   0x00DE, TRIVIAL_RESOURCE, "以罕见的白虎皮制成的皮甲。防御+80" },
	{ "玄武战袍",   0x00DF, ELITE___RESOURCE, "以玄武的壳甲锻造而成，材质坚韧色黑而无光泽。防御+80" },
	{ "朱雀战衣",   0x00E0, ELITE___RESOURCE, "以南方火鸟的羽毛编织而成。防御+80" },
	{ "披风",       0x00E1, TRIVIAL_RESOURCE, "无领对襟、无袖的披衣，俗称「斗篷」。防御+2" },
	{ "护肩",       0x00E2, TRIVIAL_RESOURCE, "披于肩臂上的铠甲，又称「掩膊」。防御+6" },
	{ "武士披风",   0x00E3, TRIVIAL_RESOURCE, "将帅所穿有护肩软甲的战帔。防御+12" },
	{ "护心镜",     0x00E4, ELITE___RESOURCE, "防护前胸要害的披甲，形如铜镜。防御+20" },
	{ "霓虹羽衣",   0x00E5, TRIVIAL_RESOURCE, "东海霓虹鸟的羽毛织成的披肩。防御+18、身法+18、吉运+18" },
	{ "菩提袈裟",   0x00E6, TRIVIAL_RESOURCE, "高等僧衣，又名「无垢衣」，多为高僧与长老所穿。灵力+16、防御+31" },
	{ "虎纹披风",   0x00E7, TRIVIAL_RESOURCE, "以整张千年白额虎虎皮制成，毛皮呈黄色，带黑色横纹。防御+40" },
	{ "凤纹披风",   0x00E8, ELITE___RESOURCE, "相传为织女缝制的披风，绣凤织锦，光彩夺目。防御+52" },
	{ "龙纹披风",   0x00E9, ELITE___RESOURCE, "布面绣双龙抢珠之彩纹，有神龙护体之功效。防御+60" },
	{ "圣灵披风",   0x00EA, CRITICAL_RESOURE, "巫后的遗物，潜藏神圣的力量。灵力+30、防御+66" },
	{ "草鞋",       0x00EB, TRIVIAL_RESOURCE, "以蔺草编织而成，十分便宜，穿起来很轻便，适宜行走。防御+1" },
	{ "木鞋",       0x00EC, TRIVIAL_RESOURCE, "以木材削制而成，鞋面刻有吉祥图案。防御+2" },
	{ "布靴",       0x00ED, CRITICAL_RESOURE, "粗布缝制的长统靴。防御+3、身法+2" },
	{ "绣花鞋",     0x00EE, TRIVIAL_RESOURCE, "以丝缎缝制，鞋面绣有龙头凤尾花。防御+4" },
	{ "铁履",       0x00EF, TRIVIAL_RESOURCE, "鞋底夹缝铁片，较普通布靴重。防御+6" },
	{ "武僧靴",     0x00F0, TRIVIAL_RESOURCE, "罗汉僧练武所穿的布靴。防御+8、身法+6" },
	{ "鹿皮靴",     0x00F1, TRIVIAL_RESOURCE, "鞋面以鹿皮毛缝制，质地轻柔，行动可如鹿般迅捷。防御+11、身法+9" },
	{ "疾风靴",     0x00F2, TRIVIAL_RESOURCE, "以薄如云雾的蝉纱织成，助穿者疾行如风。防御+14、身法+17" },
	{ "莲花靴",     0x00F3, TRIVIAL_RESOURCE, "饰以金莲的长筒绣花鞋。防御+18、身法+5" },
	{ "虎皮靴",     0x00F4, TRIVIAL_RESOURCE, "取自东北虎的皮毛缝制。防御+21、身法+12" },
	{ "龙鳞靴",     0x00F5, TRIVIAL_RESOURCE, "以龙鳞编缀而成。防御+25、身法+16" },
	{ "步云靴",     0x00F6, ELITE___RESOURCE, "云中子羽化登仙后，所遗留之神靴。吉运+28、身法+20" },
	{ "魅影神靴",   0x00F7, ELITE___RESOURCE, "妖魔附体，身如鬼魅。防御+32、身法+26" },
	{ "香袋",       0x00F8, TRIVIAL_RESOURCE, "填充木屑、香粉的小布包，常用来装饰兼避邪的物品。灵力+8、吉运+18" },
	{ "护腕",       0x00F9, TRIVIAL_RESOURCE, "粗布缝制之腕部护套。防御+2" },
	{ "铁护腕",     0x00FA, TRIVIAL_RESOURCE, "精钢打造之腕部护环。防御+5" },
	{ "竹笛",       0x00FB, TRIVIAL_RESOURCE, "青竹削制之七孔横笛。吉运+18" },
	{ "珍珠",       0x00FC, TRIVIAL_RESOURCE, "蚌类所生的球状物，是珍贵的装饰品。吉运+20" },
	{ "玉镯",       0x00FD, TRIVIAL_RESOURCE, "戴在手臂上的玉制环形首饰。防御+5、吉运+9" },
	{ "念珠",       0x00FE, TRIVIAL_RESOURCE, "佛教徒记数念经咒，或念佛号次数的计算珠。灵力+5、防御+5" },
	{ "银针",       0x00FF, TRIVIAL_RESOURCE, "用银针刺肉，以痛楚唤醒神智，可解定身、昏睡、疯魔。" },
	{ "铜镜",       0x0100, TRIVIAL_RESOURCE, "青铜铸造的照容用具。防御+6" },
	{ "八卦镜",     0x0101, TRIVIAL_RESOURCE, "用朱砂在镜面画八卦，可借用自然界的灵气。灵力+8、防御+8" },
	{ "乾坤镜",     0x0102, ELITE___RESOURCE, "铜镜背面铸有太极乾坤图，可吸取天地阴阳灵气。灵力+14、防御+14" },
	{ "豹牙手环",   0x0103, ELITE___RESOURCE, "收集花豹的利牙串成的手环。防御+9" },
	{ "圣灵珠",     0x0104, CRITICAL_RESOURE, "女娲末族祖传宝物，历代圣魂归依之所。灵力+128、防御+15。求雨时用" },
	{ "金罡珠",     0x0105, TRIVIAL_RESOURCE, "大罗金仙修炼千年的内丹。防御+90" },
	{ "五毒珠",     0x0106, CRITICAL_RESOURE, "成精蟾怪的内丹，配戴后百毒不侵。" },
	{ "风灵珠",     0x0107, CRITICAL_RESOURE, "女娲降伏风神后，禁制风神于内的宝珠。求雨时用" },
	{ "雷灵珠",     0x0108, CRITICAL_RESOURE, "女娲降伏雷神后，禁制雷神于内的宝珠。求雨时用" },
	{ "水灵珠",     0x0109, CRITICAL_RESOURE, "女娲降伏雪妖后，禁制雪妖于内的宝珠。求雨时用" },
	{ "火灵珠",     0x010A, CRITICAL_RESOURE, "女娲降伏火神后，禁制火神于内的宝珠。求雨时用" },
	{ "土灵珠",     0x010B, CRITICAL_RESOURE, "女娲降伏山神后，禁制山神于内的宝珠。求雨时用" },
	{ "炼蛊皿",     0x010C, TRIVIAL_RESOURCE, "可将蛊卵炼成各种蛊、亦可将各式虫蛊炼化成其它虫蛊。" },
	{ "寿葫芦",     0x010D, TRIVIAL_RESOURCE, "战斗中涌出活力补充持有者，有滋养恢复之奇效。" },
	{ "紫金葫芦",   0x010E, CRITICAL_RESOURE, "收妖炼丹；需与灵葫咒配合。将商人尸体身上得到的布包交给其妻子可得到此物" },
	{ "布包",       0x010F, CRITICAL_RESOURE, "长安富商的行李。" },
	{ "桂花酒",     0x0110, CRITICAL_RESOURE, "李逍遥家的客栈卖的酒。" },
	{ "紫金丹",     0x0111, CRITICAL_RESOURE, "赵灵儿所赠的仙药。" },
	{ "玉佛珠",     0x0112, CRITICAL_RESOURE, "西方如来坛前的念珠，经佛法熏陶变化通灵。灵力+88、防御+18" },
	{ "金凤凰蛋壳", 0x0113, CRITICAL_RESOURE, "药材" },
	{ "火眼麒麟角", 0x0114, CRITICAL_RESOURE, "药材" },
	{ "青龙碧血玉", 0x0115, CRITICAL_RESOURE, "传说中将神龙封印在其中的宝玉。" },
	{ "毒龙胆",     0x0116, TRIVIAL_RESOURCE, "千年毒蛟的胆，以毒攻毒可解天下所有的毒；若没中毒吃毒龙胆会毙命。" },
	{ "破天槌",     0x0117, CRITICAL_RESOURE, "用来敲碎仙灵岛石像的法宝。" },
	{ "包袱",       0x0118, CRITICAL_RESOURE, "婶婶替逍遥收拾的行李。" },
	{ "银杏果",     0x0119, CRITICAL_RESOURE, "药材" },
	{ "鲤鱼",       0x011A, CRITICAL_RESOURE, "药材" },
	{ "鹿茸",       0x011B, CRITICAL_RESOURE, "药材" },
	{ "钓竿",       0x011C, CRITICAL_RESOURE, "白河村渔夫的钓竿，用完记得要还给人家！" },
	{ "捕兽夹",     0x011D, CRITICAL_RESOURE, "猎户放置的捕鹿陷阱。" },
	{ "六神丹",     0x011E, CRITICAL_RESOURE, "韩家药铺的祖传良药。" },
	{ "情书",       0x011F, CRITICAL_RESOURE, "士兵委托的情书。" },
	{ "玉佩",       0x0120, CRITICAL_RESOURE, "婢女委托的玉佩。" },
	{ "石钥匙",     0x0121, CRITICAL_RESOURE, "开启隐龙窟后洞石门的钥匙。" },
	{ "天书",       0x0122, CRITICAL_RESOURE, "书中仙附身于书中。" },
	{ "香蕉",       0x0123, CRITICAL_RESOURE, "进入神木林取凤凰蛋壳需要给猴子，在圣姑家门前的树上可以摘到" },
	{ "凤纹手绢",   0x0124, CRITICAL_RESOURE, "姥姥交付的信物。" },
	{ "手卷",       0x0125, CRITICAL_RESOURE, "武功秘笈。" },
	{ "芦苇漂",     0x0126, TRIVIAL_RESOURCE, "可载人漂浮水面的草席。 " }
};

BOOL ResourcePage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_Grid.SetVirtualMode(FALSE);
    m_Grid.SetEditable(TRUE);
	m_Grid.SetRowResize(TRUE);
	m_Grid.SetHeaderSort(FALSE);
	m_Grid.SetAutoSizeStyle();
	
    int m_nFixCols = 1;
	int m_nFixRows = 1;
	int m_nCols    = sizeof(ColName)/sizeof(ColName[0]);
	int m_nRows    = sizeof(AllResource)/sizeof(AllResource[0]) + 1;

	TRY
	{
		m_Grid.SetRowCount(m_nRows);
		m_Grid.SetColumnCount(m_nCols);
		m_Grid.SetFixedRowCount(m_nFixRows);
		m_Grid.SetFixedColumnCount(m_nFixCols);
	}
	CATCH (CMemoryException, e)
	{
	    e->ReportError();
    	return TRUE;
	}
    END_CATCH

    CString str, s;
	GV_ITEM Item;    
	int row, col;

	for (row = 1; row < m_Grid.GetRowCount(); row++)
    {
		//物品名称
		Item.mask = GVIF_TEXT;
		Item.row  = row;
		Item.col  = 0;
        str.Format(_T("%s"), AllResource[row - 1].m_ResName);
        Item.strText = str;
	   	m_Grid.SetItem(&Item);
		m_Grid.SetItemBkColour(row, 0, RGB(128, 255, 128));	

		//是否有
		m_Grid.SetCellType(row, 1, RUNTIME_CLASS(CGridCellCheck));

		//数量
		m_Grid.SetCellType(row, 2, RUNTIME_CLASS(CGridCellNumeric));

		//物品编号
		s.Format(_T("0x%04lX"), AllResource[row - 1].m_ResID);
		m_Grid.SetItemText(row, 3, s);
		m_Grid.GetCell(row, 3)->SetState(GVIS_READONLY);

		//是否剧情相关
		m_Grid.SetCellType(row, 4, RUNTIME_CLASS(CGridCellCheck));
		CGridCellCheck *pCellCheck = (CGridCellCheck *)m_Grid.GetCell(row, 4);
		pCellCheck->SetCheck(AllResource[row - 1].m_ResType == CRITICAL_RESOURE? TRUE:FALSE);
		m_Grid.GetCell(row, 4)->SetState(GVIS_READONLY);

		//物品描述
		m_Grid.SetItemText(row, 5, AllResource[row - 1].m_ResDesc);
		m_Grid.GetCell(row, 5)->SetState(GVIS_READONLY);
	}
	
	//标题行
    for (col = 0; col < m_Grid.GetColumnCount(); col++)
	{ 
		Item.mask = GVIF_TEXT;
		Item.row  = 0;
		Item.col  = col;
        str.Format(_T("%s"), ColName[col]);
		Item.strText = str;
		m_Grid.SetItem(&Item);

		m_Grid.SetItemBkColour(0, col, RGB(255, 255, 0));
    }
	
	m_Grid.ExpandLastColumn();
	m_Grid.AutoSizeRows();

	MemoryDataToDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ResourcePage::MemoryDataToDialog()
{
	CString s;
	CGridCellCheck *pCellCheck;

	int row;
	for (row = 1; row < m_Grid.GetRowCount(); row++)
	{
		//是否拥有
		pCellCheck = (CGridCellCheck *)m_Grid.GetCell(row, 1);
		pCellCheck->SetCheck(FALSE);

		//数量
		m_Grid.SetItemText(row, 2, "");
	}

	m_TotalResource = 0;

	struct Resource  *pResInfo = g_SaveInfo.m_Resource;
	while(pResInfo->m_ResID && pResInfo->m_ResCount)
	{
		row = pResInfo->m_ResID - ResStartID + 1;                 //物品编号对应到行号
		if (row + 1  > m_Grid.GetRowCount()) continue;            //越界
		
		//是否拥有该物品
		pCellCheck = (CGridCellCheck *)m_Grid.GetCell(row, 1);
		pCellCheck->SetCheck();

		//该物品的数量
		s.Format(_T("%d"), pResInfo->m_ResCount);
		m_Grid.SetItemText(row, 2, s);

		pResInfo++;
		m_TotalResource++;
	}

	m_Grid.Invalidate();
	UpdateData(FALSE);
}

BOOL ResourcePage::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
    if (wParam == (WPARAM)m_Grid.GetDlgCtrlID())
    {
        *pResult = 1;
        NM_GRIDVIEW *pInfo = (NM_GRIDVIEW *)lParam;
        if (GVN_CHECKCHANGED == pInfo->hdr.code)
        {
			if (pInfo->iColumn == 1)
			{
				CGridCellCheck *pCellCheck = (CGridCellCheck *)m_Grid.GetCell(pInfo->iRow, pInfo->iColumn);
				if (pCellCheck)
				{
					if (pCellCheck->GetCheck())
					{
						m_Grid.SetItemText(pInfo->iRow, 2, "99");
						m_TotalResource++;
					}
					else
					{
						m_Grid.SetItemText(pInfo->iRow, 2, "");
						m_TotalResource--;
					}
					
					m_Grid.RedrawCell(pInfo->iRow, 2);
					UpdateData(FALSE);
				}
			}
            return TRUE;
        }
    }

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void ResourcePage::OnButtonEliteResource() 
{
	// TODO: Add your control notification handler code here
	m_TotalResource = 0;
	for(int row = 1; row < m_Grid.GetRowCount(); row++)
	{
		//跳过剧情相关的物品
		if (AllResource[row - 1].m_ResType == CRITICAL_RESOURE)
			continue;

		//是否拥有该物品
		BOOL Checked = ((AllResource[row - 1].m_ResType == ELITE___RESOURCE) ? TRUE: FALSE);
		CGridCellCheck *pCellCheck = (CGridCellCheck *)m_Grid.GetCell(row, 1);
		pCellCheck->SetCheck(Checked);

		//该物品的数量
		if (Checked)
		{
			m_Grid.SetItemText(row, 2, "99");
			m_TotalResource++;
		}
	}
	m_Grid.Invalidate();
	UpdateData(FALSE);
}

void ResourcePage::OnButtonFullResource() 
{
	// TODO: Add your control notification handler code here
	for(int row = 1; row < m_Grid.GetRowCount(); row++)
	{
		//是否拥有该物品
		CGridCellCheck *pCellCheck = (CGridCellCheck *)m_Grid.GetCell(row, 1);
		pCellCheck->SetCheck();

		//该物品的数量
		m_Grid.SetItemText(row, 2, "99");
	}
	m_Grid.Invalidate();

	m_TotalResource = sizeof(AllResource)/sizeof(AllResource[0]);
	UpdateData(FALSE);
}

void ResourcePage::OnButtonResetResource() 
{
	// TODO: Add your control notification handler code here
	MemoryDataToDialog();
}

void ResourcePage::DialogDataToMemory()
{
	UpdateData(TRUE);

	memset(g_SaveInfo.m_Resource, 0, sizeof(g_SaveInfo.m_Resource));
	
	CString s;
	CGridCellCheck *pCellCheck;

	struct Resource  *pResInfo = g_SaveInfo.m_Resource;
	for (int row = 1; row < m_Grid.GetRowCount(); row++)
	{		
		pCellCheck = (CGridCellCheck *)m_Grid.GetCell(row, 1);
		if (pCellCheck->GetCheck())                                   //是否拥有
		{
			pResInfo->m_ResID    = ResStartID + row - 1;
			pResInfo->m_ResCount = atol(m_Grid.GetItemText(row, 2));  //数量
			pResInfo++;
		}
	}
}
