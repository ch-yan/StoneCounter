
// StoneCounterDlg.h: 头文件
//

#pragma once


// CStoneCounterDlg 对话框
class CStoneCounterDlg : public CDialogEx
{
// 构造
public:
	CStoneCounterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STONECOUNTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOpenFile();
	afx_msg void OnBnClickedCluster();
	DECLARE_MESSAGE_MAP()

// PCL
private:
	//CStatic m_staicPCLView;
	int m_nLeftViewport, m_nRightViewport;
	boost::shared_ptr<pcl::visualization::PCLVisualizer> m_viewer; //要共享指针类型的，否则显示窗口会跳出MFC界面

	vtkRenderWindow* m_win;   //vtk渲染的窗口句柄
	vtkRenderWindowInteractor* m_iren; //vtk交互的对象
	pcl::PointCloud<pcl::PointXYZ>::Ptr m_curCloud;
	void SetCamera(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, const char* name);

	void output(CString info);

public:
	CEdit m_editOutput;
	//pcl::PointCloud<pcl::PointXYZ>::Ptr m_cloud;
	//pcl::visualization::PCLVisualizer viewer;
};
