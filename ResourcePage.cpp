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
	"����", "�з�", "����", "���", "�������", "��Ʒ����"
};

enum RESOURCE_TYPE
{
	CRITICAL_RESOURE,           //���鷢չ����Ҫ�ĵ���
	ELITE___RESOURCE   ,        //��Ʒװ��
	TRIVIAL_RESOURCE            //�Ǽ�Ʒװ��
};

struct ResourceInfo
{
	char          *m_ResName;             //��Ʒ����
	short          m_ResID;               //��Ʒ���
	RESOURCE_TYPE  m_ResType;             //������Ҫ����Ʒ
	char           *m_ResDesc;            //��Ʒ������Ϣ
};

static const short ResStartID = 0x003D;

static const struct ResourceInfo AllResource[] =
{
	{ "������",     0x003D, TRIVIAL_RESOURCE, "�Թ���ʥˮ��д�����������+150" },
	{ "ʥ���",     0x003E, TRIVIAL_RESOURCE, "����Ů���ǿ�������ķ��䡣ȫ������+300" },
	{ "��շ�",     0x003F, TRIVIAL_RESOURCE, "ʹ�ú����н������ֻ������ӷ����߻غ�" },
	{ "���·�",     0x0040, TRIVIAL_RESOURCE, "����Р������а�ķ������ɽ�඾��ʬ�����ζ���" },
	{ "���ķ�",     0x0041, TRIVIAL_RESOURCE, "����������а����Ч���ɽ��ħ��������˯����⡣" },
	{ "��ʦ��",     0x0042, TRIVIAL_RESOURCE, "éɽ��ʿ�����Ը����ֵķ��䡣" },
	{ "�����",     0x0043, TRIVIAL_RESOURCE, "������ϵ�����ķ��䡣" },
	{ "�����",     0x0044, TRIVIAL_RESOURCE, "������ϵ�����ķ��䡣" },
	{ "ˮ���",     0x0045, TRIVIAL_RESOURCE, "������ϵ�����ķ��䡣" },
	{ "�����",     0x0046, TRIVIAL_RESOURCE, "������ϵ�����ķ��䡣" },
	{ "�����",     0x0047, TRIVIAL_RESOURCE, "������ϵ�����ķ��䡣" },
	{ "������",     0x0048, TRIVIAL_RESOURCE, "�õ���ɮ����𻯺󣬽������״�Ķ������������ֵ+3" },
	{ "������",     0x0049, TRIVIAL_RESOURCE, "ī�������������ѡ��������ֵ+5" },
	{ "������",     0x004A, TRIVIAL_RESOURCE, "����������İ�ɫ�˹����������ֵ+3" },
	{ "Ŵ��",       0x004B, TRIVIAL_RESOURCE, "Ŵ�����ף�������ԣ��ɽ�ʬ������˵�������ƚ�ʬ��" },
	{ "Ŵ�׸�",     0x004C, TRIVIAL_RESOURCE, "Ŵ�׼���ѿ����������׸⣬�ɽ�ʬ������˵�������ƚ�ʬ������+25" },
	{ "�ΰ�",       0x004D, TRIVIAL_RESOURCE, "ȡ��ˮ�尾����ɹ���ɣ�������ζ����ʱ�ɽⶾ��" },
	{ "��Ҷ��",     0x004E, TRIVIAL_RESOURCE, "����ˮ����Բ�Ҷ��ζ����������+15" },
	{ "����",       0x004F, TRIVIAL_RESOURCE, "���˶�������ʳ���������+10" },
	{ "�Ǻ�«",     0x0050, TRIVIAL_RESOURCE, "����ǩ�����ӣ�������ѿ�ǣ������«���ʳơ��Ǻ�«������������+22" },
	{ "����",       0x0051, CRITICAL_RESOURE, "�����Ƴ�����������Ķ������ƽ��������õ�" },
	{ "��ֽ",       0x0052, CRITICAL_RESOURE, "���ƵĿհ׷�ֽ���ƽ��������õ�" },
	{ "̴��",       0x0053, CRITICAL_RESOURE, "����̴���ϵ�һ���㣬��ȼ����Ѭ�¡�Ѭ��������������ƽ��������õ�"},
	{ "����",       0x0054, TRIVIAL_RESOURCE, "���г��ࡢР��������ȹ��ܣ�������ҩ��ս���з�ʳ�����ӱܶ���" },
	{ "�ڹ�Ѫ",     0x0055, TRIVIAL_RESOURCE, "" },
	{ "��",         0x0056, CRITICAL_RESOURE, "���׼Ӿ������ƶ��ɣ��ɽ�඾����������+15���ƽ���Ҫ�ȵ�" },
	{ "�ۻ�",       0x0057, TRIVIAL_RESOURCE, "��Ȼ���Ŀ����״��ɫ�ƣ��ɽ�඾��" },
	{ "�ۻƾ�",     0x0058, TRIVIAL_RESOURCE, "һ�����ۻƣ����ھ��У�ϰ���ڶ���ں����־ƣ��ɽ�඾���ζ���" },
	{ "�Ž�����",   0x0059, TRIVIAL_RESOURCE, "һ��ˮ�ݣ�Ҷ�������罣���ɽ�඾��ʬ�����ζ�����˿��" },
	{ "��ħ��",     0x005A, ELITE___RESOURCE, "��ȼ�󷢳�ħ��������ζ��ʹħ�ﲻ�ҽӽ���ʹ��ʱ�޷���¼��" },
	{ "ʮ����",     0x005B, TRIVIAL_RESOURCE, "��ȼ�󷢳�����ħ�����ζ��ʹ��ʱ�޷���¼��" },
	{ "ˮ��",       0x005C, TRIVIAL_RESOURCE, "�������ݡ�������������������+20" },
	{ "����",       0x005D, TRIVIAL_RESOURCE, "��̿��Ѭ���Ľ�֭���⡣��������+30" },
	{ "����",       0x005E, TRIVIAL_RESOURCE, "���ΰ����յ����⡣����+85" },
	{ "������",     0x005F, TRIVIAL_RESOURCE, "��ȼ���ɢ���������������ǣ��������ǻع����塣��������ظ��ٷ�֮ʮ" },
	{ "����",     0x0060, TRIVIAL_RESOURCE, "��������������棬������߻��ǡ���������ظ��ٷ�֮��ʮ" },
	{ "������",     0x0061, TRIVIAL_RESOURCE, "������������ҵ�ϣ�ʹ���߸����������ظ�һ��" },
	{ "��������¶", 0x0062, ELITE___RESOURCE, "�Դ��������ҩ�������ɣ�������׹ǡ�ҩ����֮��Ч���������ȫ���ظ�" },
	{ "ֹѪ��",     0x0063, TRIVIAL_RESOURCE, "���������˿��ϣ���Ѹ��ֹѪ������+50" },
	{ "�о���",     0x0064, TRIVIAL_RESOURCE, "��Ѫ˳����ҩ�衣����+100" },
	{ "��ҩ",     0x0065, TRIVIAL_RESOURCE, "�ϵȵ���ҩ��ȥ������������+200" },
	{ "��",       0x0066, TRIVIAL_RESOURCE, "����ĸ���԰���֣���С���֭Һ��������+450" },
	{ "��ݼ���ظ�", 0x0067, ELITE___RESOURCE, "����͢�ط��������ҩ�����ƣ�������ҩ�ļ�Ʒ������ȫ��" },
	{ "�����",     0x0068, TRIVIAL_RESOURCE, "����ɽ��Ұ�أ���Ϊ������ʳ�����˷�������ƽ�ء�����+36" },
	{ "����",     0x0069, TRIVIAL_RESOURCE, "�������Ե�ҩ�衣����+50" },
	{ "���Ѳ�",     0x006A, TRIVIAL_RESOURCE, "���������ѹ�ȶ��������в�������֮��Ч������+110" },
	{ "��ɽ��֥",   0x006B, CRITICAL_RESOURE, "�����ڿ�ľ�ϵľ��࣬�׳���ݣ�����������Ԫ֮��Ч������+250" },
	{ "ѩ����",     0x006C, TRIVIAL_RESOURCE, "����ѩ��֮���ӣ�������Բ���󣬷�ʳ��������ӯ������ͨ��������+400" },
	{ "������¶",   0x006D, TRIVIAL_RESOURCE, "����������ƿ��¶ˮ���˼�������ɽ�ʥҩ������+700" },
	{ "���ɲ�",     0x006E, TRIVIAL_RESOURCE, "���ɹ������������֮�ط�����������+440" },
	{ "����ɵ�",   0x006F, TRIVIAL_RESOURCE, "�޵���ʿ������ҩ����������+250" },
	{ "������",     0x0070, TRIVIAL_RESOURCE, "ҩ����ũ�ϳ��ٲ�ʱ�����緢�ֵ���ҩ������+3" },
	{ "Ů�ʯ",     0x0071, TRIVIAL_RESOURCE, "Ů��活���ʯ���������֮��ɫʯ������+3" },
	{ "����ʯ",     0x0072, TRIVIAL_RESOURCE, "����ʯ�������ɼ�֮���󡣷���+2" },
	{ "�䳲",       0x0073, TRIVIAL_RESOURCE, "�۷���ѣ��������������ˡ�" },
	{ "ʬ����",     0x0074, TRIVIAL_RESOURCE, "մȾʬ���ĸ�����顣ʬ��" },
	{ "������",     0x0075, TRIVIAL_RESOURCE, "" },
	{ "��Ы��",     0x0076, TRIVIAL_RESOURCE, "" },
	{ "�����",     0x0077, TRIVIAL_RESOURCE, "" },
	{ "֩����",     0x0078, TRIVIAL_RESOURCE, "" },
	{ "�����",     0x0079, TRIVIAL_RESOURCE, "" },
	{ "�׶���",     0x007A, ELITE___RESOURCE, "�ߴ󶾹ƣ��ж���ÿ�غ���Ѫ���������ݡ�" },
	{ "��ȸ��",     0x007B, TRIVIAL_RESOURCE, "�ߴ󶾹ƣ��ж���ÿ�غϺ����������������ݡ�" },
	{ "Ѫ����",     0x007C, TRIVIAL_RESOURCE, "�ߴ󶾹ƣ��ж���ÿ�غ���Ѫ���������ݡ�" },
	{ "�ϳ���",     0x007D, TRIVIAL_RESOURCE, "�ߴ󶾹ƣ��ж���ÿ�غ���Ѫ���������ݡ�" },
	{ "������",     0x007E, TRIVIAL_RESOURCE, "��ҶС�׻���ɢ��Ũ���������ŵ��������������һ�㡣������غ�" },
	{ "���껨",     0x007F, TRIVIAL_RESOURCE, "����ɫС����ɢ�������������ŵ�����������˯���غϡ�" },
	{ "�����",     0x0080, TRIVIAL_RESOURCE, "��ʳ�߻�����þ������Ҳ��֡���ħ���غ�" },
	{ "�����",     0x0081, TRIVIAL_RESOURCE, "�߶��Եĺں�ɫҰ���������ɽ�඾��ʬ�����ζ�����˿��" },
	{ "������",     0x0082, TRIVIAL_RESOURCE, "���ߵĶ��ѡ��ζ�" },
	{ "������",     0x0083, TRIVIAL_RESOURCE, "�۷�������õ��ۡ���������+150" },
	{ "ѩ���",     0x0084, TRIVIAL_RESOURCE, "��������ɽ����֮�أ���ͭǮ���С������������������+2" },
	{ "��Ы��",     0x0085, TRIVIAL_RESOURCE, "����ֻ��βЫ��ĥ�ɵķ�ĩ����ʹ�з�ȫ���г඾��" },
	{ "��ʬˮ",     0x0086, TRIVIAL_RESOURCE, "�����˿�Ѫˮ���㸯ʴ���ã�������մȾ���С�" },
	{ "�Ի���",     0x0087, TRIVIAL_RESOURCE, "��ȼ�ɺ�ҩɢ�����㣬��ʹ���˻�˯��غϡ�" },
	{ "����ɢ",     0x0088, TRIVIAL_RESOURCE, "��ʳǰ�����ж����ɲ������������޷��ⶾ����ʳǰ��û�ж������̱�����" },
	{ "��Ӱ��",     0x0089, ELITE___RESOURCE, "�ߴ󶾹ƣ��ж�����������������һ��������" },
	{ "��ʬ��",     0x008A, TRIVIAL_RESOURCE, "�ߴ󶾹ƣ��ж���Ǳ��Ƭ�̼��ᷢ�������Էǳ����ҡ�" },
	{ "��Ϲ�",     0x008B, TRIVIAL_RESOURCE, "�ߴ󶾹ƣ��ж���ÿ�غ���ʧ��Ǯ���������ݡ�" },
	{ "�ù�",       0x008C, TRIVIAL_RESOURCE, "���ڵĶ�Һ��Ӱ���˵��Բ���ʹ�˵��Ҳ��֡���ħ��غ�" },
	{ "����",       0x008D, TRIVIAL_RESOURCE, "������̼׳棬�����������ƣ�ɢ��֮��������ҷ������μ���ȫ���������غϡ�" },
	{ "���Ϲ�",     0x008E, TRIVIAL_RESOURCE, "��ѩɽ�������������������ĹƳ棬����Ϊ�������ߡ�" },
	{ "��Ϲ�",     0x008F, TRIVIAL_RESOURCE, "�������׶�����������������ĹƳ棬����Ϊ�������ߡ�" },
	{ "ʳ����",     0x0090, TRIVIAL_RESOURCE, "����������ȡ�������ŻغϺ󣬿�������ơ�" },
	{ "���",       0x0091, TRIVIAL_RESOURCE, "��ϡ��ҩ��ι���Ĵƹơ�ȫ������+250" },
	{ "���ҹ�",     0x0092, ELITE___RESOURCE, "Ԥ�Ƚ����������ڹƳ����ڣ���Ͷ�����˲���ǿ�ұ�ը��" },
	{ "��Ѫ��",     0x0093, TRIVIAL_RESOURCE, "����������ȡѪҺ���ŻغϺ󣬿����ɳ�Ѫ�ϡ�" },
	{ "��",         0x0094, TRIVIAL_RESOURCE, "��ʦʩ������Ĳ��ϡ�" },
	{ "��Ѫ��",     0x0095, ELITE___RESOURCE, "��Ѫά���Ķ��ƣ�ʹ��ʳ��������ȫ�ָ�������ȫ��" },
	{ "�����",     0x0096, ELITE___RESOURCE, "����֮������ҹɢ����ɫ�׹⣬��ʳ����������С�����+1" },
	{ "��·��",     0x0097, ELITE___RESOURCE, "������·����У��ɻص��Թ���㡣" },
	{ "���ܳ�",     0x0098, CRITICAL_RESOURE, "�����ƹ���ʦ���Կ���ʬ�壬��ʹ���߼��������Żغϡ�" },
	{ "÷����",     0x0099, TRIVIAL_RESOURCE, "����÷���İ�����" },
	{ "���｣",     0x009A, TRIVIAL_RESOURCE, "�����������еķɽ���" },
	{ "͸�Ƕ�",     0x009B, TRIVIAL_RESOURCE, "�������졢���糤�����룬�Ǻܷ����İ�����" },
	{ "�׻���",     0x009C, TRIVIAL_RESOURCE, "����ҩ�����飬Ͷ��ײ����ᱬ�����ˡ�" },
	{ "����ɰ",     0x009D, TRIVIAL_RESOURCE, "�Ը��߶����Ƴɵ�ϸɰ����ʹ�з�ȫ�����ζ���" },
	{ "������",     0x009E, TRIVIAL_RESOURCE, "������ץ��β��ϵ����Ʋ�˿������ȡ������������ȡ��������100" },
	{ "����˿",     0x009F, TRIVIAL_RESOURCE, "ǧ��֩��Ķ�˿����˿" },
	{ "������",     0x00A0, TRIVIAL_RESOURCE, "ʩ�������Ĵ��������������ֶ������á�������غ�" },
	{ "��Ӱ����",   0x00A1, TRIVIAL_RESOURCE, "ϸ��ţë�����������Ρ�" },
	{ "Ѫ����",     0x00A2, TRIVIAL_RESOURCE, "��ɫ��������װ�з�����Ƭ��" },
	{ "����",       0x00A3, TRIVIAL_RESOURCE, "��ţƤ�Ƶ��߳���ޣ������з�ȫ�塣����+20����+20" },
	{ "�Žر�",     0x00A4, TRIVIAL_RESOURCE, "������������ɵľŽ���ޣ������з�ȫ�塣����+66����+33" },
	{ "���߱�",     0x00A5, ELITE___RESOURCE, "����Ƥ���Խ�˿����֯�ɾų���ޣ������з�ȫ�塣����+99����+60" },
	{ "ľ��",       0x00A6, CRITICAL_RESOURE, "��ľ�ĵ�̵Ľ���С����ߡ�����+2����+3" },
	{ "�̵�",       0x00A7, TRIVIAL_RESOURCE, "һ�߰볤�Ķ۵�������������ľ�ġ�����+6������5" },
	{ "����",       0x00A8, TRIVIAL_RESOURCE, "һ���������������Ľ����������Ϊ���ӡ�����+10������+3" },
	{ "��",       0x00A9, TRIVIAL_RESOURCE, "�����������в��������������ء�����+16������+1" },
	{ "��Ů��",     0x00AA, TRIVIAL_RESOURCE, "һ�߳���˫�ֽ����ʺ�Ů��ʹ�ã��ɷ������ι���������+8������+5" },
	{ "����",       0x00AB, TRIVIAL_RESOURCE, "һ���������ܶ���Ľ������������·���������+25" },
	{ "��ӧ��",     0x00AC, TRIVIAL_RESOURCE, "���ִ��죬�����б����������Ժ�ɫ���롣����+38" },
	{ "ԽŮ��",     0x00AD, TRIVIAL_RESOURCE, "��������ָ��רΪŮ�Ӵ��졣����+22����+18" },
	{ "�䵶",       0x00AE, TRIVIAL_RESOURCE, "����������������֮�����Ͻ�����ȾѪ������+55������+10������+5" },
	{ "������",     0x00AF, TRIVIAL_RESOURCE, "�����ĺ�ɫ���������ɣ����ͷ����������ء�����+70��������15������+9" },
	{ "ܽ�ص�",     0x00B0, TRIVIAL_RESOURCE, "�ٻ��ɶ��ű���˫���䵶���ɷ������ι���������+16����+8" },
	{ "���µ�",     0x00B1, TRIVIAL_RESOURCE, "ϸ������˫����������Ҷ���£��ɷ������ι���������+3������+28������+12" },
	{ "��潣",     0x00B2, TRIVIAL_RESOURCE, "���Ҿ��Ĵ���Ľ����ᱡ����������+75����+26" },
	{ "�絶",       0x00B3, TRIVIAL_RESOURCE, "����սʿ�����õ��嵶������+70����+32" },
	{ "������",     0x00B4, TRIVIAL_RESOURCE, "����֮������������ʳơ���������������+124������+16������+9" },
	{ "˫����",     0x00B5, TRIVIAL_RESOURCE, "��һ�㽣������ͬ��˫�ֽ����ɷ������ι���������+62������+9����+9" },
	{ "��Ů��",     0x00B6, TRIVIAL_RESOURCE, "ԧ��˫���еĴƽ������ͯ��Ϊһ�ԡ�����+100������+15����+20" },
	{ "��ͯ��",     0x00B7, TRIVIAL_RESOURCE, "ԧ��˫���е��۽�������Ů��Ϊһ�ԡ�����+100������+15������+3" },
	{ "��Ȫ��",     0x00B8, TRIVIAL_RESOURCE, "��Ȫ��ˮ�ʷǳ��ʺ��콣�����������Ľ�����Ȫ��������+88����+20" },
	{ "������",     0x00B9, TRIVIAL_RESOURCE, "�絶��һ�֣����⵹�����ֳơ����굶��������+90����+26�����ˣ�9" },
	{ "���ǽ�",     0x00BA, TRIVIAL_RESOURCE, "������Ƕ�߿Ž�Ʊ�ʯ������ȡ��������֮����������+120������+50������+7" },
	{ "��ڤ����",   0x00BB, ELITE___RESOURCE, "�����������з�ȫ�����Σ���˵��ħ���а�������Ч������" },
	{ "������",   0x00BC, ELITE___RESOURCE, "������½����֮����Ч������" },
	{ "������",     0x00BD, TRIVIAL_RESOURCE, "������ʦŷұ����������������������������������+135������+37������+8" },
	{ "̫����",     0x00BE, TRIVIAL_RESOURCE, "��������֮�����佣����ʦ���Խ�����ħ������+158������+90������+35" },
	{ "�޳���",     0x00BF, ELITE___RESOURCE, "�Ϲ��񽣣�ָ����������ص��ѡ�����+200������+20����+77������+33" },
	{ "������",     0x00C0, TRIVIAL_RESOURCE, "���˫�߲��Ƶ������ȡ�����+50������+62������+6" },
	{ "��ͷ��",     0x00C1, TRIVIAL_RESOURCE, "������ʦ�۹�����֮������ͷ­��������ʮ�������ꡣ����+70������+88" },
	{ "ڤ����",     0x00C2, TRIVIAL_RESOURCE, "����ڤ��֮ħ�ȣ�����Ⱥа����Ƕǧ�������ڵ�������+88������+120" },
	{ "������",     0x00C3, ELITE___RESOURCE, "Ů��活�������ɫʯ���÷��ȡ�����+100������+150������+33" },
	{ "ͷ��",       0x00C4, TRIVIAL_RESOURCE, "��ʣ�಼�Ϸ��Ƶ�ͷ������+1" },
	{ "��˿��",     0x00C5, TRIVIAL_RESOURCE, "��ɫ��˿֯����������+2" },
	{ "����",       0x00C6, TRIVIAL_RESOURCE, "���Ƶ�Ů��ͷ�Ρ�����+3" },
	{ "����",       0x00C7, TRIVIAL_RESOURCE, "�����ķ��Ρ�����+5" },
	{ "�������",   0x00C8, TRIVIAL_RESOURCE, "���������Ļƽ��Ρ�����+9" },
	{ "Ƥñ",       0x00C9, TRIVIAL_RESOURCE, "��Ƥ���Ƶ�ñ�ӣ��ǳ���ů������+4" },
	{ "�����",     0x00CA, TRIVIAL_RESOURCE, "�������׺�ĺ�ɫ���ڡ�����+13" },
	{ "��ʦñ",     0x00CB, TRIVIAL_RESOURCE, "��ʿ����ʱ������ñ�ӡ�����+3������+11" },
	{ "�Ͻ��",     0x00CC, TRIVIAL_RESOURCE, "�Ͻ���Ա�ͭƬ���ɣ���������ν���ɡ�����+18" },
	{ "���˿��",   0x00CD, ELITE___RESOURCE, "�Լ��������˿֯�ɣ��ᱡ���͡�����+25����+8" },
	{ "�����ë",   0x00CE, ELITE___RESOURCE, "����˸�������ɫ��ë������+7����+24������+9" },
	{ "�����",     0x00CF, ELITE___RESOURCE, "����������Ļ�ͷ���������˫��β������+3������+28����+5������+3" },
	{ "����",       0x00D0, TRIVIAL_RESOURCE, "�ֲ����ƵĽ��쳤����ۡ�����+3" },
	{ "�ټ�",       0x00D1, TRIVIAL_RESOURCE, "�Ծ��ٱ��ƵĻ��ס�����+7" },
	{ "˿��",       0x00D2, TRIVIAL_RESOURCE, "�Բ�˿��֯���ɣ�����͸��������+3����+4" },
	{ "������",     0x00D3, TRIVIAL_RESOURCE, "�����������ƳɵĻ��ס�����+13������10" },
	{ "ҹ����",     0x00D4, ELITE___RESOURCE, "����ɫ�Ľ����¿�����������ҹɫ֮�С�����+18����+12������+12" },
	{ "��ͭ��",     0x00D5, TRIVIAL_RESOURCE, "��ͭ�Ƶ��������ػ��ס�����+22������13" },
	{ "�޺���",     0x00D6, TRIVIAL_RESOURCE, "���еõ��ĺ������������ۡ�����+10������+10������+10" },
	{ "���ۼ�",     0x00D7, TRIVIAL_RESOURCE, "�������μ�Ƭ��׺���ɵ����ס�����+28������4" },
	{ "��ʦ����",   0x00D8, TRIVIAL_RESOURCE, "��ʦ��������ʱ�����ķ��¡�����+28������+33" },
	{ "����ս��",   0x00D9, TRIVIAL_RESOURCE, "����Բ�εľ���Ƭ��׺���ɣ��������ˣ��ֳơ���������������+40������7" },
	{ "������",     0x00DA, TRIVIAL_RESOURCE, "�Խ��ߴ���Ƭ���ƶ��ɣ��ֳơ��������¡�������+47������10" },
	{ "������",     0x00DB, ELITE___RESOURCE, "�������̵�ͭ�ƿ��ס�����+9������+55������6" },
	{ "��ϱ���",   0x00DC, TRIVIAL_RESOURCE, "�Լ��������˿֯�ɣ��ᱡ���͡�����+66" },
	{ "��������",   0x00DD, ELITE___RESOURCE, "���۱�׺���ɣ��������ս�ס�����+90" },
	{ "�׻�֮��",   0x00DE, TRIVIAL_RESOURCE, "�Ժ����İ׻�Ƥ�Ƴɵ�Ƥ�ס�����+80" },
	{ "����ս��",   0x00DF, ELITE___RESOURCE, "������ĿǼ׶�����ɣ����ʼ���ɫ�ڶ��޹��󡣷���+80" },
	{ "��ȸս��",   0x00E0, ELITE___RESOURCE, "���Ϸ��������ë��֯���ɡ�����+80" },
	{ "����",       0x00E1, TRIVIAL_RESOURCE, "����Խ���������£��׳ơ����񡹡�����+2" },
	{ "����",       0x00E2, TRIVIAL_RESOURCE, "���ڼ���ϵ����ף��ֳơ��ڲ���������+6" },
	{ "��ʿ����",   0x00E3, TRIVIAL_RESOURCE, "��˧�����л�����׵�ս��������+12" },
	{ "���ľ�",     0x00E4, ELITE___RESOURCE, "����ǰ��Ҫ�������ף�����ͭ��������+20" },
	{ "�޺�����",   0x00E5, TRIVIAL_RESOURCE, "�����޺������ë֯�ɵ����硣����+18����+18������+18" },
	{ "��������",   0x00E6, TRIVIAL_RESOURCE, "�ߵ�ɮ�£��������޹��¡�����Ϊ��ɮ�볤������������+16������+31" },
	{ "��������",   0x00E7, TRIVIAL_RESOURCE, "������ǧ��׶��Ƥ�Ƴɣ�ëƤ�ʻ�ɫ������ɫ���ơ�����+40" },
	{ "��������",   0x00E8, ELITE___RESOURCE, "�ഫΪ֯Ů���Ƶ����磬���֯������ʶ�Ŀ������+52" },
	{ "��������",   0x00E9, ELITE___RESOURCE, "������˫������֮���ƣ�����������֮��Ч������+60" },
	{ "ʥ������",   0x00EA, CRITICAL_RESOURE, "�׺�����Ǳ����ʥ������������+30������+66" },
	{ "��Ь",       0x00EB, TRIVIAL_RESOURCE, "�����ݱ�֯���ɣ�ʮ�ֱ��ˣ�����������㣬�������ߡ�����+1" },
	{ "ľЬ",       0x00EC, TRIVIAL_RESOURCE, "��ľ�����ƶ��ɣ�Ь����м���ͼ��������+2" },
	{ "��ѥ",       0x00ED, CRITICAL_RESOURE, "�ֲ����Ƶĳ�ͳѥ������+3����+2" },
	{ "�廨Ь",     0x00EE, TRIVIAL_RESOURCE, "��˿�з��ƣ�Ь��������ͷ��β��������+4" },
	{ "����",       0x00EF, TRIVIAL_RESOURCE, "Ь�׼з���Ƭ������ͨ��ѥ�ء�����+6" },
	{ "��ɮѥ",     0x00F0, TRIVIAL_RESOURCE, "�޺�ɮ���������Ĳ�ѥ������+8����+6" },
	{ "¹Ƥѥ",     0x00F1, TRIVIAL_RESOURCE, "Ь����¹Ƥë���ƣ��ʵ����ᣬ�ж�����¹��Ѹ�ݡ�����+11����+9" },
	{ "����ѥ",     0x00F2, TRIVIAL_RESOURCE, "�Ա�������Ĳ�ɴ֯�ɣ������߼�����硣����+14����+17" },
	{ "����ѥ",     0x00F3, TRIVIAL_RESOURCE, "���Խ����ĳ�Ͳ�廨Ь������+18����+5" },
	{ "��Ƥѥ",     0x00F4, TRIVIAL_RESOURCE, "ȡ�Զ�������Ƥë���ơ�����+21����+12" },
	{ "����ѥ",     0x00F5, TRIVIAL_RESOURCE, "�����۱�׺���ɡ�����+25����+16" },
	{ "����ѥ",     0x00F6, ELITE___RESOURCE, "�������𻯵��ɺ�������֮��ѥ������+28����+20" },
	{ "��Ӱ��ѥ",   0x00F7, ELITE___RESOURCE, "��ħ���壬������ȡ�����+32����+26" },
	{ "���",       0x00F8, TRIVIAL_RESOURCE, "���ľм����۵�С������������װ�μ��а����Ʒ������+8������+18" },
	{ "����",       0x00F9, TRIVIAL_RESOURCE, "�ֲ�����֮�󲿻��ס�����+2" },
	{ "������",     0x00FA, TRIVIAL_RESOURCE, "���ִ���֮�󲿻���������+5" },
	{ "���",       0x00FB, TRIVIAL_RESOURCE, "��������֮�߿׺�ѡ�����+18" },
	{ "����",       0x00FC, TRIVIAL_RESOURCE, "������������״�������װ��Ʒ������+20" },
	{ "����",       0x00FD, TRIVIAL_RESOURCE, "�����ֱ��ϵ����ƻ������Ρ�����+5������+9" },
	{ "����",       0x00FE, TRIVIAL_RESOURCE, "���ͽ������䣬�����Ŵ����ļ����顣����+5������+5" },
	{ "����",       0x00FF, TRIVIAL_RESOURCE, "��������⣬��ʹ���������ǣ��ɽⶨ����˯����ħ��" },
	{ "ͭ��",       0x0100, TRIVIAL_RESOURCE, "��ͭ����������þߡ�����+6" },
	{ "���Ծ�",     0x0101, TRIVIAL_RESOURCE, "����ɰ�ھ��滭���ԣ��ɽ�����Ȼ�������������+8������+8" },
	{ "Ǭ����",     0x0102, ELITE___RESOURCE, "ͭ����������̫��Ǭ��ͼ������ȡ�����������������+14������+14" },
	{ "�����ֻ�",   0x0103, ELITE___RESOURCE, "�ռ��������������ɵ��ֻ�������+9" },
	{ "ʥ����",     0x0104, CRITICAL_RESOURE, "Ů�ĩ���洫�������ʥ�����֮��������+128������+15������ʱ��" },
	{ "�����",     0x0105, TRIVIAL_RESOURCE, "���޽�������ǧ����ڵ�������+90" },
	{ "�嶾��",     0x0106, CRITICAL_RESOURE, "�ɾ��ֵ��ڵ��������ٶ����֡�" },
	{ "������",     0x0107, CRITICAL_RESOURE, "Ů洽�������󣬽��Ʒ������ڵı��顣����ʱ��" },
	{ "������",     0x0108, CRITICAL_RESOURE, "Ů洽�������󣬽����������ڵı��顣����ʱ��" },
	{ "ˮ����",     0x0109, CRITICAL_RESOURE, "Ů洽���ѩ���󣬽���ѩ�����ڵı��顣����ʱ��" },
	{ "������",     0x010A, CRITICAL_RESOURE, "Ů洽�������󣬽��ƻ������ڵı��顣����ʱ��" },
	{ "������",     0x010B, CRITICAL_RESOURE, "Ů洽���ɽ��󣬽���ɽ�����ڵı��顣����ʱ��" },
	{ "������",     0x010C, TRIVIAL_RESOURCE, "�ɽ��������ɸ��ֹơ���ɽ���ʽ���������������ơ�" },
	{ "�ٺ�«",     0x010D, TRIVIAL_RESOURCE, "ս����ӿ��������������ߣ��������ָ�֮��Ч��" },
	{ "�Ͻ��«",   0x010E, CRITICAL_RESOURE, "���������������������ϡ�������ʬ�����ϵõ��Ĳ������������ӿɵõ�����" },
	{ "����",       0x010F, CRITICAL_RESOURE, "�������̵����" },
	{ "�𻨾�",     0x0110, CRITICAL_RESOURE, "����ң�ҵĿ�ջ���ľơ�" },
	{ "�Ͻ�",     0x0111, CRITICAL_RESOURE, "�������������ҩ��" },
	{ "�����",     0x0112, CRITICAL_RESOURE, "��������̳ǰ�����飬����Ѭ�ձ仯ͨ�顣����+88������+18" },
	{ "���˵���", 0x0113, CRITICAL_RESOURE, "ҩ��" },
	{ "���������", 0x0114, CRITICAL_RESOURE, "ҩ��" },
	{ "������Ѫ��", 0x0115, CRITICAL_RESOURE, "��˵�н�������ӡ�����еı���" },
	{ "������",     0x0116, TRIVIAL_RESOURCE, "ǧ�궾�Եĵ����Զ������ɽ��������еĶ�����û�ж��Զ������������" },
	{ "�����",     0x0117, CRITICAL_RESOURE, "�����������鵺ʯ��ķ�����" },
	{ "����",       0x0118, CRITICAL_RESOURE, "��������ң��ʰ�����" },
	{ "���ӹ�",     0x0119, CRITICAL_RESOURE, "ҩ��" },
	{ "����",       0x011A, CRITICAL_RESOURE, "ҩ��" },
	{ "¹��",       0x011B, CRITICAL_RESOURE, "ҩ��" },
	{ "����",       0x011C, CRITICAL_RESOURE, "�׺Ӵ����ĵ��ͣ�����ǵ�Ҫ�����˼ң�" },
	{ "���޼�",     0x011D, CRITICAL_RESOURE, "�Ի����õĲ�¹���塣" },
	{ "����",     0x011E, CRITICAL_RESOURE, "����ҩ�̵��洫��ҩ��" },
	{ "����",       0x011F, CRITICAL_RESOURE, "ʿ��ί�е����顣" },
	{ "����",       0x0120, CRITICAL_RESOURE, "�Ůί�е����塣" },
	{ "ʯԿ��",     0x0121, CRITICAL_RESOURE, "���������ߺ�ʯ�ŵ�Կ�ס�" },
	{ "����",       0x0122, CRITICAL_RESOURE, "�����ɸ��������С�" },
	{ "�㽶",       0x0123, CRITICAL_RESOURE, "������ľ��ȡ��˵�����Ҫ�����ӣ���ʥ�ü���ǰ�����Ͽ���ժ��" },
	{ "�����־�",   0x0124, CRITICAL_RESOURE, "���ѽ��������" },
	{ "�־�",       0x0125, CRITICAL_RESOURE, "�书���š�" },
	{ "«έƯ",     0x0126, TRIVIAL_RESOURCE, "������Ư��ˮ��Ĳ�ϯ�� " }
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
		//��Ʒ����
		Item.mask = GVIF_TEXT;
		Item.row  = row;
		Item.col  = 0;
        str.Format(_T("%s"), AllResource[row - 1].m_ResName);
        Item.strText = str;
	   	m_Grid.SetItem(&Item);
		m_Grid.SetItemBkColour(row, 0, RGB(128, 255, 128));	

		//�Ƿ���
		m_Grid.SetCellType(row, 1, RUNTIME_CLASS(CGridCellCheck));

		//����
		m_Grid.SetCellType(row, 2, RUNTIME_CLASS(CGridCellNumeric));

		//��Ʒ���
		s.Format(_T("0x%04lX"), AllResource[row - 1].m_ResID);
		m_Grid.SetItemText(row, 3, s);
		m_Grid.GetCell(row, 3)->SetState(GVIS_READONLY);

		//�Ƿ�������
		m_Grid.SetCellType(row, 4, RUNTIME_CLASS(CGridCellCheck));
		CGridCellCheck *pCellCheck = (CGridCellCheck *)m_Grid.GetCell(row, 4);
		pCellCheck->SetCheck(AllResource[row - 1].m_ResType == CRITICAL_RESOURE? TRUE:FALSE);
		m_Grid.GetCell(row, 4)->SetState(GVIS_READONLY);

		//��Ʒ����
		m_Grid.SetItemText(row, 5, AllResource[row - 1].m_ResDesc);
		m_Grid.GetCell(row, 5)->SetState(GVIS_READONLY);
	}
	
	//������
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
		//�Ƿ�ӵ��
		pCellCheck = (CGridCellCheck *)m_Grid.GetCell(row, 1);
		pCellCheck->SetCheck(FALSE);

		//����
		m_Grid.SetItemText(row, 2, "");
	}

	m_TotalResource = 0;

	struct Resource  *pResInfo = g_SaveInfo.m_Resource;
	while(pResInfo->m_ResID && pResInfo->m_ResCount)
	{
		row = pResInfo->m_ResID - ResStartID + 1;                 //��Ʒ��Ŷ�Ӧ���к�
		if (row + 1  > m_Grid.GetRowCount()) continue;            //Խ��
		
		//�Ƿ�ӵ�и���Ʒ
		pCellCheck = (CGridCellCheck *)m_Grid.GetCell(row, 1);
		pCellCheck->SetCheck();

		//����Ʒ������
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
		//����������ص���Ʒ
		if (AllResource[row - 1].m_ResType == CRITICAL_RESOURE)
			continue;

		//�Ƿ�ӵ�и���Ʒ
		BOOL Checked = ((AllResource[row - 1].m_ResType == ELITE___RESOURCE) ? TRUE: FALSE);
		CGridCellCheck *pCellCheck = (CGridCellCheck *)m_Grid.GetCell(row, 1);
		pCellCheck->SetCheck(Checked);

		//����Ʒ������
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
		//�Ƿ�ӵ�и���Ʒ
		CGridCellCheck *pCellCheck = (CGridCellCheck *)m_Grid.GetCell(row, 1);
		pCellCheck->SetCheck();

		//����Ʒ������
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
		if (pCellCheck->GetCheck())                                   //�Ƿ�ӵ��
		{
			pResInfo->m_ResID    = ResStartID + row - 1;
			pResInfo->m_ResCount = atol(m_Grid.GetItemText(row, 2));  //����
			pResInfo++;
		}
	}
}
