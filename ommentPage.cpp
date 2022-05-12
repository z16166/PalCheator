// ommentPage.cpp : implementation file
//

#include "stdafx.h"
#include "PalCheator.h"
#include "ommentPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CommentPage dialog


CommentPage::CommentPage(CWnd* pParent /*=NULL*/)
	: CDialog(CommentPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CommentPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CommentPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CommentPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_GridControl(pDX, IDC_COMMENT_GRID, m_Grid);
	//}}AFX_DATA_MAP
}

void CommentPage::OnOK()
{

}

void CommentPage::OnCancel()
{
}

BEGIN_MESSAGE_MAP(CommentPage, CDialog)
	//{{AFX_MSG_MAP(CommentPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CommentPage message handlers
static const char *ColName[] =
{
	"文件偏移量", "描述信息"
};

struct FileFieldDesc
{
	int  m_FileOffset;
	char *m_FieldDesc;
};

static const struct FileFieldDesc DescArray[] =
{
	{ 0x00,  "存盘次数，2字节" },
	{ 0x28,  "金钱，4字节" },
	{ 0x7C,  "经验值和初始等级，每人8字节，共5人（李、赵、林、巫后、阿奴），8字节 = 4字节经验 + 4字节初始等级"},
	{ 0x244, "修行等级，每人2字节，共5人，最大值99"},
	{ 0x250, "体力上限，每人2字节，共5人，最大值999"},
	{ 0x25C, "真气上限，每人2字节，共5人，最大值999"},
	{ 0x268, "体力，每人2字节，共5人"},
	{ 0x274, "真气，每人2字节，共5人"},
	{ 0x2C8, "武术，每人2字节，共5人，最大值999"},
	{ 0x2D4, "灵力，每人2字节，共5人，可以超过999，但和装备属性叠加后不要超过32767"},
	{ 0x2E0, "防御，每人2字节，共5人，可以超过999，但和装备属性叠加后不要超过32767"},
	{ 0x2EC, "身法，每人2字节，共5人，可以超过999，但和装备属性叠加后不要超过32767"},
	{ 0x2F8, "吉运，每人2字节，共5人，可以超过999，但和装备属性叠加后不要超过32767"},
	{ 0x37E, "仙术，每种2字节的编号，27 01是梦蛇，28 01是气疗术等，细节未明"},
	{ 0x6C0, "物品，每种物品6个字节 = 2字节的物品编号 +  4字节的数量（上限99）。全满是234种"},
};

BOOL CommentPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_Grid.SetVirtualMode(FALSE);
    m_Grid.SetEditable(FALSE);
	m_Grid.SetHeaderSort(FALSE);
	m_Grid.SetAutoSizeStyle();
	m_Grid.SetRowResize(TRUE);

    int m_nFixCols = 1;
	int m_nFixRows = 1;
	int m_nCols    = sizeof(ColName)/sizeof(ColName[0]);
	int m_nRows    = sizeof(DescArray)/sizeof(DescArray[0]) + 1;

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

	for (int row = 0; row < m_Grid.GetRowCount(); row++)
    {
	    for (int col = 0; col < m_Grid.GetColumnCount(); col++)
		{ 
            CString str;

			GV_ITEM Item;

    		Item.mask = GVIF_TEXT;
	    	Item.row  = row;
		    Item.col  = col;

			if (row < m_nFixRows)
			{
                str.Format(_T("%s"), ColName[col]);
				m_Grid.SetItemBkColour(row, col, RGB(255, 255, 0));
			}
            else if (col < m_nFixCols) 
			{
                str.Format(_T("0x%03lX"), DescArray[row - 1].m_FileOffset);
				m_Grid.SetItemBkColour(row, col, RGB(128, 255, 128));
			}
            else 
			{
				str.Format(_T("%s"), DescArray[row - 1].m_FieldDesc);
			}
            Item.strText = str;


        	m_Grid.SetItem(&Item);
	    }
    }

	m_Grid.ExpandLastColumn();
	m_Grid.AutoSizeRows();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
