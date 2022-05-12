// PlayerPage.cpp : implementation file
//

#include "stdafx.h"
#include "PalCheator.h"
#include "PlayerPage.h"
#include "GridCellNumeric.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern GameSaveInfo g_SaveInfo;
/////////////////////////////////////////////////////////////////////////////
// PlayerPage dialog


PlayerPage::PlayerPage(CWnd* pParent /*=NULL*/)
	: CDialog(PlayerPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(PlayerPage)
	m_Money = 0;
	//}}AFX_DATA_INIT
}


void PlayerPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PlayerPage)
	DDX_Text(pDX, IDC_EDIT_MONEY, m_Money);
	DDV_MinMaxLong(pDX, m_Money, 0, 2147483647);
	DDX_GridControl(pDX, IDC_PLAYER_GRID, m_Grid);
	//}}AFX_DATA_MAP
}

void PlayerPage::OnOK( )
{
}

void PlayerPage::OnCancel( )
{
}

BEGIN_MESSAGE_MAP(PlayerPage, CDialog)
	//{{AFX_MSG_MAP(PlayerPage)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, OnButtonRestore)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PlayerPage message handlers

static const char *ColName[] =
{
	"", "李逍遥", "赵灵儿", "林月如", "巫后", "阿奴"
};

static const char *RowName[] =
{
	"经验",
	"修行",
	"体力上限",
	"真气上限",
	"体力",
	"真气",
	"武术",
	"灵力",
	"防御",
	"身法",
	"吉运"
};

BOOL PlayerPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	m_Grid.SetVirtualMode(FALSE);
    m_Grid.SetEditable(TRUE);
	m_Grid.SetRowResize(TRUE);
	m_Grid.SetHeaderSort(FALSE);

    int m_nFixCols = 1;
	int m_nFixRows = 1;
	int m_nCols    = sizeof(ColName)/sizeof(ColName[0]);
	int m_nRows    = sizeof(RowName)/sizeof(RowName[0]) + 1;

    m_Grid.SetAutoSizeStyle();

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
                str.Format(_T("%s"), RowName[row - 1]);
				m_Grid.SetItemBkColour(row, col, RGB(128, 255, 128));
			}
            else 
			{
				str.Format(_T("%s"), "");
				m_Grid.SetCellType(row, col, RUNTIME_CLASS(CGridCellNumeric));
			}
            Item.strText = str;
			
        	m_Grid.SetItem(&Item);
	    }
    }

	m_Grid.ExpandLastColumn();
	m_Grid.AutoSizeRows();

	MemoryDataToDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void PlayerPage::MemoryDataToDialog()
{	
	m_Money = g_SaveInfo.m_Money;

	CString s;
	int k;
	
	for(k = 0; k < NUM_PLAYERS; k++)
	{
		int row = 1;

		//经验
		s.Format(_T("%d"), g_SaveInfo.m_EXP[k].m_RealEXP);
		m_Grid.SetItemText(row++, k+1, s);

		//修行
		s.Format(_T("%d"), g_SaveInfo.m_Level[k]);
		m_Grid.SetItemText(row++, k+1, s);

		//体力上限
		s.Format(_T("%d"), g_SaveInfo.m_MaxHP[k]);
		m_Grid.SetItemText(row++, k+1, s);

		//真气上限
		s.Format(_T("%d"), g_SaveInfo.m_MaxMP[k]);
		m_Grid.SetItemText(row++, k+1, s);

		//体力
		s.Format(_T("%d"), g_SaveInfo.m_HP[k]);
		m_Grid.SetItemText(row++, k+1, s);

		//真气
		s.Format(_T("%d"), g_SaveInfo.m_MP[k]);
		m_Grid.SetItemText(row++, k+1, s);

		//武术
		s.Format(_T("%d"), g_SaveInfo.m_Attack[k]);
		m_Grid.SetItemText(row++, k+1, s);

		//灵力
		s.Format(_T("%d"), g_SaveInfo.m_Spirit[k]);
		m_Grid.SetItemText(row++, k+1, s);

		//防御
		s.Format(_T("%d"), g_SaveInfo.m_Defence[k]);
		m_Grid.SetItemText(row++, k+1, s);

		//身法
		s.Format(_T("%d"), g_SaveInfo.m_Dexterity[k]);
		m_Grid.SetItemText(row++, k+1, s);

		//吉运
		s.Format(_T("%d"), g_SaveInfo.m_Luck[k]);
		m_Grid.SetItemText(row++, k+1, s);
	}

	m_Grid.Invalidate();
	UpdateData(FALSE);
}

void PlayerPage::OnButtonRecommend() 
{
	// TODO: Add your control notification handler code here
	m_Money = 999999;

	int k;
	
	for(k = 0; k < NUM_PLAYERS; k++)
	{
		int row = 1;

		//经验
		m_Grid.SetItemText(row++, k+1, "99999");

		//修行
		m_Grid.SetItemText(row++, k+1, "99");

		//体力上限
		m_Grid.SetItemText(row++, k+1, "999");

		//真气上限
		m_Grid.SetItemText(row++, k+1, "999");

		//体力
		m_Grid.SetItemText(row++, k+1, "999");

		//真气
		m_Grid.SetItemText(row++, k+1, "999");

		//武术
		m_Grid.SetItemText(row++, k+1, "999");

		//灵力
		m_Grid.SetItemText(row++, k+1, "16000");

		//防御
		m_Grid.SetItemText(row++, k+1, "16000");

		//身法
		m_Grid.SetItemText(row++, k+1, "16000");

		//吉运
		m_Grid.SetItemText(row++, k+1, "16000");
	}

	m_Grid.Invalidate();
	UpdateData(FALSE);
}

void PlayerPage::OnButtonRestore() 
{
	// TODO: Add your control notification handler code here
	MemoryDataToDialog();
}

void PlayerPage::DialogDataToMemory()
{
	UpdateData(TRUE);

	g_SaveInfo.m_Money = m_Money;

	CString s;
	int k;
	
	for(k = 0; k < NUM_PLAYERS; k++)
	{
		int row = 1;

		//经验
		g_SaveInfo.m_EXP[k].m_RealEXP = atol(m_Grid.GetItemText(row++, k+1));
		
		//修行
		g_SaveInfo.m_Level[k] = (short)atol(m_Grid.GetItemText(row++, k+1));

		//体力上限
		g_SaveInfo.m_MaxHP[k] = (short)atol(m_Grid.GetItemText(row++, k+1));

		//真气上限
		g_SaveInfo.m_MaxMP[k] = (short)atol(m_Grid.GetItemText(row++, k+1));

		//体力
		g_SaveInfo.m_HP[k] = (short)atol(m_Grid.GetItemText(row++, k+1));

		//真气
		g_SaveInfo.m_MP[k]  = (short)atol(m_Grid.GetItemText(row++, k+1));

		//武术
		g_SaveInfo.m_Attack[k] = (short)atol(m_Grid.GetItemText(row++, k+1));

		//灵力
		g_SaveInfo.m_Spirit[k] = (short)atol(m_Grid.GetItemText(row++, k+1));

		//防御
		g_SaveInfo.m_Defence[k] = (short)atol(m_Grid.GetItemText(row++, k+1));

		//身法
		g_SaveInfo.m_Dexterity[k] = (short)atol(m_Grid.GetItemText(row++, k+1));

		//吉运
		g_SaveInfo.m_Luck[k] = (short)atol(m_Grid.GetItemText(row++, k+1));		
	}
}
