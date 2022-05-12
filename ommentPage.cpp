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
	"�ļ�ƫ����", "������Ϣ"
};

struct FileFieldDesc
{
	int  m_FileOffset;
	char *m_FieldDesc;
};

static const struct FileFieldDesc DescArray[] =
{
	{ 0x00,  "���̴�����2�ֽ�" },
	{ 0x28,  "��Ǯ��4�ֽ�" },
	{ 0x7C,  "����ֵ�ͳ�ʼ�ȼ���ÿ��8�ֽڣ���5�ˣ���ԡ��֡��׺󡢰�ū����8�ֽ� = 4�ֽھ��� + 4�ֽڳ�ʼ�ȼ�"},
	{ 0x244, "���еȼ���ÿ��2�ֽڣ���5�ˣ����ֵ99"},
	{ 0x250, "�������ޣ�ÿ��2�ֽڣ���5�ˣ����ֵ999"},
	{ 0x25C, "�������ޣ�ÿ��2�ֽڣ���5�ˣ����ֵ999"},
	{ 0x268, "������ÿ��2�ֽڣ���5��"},
	{ 0x274, "������ÿ��2�ֽڣ���5��"},
	{ 0x2C8, "������ÿ��2�ֽڣ���5�ˣ����ֵ999"},
	{ 0x2D4, "������ÿ��2�ֽڣ���5�ˣ����Գ���999������װ�����Ե��Ӻ�Ҫ����32767"},
	{ 0x2E0, "������ÿ��2�ֽڣ���5�ˣ����Գ���999������װ�����Ե��Ӻ�Ҫ����32767"},
	{ 0x2EC, "����ÿ��2�ֽڣ���5�ˣ����Գ���999������װ�����Ե��Ӻ�Ҫ����32767"},
	{ 0x2F8, "���ˣ�ÿ��2�ֽڣ���5�ˣ����Գ���999������װ�����Ե��Ӻ�Ҫ����32767"},
	{ 0x37E, "������ÿ��2�ֽڵı�ţ�27 01�����ߣ�28 01���������ȣ�ϸ��δ��"},
	{ 0x6C0, "��Ʒ��ÿ����Ʒ6���ֽ� = 2�ֽڵ���Ʒ��� +  4�ֽڵ�����������99����ȫ����234��"},
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
