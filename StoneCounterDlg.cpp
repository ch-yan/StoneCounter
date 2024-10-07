
// StoneCounterDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "StoneCounter.h"
#include "StoneCounterDlg.h"
#include "afxdialogex.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CStoneCounterDlg 对话框



CStoneCounterDlg::CStoneCounterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STONECOUNTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStoneCounterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_edit_output);
}

BEGIN_MESSAGE_MAP(CStoneCounterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FILE, &CStoneCounterDlg::OnBnClickedOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_CLUSTER, &CStoneCounterDlg::OnBnClickedCluster)
END_MESSAGE_MAP()


// CStoneCounterDlg 消息处理程序

BOOL CStoneCounterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//pcl显示窗口初始化
	m_viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));//初始化viewer对象
	//m_viewer->addCoordinateSystem(); //设置对应的坐标系
	m_viewer->setBackgroundColor(0, 0, 0);//设置背景颜色
	m_viewer->initCameraParameters();//初始化相机的参数
	//m_viewer->addCoordinateSystem(1.0, "global");
	m_win = m_viewer->getRenderWindow();//将view中的渲染窗口的句柄传递给vtk window
	m_iren = vtkRenderWindowInteractor::New(); //初始化vtkwindow交互的对象 
	m_viewer->resetCamera();//使点云显示在屏幕中间，并绕中心操作
	CRect rect;
	GetClientRect(&rect);//实时获取MFC窗口大小
	m_win->SetSize(rect.right - rect.left, rect.bottom - rect.top);//根据当前窗口的大小设置vtk 窗口的大小
	CWnd* viewer_pcWnd;
	viewer_pcWnd = this->GetDlgItem(IDC_VTK1);//获取对应的wnd
	m_win->SetParentId(viewer_pcWnd->m_hWnd);//设置vtk窗口的句柄
	m_iren->SetRenderWindow(m_win);//将vtk交互对象与vtk window绑定 
	m_viewer->createInteractor();
	m_win->Render();//开始渲染

	//设置pclviewer窗口
	m_viewer_viewport1 = 1;
	m_viewer_viewport2 = 2;
	m_viewer->createViewPort(0, 0, 0.5, 1, m_viewer_viewport1);
	m_viewer->createViewPort(0.5, 0, 1, 1, m_viewer_viewport2);
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CStoneCounterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CStoneCounterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CStoneCounterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStoneCounterDlg::OnBnClickedOpenFile() 
{
	CString strFile = _T("");
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.pcd)|*.pcd|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
		//Cstring 转string
		CString theCStr;
		std::string STDStr(CW2A(strFile.GetString()));
		pcl::PointCloud<pcl::PointXYZ>::Ptr 
			cloud(new pcl::PointCloud<pcl::PointXYZ>);
		if (pcl::io::loadPCDFile<pcl::PointXYZ>(STDStr, *cloud) == -1)//*打开点云文件
		{
			AfxMessageBox(_T("读入点云数据失败"));
		}
		cout << cloud->points.size() << endl;
		m_viewer->removeAllPointClouds();//将前一次点云移除  

		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(cloud, 0, 255, 0);

		m_viewer->addPointCloud<pcl::PointXYZ >(cloud, single_color, "org_cloud", m_viewer_viewport1);
		setCamera(cloud, "org_cloud");
		
		m_viewer->spinOnce(5000);
	}
}

void CStoneCounterDlg::OnBnClickedCluster()
{
	m_edit_output.SetWindowTextW(L"begin clicked");
	// 创建KdTreee对象作为搜索方法
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
	tree->setInputCloud(cloud);

	std::vector<pcl::PointIndices> cluster_indices;

	clock_t start = clock();
	//求法线
	std::cout << "start computing normals ..." << std::endl;
	pcl::PointCloud <pcl::Normal>::Ptr normals(new pcl::PointCloud <pcl::Normal>);
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normal_estimator;
	normal_estimator.setSearchMethod(tree);
	normal_estimator.setInputCloud(cloud);
	normal_estimator.setKSearch(50);
	normal_estimator.compute(*normals);

	std::cout << "computing normals cost" << (double)(clock() - start) / CLOCKS_PER_SEC << std::endl;

	//区域生长分割器
	start = clock();
	pcl::RegionGrowing<pcl::PointXYZ, pcl::Normal> reg;

	//输入分割目标
	reg.setSearchMethod(tree);
	reg.setNumberOfNeighbours(30);
	reg.setInputCloud(cloud);
	//reg.setIndices (indices);
	reg.setInputNormals(normals);

	//设置限制条件及先验知识
	reg.setMinClusterSize(2000);
	reg.setMaxClusterSize(1000000);
	reg.setSmoothnessThreshold(30.0 / 180.0 * M_PI);
	reg.setCurvatureThreshold(1.0);

	reg.extract(cluster_indices);

	std::cout << "cluster operation cost: " << (double)(clock() - start) / CLOCKS_PER_SEC << "s, cluster size: " << cluster_indices.size() << endl;
	//遍历抽取结果，将其显示并保存
	int j = 0;
	for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin(); it != cluster_indices.end(); ++it)
	{
		cout << j << ":" << it->indices.size() << endl;

		//创建临时保存点云族的点云
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);
		//////通过下标，逐个填充
		for (std::vector<int>::const_iterator pit = it->indices.begin(); pit != it->indices.end(); pit++)
			cloud_cluster->points.push_back(cloud->points[*pit]); //*

		//////设置点云属性
		cloud_cluster->width = cloud_cluster->points.size();
		cloud_cluster->height = 1;
		cloud_cluster->is_dense = true;

		////std::cout << "当前聚类 " << j << "包含的点云数量: " << cloud_cluster->points.size() << "data points." << std::endl;
		std::stringstream ss;
		ss << "cloud_cluster_" << j << ".pcd";
		j++;
		//writer.write<pcl::PointXYZ>(ss.str(), *cloud_cluster, false); //*

		//显示,随机设置不同颜色，以区分不同的聚类
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> cluster_color(cloud_cluster, rand() * 100 + j * 80, rand() * 50 + j * 90, rand() * 200 + j * 100);
		viewer.addPointCloud(cloud_cluster, cluster_color, ss.str(), 2);
		viewer.spinOnce(5000);

	}
}



void CStoneCounterDlg::SetCamera(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, const char* name
{
	// TODO: 在此处添加实现代码.
	pcl::PointXYZ min_pt, max_pt;
	pcl::getMinMax3D(*cloud, min_pt, max_pt);

	Eigen::Vector3f diff = max_pt.getVector3fMap() - min_pt.getVector3fMap();
	float distance = diff.norm();
	Eigen::Vector3f center((max_pt.x + min_pt.x) / 2, (max_pt.y + min_pt.y) / 2, (max_pt.z + min_pt.z) / 2);

	m_viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "org_cloud");

	m_viewer->setCameraPosition(center[0], center[1], center[2] + distance, center[0], center[1], center[2], 0, -1, 0);

}
