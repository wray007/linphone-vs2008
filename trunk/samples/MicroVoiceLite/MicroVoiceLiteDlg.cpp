
// MicroVoiceLiteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MicroVoiceLite.h"
#include "MicroVoiceLiteDlg.h"
#include "linphonecore.h"
#include "private.h"

#define PHONE_CORE_TIMER 100

#ifdef __cplusplus
extern "C" {
#endif

static LinphoneCoreVTable vTable;
static const char *config_file = "libsipua2.ini";
static const char *factoryConfig = "libsipua2.ini";
static LinphoneCore *the_core=NULL;


static void linphone_log_handler(OrtpLogLevel lev, const char *fmt, va_list args){
	const char *lname="undef";
	char *msg;
	char *msg_str;

	switch(lev){
			case ORTP_DEBUG:
				lname="DEBUG";
				break;
			case ORTP_MESSAGE:
				lname="MESSAGE";
				break;
			case ORTP_WARNING:
				lname="WARING";
				break;
			case ORTP_ERROR:
				lname="ERROR";
				break;
			case ORTP_FATAL:
				lname="FATAL";
				break;
			default:
				lname = ("Bad level !");
	}


	msg=ortp_strdup_vprintf(fmt,args);


	msg_str = ortp_strdup_printf("%s: %s\r\n",lname,msg);

#ifdef _MSC_VER
	OutputDebugString(msg_str);
#endif

	ms_free(msg);
	ms_free(msg_str);
}

static void showInterfaceCb(LinphoneCore *lc) {
	CMicroVoiceLiteDlg *the_ui = (CMicroVoiceLiteDlg *)linphone_core_get_user_data(lc);

	if(the_ui)
		the_ui->ShowWindow(SW_SHOW);
}
static  void inviteReceivedCb(LinphoneCore *lc, const char *from) {
	CMicroVoiceLiteDlg *the_ui = (CMicroVoiceLiteDlg *)linphone_core_get_user_data(lc);

	if(the_ui)
		the_ui->mStatus.SetWindowText(CString(from));

	CString tmp;

	if (the_ui)
	{
		tmp.Format("新来电 (%s)",from);

		if(IDYES   ==   AfxMessageBox(tmp+"\n \t\t\t是否接听?",MB_YESNO|MB_DEFBUTTON2))
			linphone_core_accept_call(the_core,NULL);
		else
		{
			linphone_core_terminate_call(the_core,NULL);
		}
	}
}
static void byeReceivedCb(LinphoneCore *lc, const char *from) {

}
static void displayStatusCb(LinphoneCore *lc, const char *message) {
	CMicroVoiceLiteDlg *the_ui = (CMicroVoiceLiteDlg *)linphone_core_get_user_data(lc);
#ifdef _MSC_VER
	OutputDebugString(message);
#endif
	if(the_ui)
		the_ui->mStatus.SetWindowText(CString(message));
}
static void displayMessageCb(LinphoneCore *lc, const char *message) {

}
static void authInfoRequested(LinphoneCore *lc, const char *realm, const char *username) {

}
static void generalStateChange(LinphoneCore *lc, LinphoneGeneralState *gstate) {

}

static void network_quality(LinphoneCore *lc, int ms)
{
	CMicroVoiceLiteDlg *the_ui = (CMicroVoiceLiteDlg *)linphone_core_get_user_data(lc);
	CString tmp;

	tmp.Format("网络延时 %d ms",ms);
	if(the_ui)
		the_ui->mStatus.SetWindowText(tmp);
}

static void media_quality(LinphoneCore *lc, int jitter, float packet_lost)
{
	CMicroVoiceLiteDlg *the_ui = (CMicroVoiceLiteDlg *)linphone_core_get_user_data(lc);
	CString tmp;

	tmp.Format("包间平均抖动 %d ms, 丢包率 %2.2f%%",jitter,packet_lost);
	if(the_ui)
		the_ui->mStatus.SetWindowText(tmp);
}

#ifdef __cplusplus
}
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMicroVoiceLiteDlg 对话框




CMicroVoiceLiteDlg::CMicroVoiceLiteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMicroVoiceLiteDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMicroVoiceLiteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, mUsername);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, mPassword);
	DDX_Control(pDX, IDC_EDIT_HOST, mHost);
	DDX_Control(pDX, IDC_EDIT_DIAL_NUM, mDialNum);
	DDX_Control(pDX, IDC_STATIC_INFO, mStatus);
	DDX_Control(pDX, IDC_CHECK_HOLD, mHold);
	DDX_Control(pDX, IDC_CHECK_AGC, mAGC);
	DDX_Control(pDX, IDC_CHECK_NR, mNR);
	DDX_Control(pDX, IDC_CHECK_RECORD, mRecord);
	DDX_Control(pDX, IDC_CHECK_AEC, mAEC);
	//DDX_Control(pDX, IDC_EDIT_DTMF, mDTMF);
	DDX_Control(pDX, IDC_COMBO_CAM_LIST, mCamera);
	DDX_Control(pDX, IDC_EDIT1, mVideoWnd);
}

BEGIN_MESSAGE_MAP(CMicroVoiceLiteDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BNT_MAKECALL, &CMicroVoiceLiteDlg::OnBnClickedBntMakecall)
	ON_BN_CLICKED(IDC_BNT_REGISTER, &CMicroVoiceLiteDlg::OnBnClickedBntRegister)
	ON_BN_CLICKED(IDC_BNT_DROPCALL, &CMicroVoiceLiteDlg::OnBnClickedBntDropcall)
	ON_BN_CLICKED(IDC_BNT_UNREGISTER, &CMicroVoiceLiteDlg::OnBnClickedBntUnregister)
	ON_BN_CLICKED(IDCANCEL, &CMicroVoiceLiteDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_HOLD, &CMicroVoiceLiteDlg::OnBnClickedCheckHold)
	ON_BN_CLICKED(IDC_CHECK_AGC, &CMicroVoiceLiteDlg::OnBnClickedCheckAgc)
	ON_BN_CLICKED(IDC_CHECK_NR, &CMicroVoiceLiteDlg::OnBnClickedCheckNr)
	ON_BN_CLICKED(IDC_CHECK_RECORD, &CMicroVoiceLiteDlg::OnBnClickedCheckRecord)
	ON_BN_CLICKED(IDC_CHECK_AEC, &CMicroVoiceLiteDlg::OnBnClickedCheckAec)
	//ON_BN_CLICKED(IDC_BNT_SEND_DTMF, &CMicroVoiceLiteDlg::OnBnClickedBntSendDtmf)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CMicroVoiceLiteDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_DTMF_1, &CMicroVoiceLiteDlg::OnBnClickedButtonDtmf1)
	ON_BN_CLICKED(IDC_BUTTON_DTMF_2, &CMicroVoiceLiteDlg::OnBnClickedButtonDtmf2)
	ON_BN_CLICKED(IDC_BUTTON_DTMF_3, &CMicroVoiceLiteDlg::OnBnClickedButtonDtmf3)
	ON_BN_CLICKED(IDC_BUTTON_DTMF_4, &CMicroVoiceLiteDlg::OnBnClickedButtonDtmf4)
	ON_BN_CLICKED(IDC_BUTTON_DTMF_5, &CMicroVoiceLiteDlg::OnBnClickedButtonDtmf5)
	ON_BN_CLICKED(IDC_BUTTON_DTMF_6, &CMicroVoiceLiteDlg::OnBnClickedButtonDtmf6)
	ON_BN_CLICKED(IDC_BUTTON_DTMF_7, &CMicroVoiceLiteDlg::OnBnClickedButtonDtmf7)
	ON_BN_CLICKED(IDC_BUTTON_DTMF_8, &CMicroVoiceLiteDlg::OnBnClickedButtonDtmf8)
	ON_BN_CLICKED(IDC_BUTTON_DTMF_9, &CMicroVoiceLiteDlg::OnBnClickedButtonDtmf9)
	ON_BN_CLICKED(IDC_BUTTON_DTMF_10, &CMicroVoiceLiteDlg::OnBnClickedButtonDtmf10)
	ON_BN_CLICKED(IDC_BUTTON_DTMF_0, &CMicroVoiceLiteDlg::OnBnClickedButtonDtmf0)
	ON_BN_CLICKED(IDC_BUTTON_DTMF_11, &CMicroVoiceLiteDlg::OnBnClickedButtonDtmf11)
	ON_CBN_SELCHANGE(IDC_COMBO_CAM_LIST, &CMicroVoiceLiteDlg::OnCbnSelchangeComboCamList)
END_MESSAGE_MAP()


// CMicroVoiceLiteDlg 消息处理程序

BOOL CMicroVoiceLiteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	mUsername.SetWindowText("203");
	mPassword.SetWindowText("203");
	mHost.SetWindowText("192.168.1.200");
	mDialNum.SetWindowText("443");
//	mDTMF.SetWindowText("#");
	linphone_core_enable_logs_with_cb(linphone_log_handler);
	
	

	memset(&vTable,0,sizeof(vTable));
	vTable.show = showInterfaceCb;
	vTable.inv_recv = inviteReceivedCb;
	vTable.auth_info_requested = authInfoRequested;
	vTable.display_status = displayStatusCb;
	vTable.display_message = displayMessageCb;
	vTable.display_warning = displayMessageCb;
	vTable.general_state = generalStateChange;
	vTable.network_quality_monitor = network_quality;
	vTable.media_quality_monitor = media_quality;


	the_core = linphone_core_new(	&vTable
		,config_file
		,factoryConfig
		,this);

	linphone_core_set_playback_gain_db(the_core,1.0);

	{//摄像头列表
		const char **video_devices=linphone_core_get_video_devices(the_core);
		const char *curdev = linphone_core_get_video_device(the_core);
		const char *dev = NULL;

		int i=0;
		int selected = -1;
		for(;dev=video_devices[i];i++){
			CString tmp = CString(dev);
			mCamera.AddString(tmp);
			if(curdev && strlen(curdev) && !strcmp(curdev,tmp))
			{
				selected = i;
				linphone_core_set_video_device(the_core,dev);
			}
		}

		mCamera.SetCurSel(selected);
	}

	//设置本地视频窗口
//	linphone_core_set_native_video_window_id(the_core,(unsigned long)mVideoView.m_hWnd);

	if (the_core)
	{
		SetTimer(PHONE_CORE_TIMER,30,NULL);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMicroVoiceLiteDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMicroVoiceLiteDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMicroVoiceLiteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMicroVoiceLiteDlg::OnDestroy()
{
	CDialog::OnDestroy();
	
	KillTimer(PHONE_CORE_TIMER);

	if (the_core!=NULL)
	{
		linphone_core_destroy(the_core);
		the_core=NULL;
	}
	// TODO: 在此处添加消息处理程序代码
}

void CMicroVoiceLiteDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == PHONE_CORE_TIMER && the_core != NULL)
	{
		linphone_core_iterate(the_core);
	}

	CDialog::OnTimer(nIDEvent);
}

void CMicroVoiceLiteDlg::OnBnClickedBntMakecall()
{

	CString num;

	mDialNum.GetWindowText(num);

	if(num.IsEmpty()) return;

	char normalizedUserName[256];

	LinphoneProxyConfig* proxyCfg=NULL;	
	//get default proxy
	linphone_core_get_default_proxy(the_core,&proxyCfg);

	if(proxyCfg!=NULL){
		linphone_proxy_config_normalize_number(proxyCfg,num.GetString(), normalizedUserName,sizeof(normalizedUserName));
		const char * uri = linphone_core_get_identity(the_core);
		LinphoneAddress* tmpAddress = linphone_address_new(uri);
		linphone_address_set_username(tmpAddress,normalizedUserName);
		linphone_address_set_display_name(tmpAddress,"libsipua3");
		const char *url = linphone_address_as_string(tmpAddress);
		linphone_core_invite(the_core,url) ;
		linphone_address_destroy(tmpAddress);
	}
}

void CMicroVoiceLiteDlg::OnBnClickedBntRegister()
{
	CString username,password,server,szidentity;

	mUsername.GetWindowText(username);
	mPassword.GetWindowText(password);
	mHost.GetWindowText(server);

	if (username.IsEmpty() || password.IsEmpty() || server.IsEmpty())
	{
		return;
	}
	
	szidentity.Format("sip:%s@%s",username.GetString(),server.GetString());

	const char *identity = szidentity.GetString();
	const char *secret = password.GetString();
	const char *host = server.GetString();

	LinphoneProxyConfig* proxyCfg=NULL;	
	//get default proxy
	linphone_core_get_default_proxy(the_core,&proxyCfg);

	if (proxyCfg!=NULL)
	{
		if (linphone_proxy_config_is_registered(proxyCfg))
		{
			MessageBox("已注册到服务器！");
			return;
		}else
		{
			linphone_core_clear_proxy_config(the_core);
		}
		
	}
	
	proxyCfg = linphone_proxy_config_new();

	// add username password
	LinphoneAddress *from = linphone_address_new(identity);
	LinphoneAuthInfo *info;
	if (from !=0){
		info=linphone_auth_info_new(linphone_address_get_username(from),NULL,secret,NULL,NULL);
		linphone_core_add_auth_info(the_core,info);
	}
	linphone_address_destroy(from);

	// configure proxy entries
	linphone_proxy_config_set_identity(proxyCfg,identity);
	linphone_proxy_config_set_server_addr(proxyCfg,host);
	linphone_proxy_config_enable_register(proxyCfg,true);


	linphone_proxy_config_set_dial_escape_plus(proxyCfg,TRUE);

	linphone_core_add_proxy_config(the_core,proxyCfg);
	//set to default proxy
	linphone_core_set_default_proxy(the_core,proxyCfg);

}

void CMicroVoiceLiteDlg::OnBnClickedBntDropcall()
{
	linphone_core_terminate_call(the_core,NULL);
}

void CMicroVoiceLiteDlg::OnBnClickedBntUnregister()
{
	/*LinphoneProxyConfig* proxyCfg=NULL;	
	//get default proxy
	linphone_core_get_default_proxy(the_core,&proxyCfg);

	if(proxyCfg)
		linphone_core_remove_proxy_config(the_core,proxyCfg);
	*/

	linphone_core_clear_proxy_config(the_core);
}

void CMicroVoiceLiteDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CMicroVoiceLiteDlg::OnBnClickedCheckHold()
{
	if (mHold.GetCheck() ==1){
		linphone_core_call_holdon(the_core,the_core->call,TRUE);
	}else{
		linphone_core_call_holdon(the_core,the_core->call,FALSE);
	}
	//linphone_core_call_restart_media_stream(the_core,the_core->call);
}

void CMicroVoiceLiteDlg::OnBnClickedCheckAgc()
{
	if (mAGC.GetCheck() ==1){
		linphone_core_enable_agc(the_core,TRUE);
	}else{
		linphone_core_enable_agc(the_core,FALSE);
	}
}

void CMicroVoiceLiteDlg::OnBnClickedCheckNr()
{
	if (mNR.GetCheck() ==1){
		linphone_core_enable_nr(the_core,TRUE);
	}else{
		linphone_core_enable_nr(the_core,FALSE);
	}
}

void CMicroVoiceLiteDlg::OnBnClickedCheckRecord()
{
	if (mRecord.GetCheck() ==1){
		linphone_core_call_start_recoding(the_core,the_core->call,".");
	}else{
		linphone_core_call_stop_recoding(the_core,the_core->call);
	}
}

void CMicroVoiceLiteDlg::OnBnClickedCheckAec()
{
	if (mAEC.GetCheck() ==1){
		linphone_core_enable_echo_cancellation(the_core,TRUE);
	}else{
		linphone_core_enable_echo_cancellation(the_core,FALSE);
	}
}

void CMicroVoiceLiteDlg::OnBnClickedBntSendDtmf()
{
	CString dtmf;
	const char *dtmf_char;

	mDTMF.GetWindowText(dtmf);

	if(dtmf.IsEmpty()) return;

	dtmf_char = dtmf.GetString();
	linphone_core_send_dtmf(the_core,dtmf_char[0]);
}

void CMicroVoiceLiteDlg::OnBnClickedButtonTest()
{
	linphone_core_network_send_quality_test(the_core);
}

void CMicroVoiceLiteDlg::OnBnClickedButtonDtmf1()
{
	linphone_core_send_dtmf(the_core,'1');
}

void CMicroVoiceLiteDlg::OnBnClickedButtonDtmf2()
{
	linphone_core_send_dtmf(the_core,'2');
}

void CMicroVoiceLiteDlg::OnBnClickedButtonDtmf3()
{
	linphone_core_send_dtmf(the_core,'3');
}

void CMicroVoiceLiteDlg::OnBnClickedButtonDtmf4()
{
	linphone_core_send_dtmf(the_core,'4');
}

void CMicroVoiceLiteDlg::OnBnClickedButtonDtmf5()
{
	linphone_core_send_dtmf(the_core,'5');
}

void CMicroVoiceLiteDlg::OnBnClickedButtonDtmf6()
{
	linphone_core_send_dtmf(the_core,'6');
}

void CMicroVoiceLiteDlg::OnBnClickedButtonDtmf7()
{
	linphone_core_send_dtmf(the_core,'7');
}

void CMicroVoiceLiteDlg::OnBnClickedButtonDtmf8()
{
	linphone_core_send_dtmf(the_core,'8');
}

void CMicroVoiceLiteDlg::OnBnClickedButtonDtmf9()
{
	linphone_core_send_dtmf(the_core,'9');
}

void CMicroVoiceLiteDlg::OnBnClickedButtonDtmf10()
{
	linphone_core_send_dtmf(the_core,'*');
}

void CMicroVoiceLiteDlg::OnBnClickedButtonDtmf0()
{
	linphone_core_send_dtmf(the_core,'0');
}

void CMicroVoiceLiteDlg::OnBnClickedButtonDtmf11()
{
	linphone_core_send_dtmf(the_core,'#');
}

void CMicroVoiceLiteDlg::OnCbnSelchangeComboCamList()
{
	CString device;
	mCamera.GetWindowText(device);
	linphone_core_set_video_device(the_core,device.GetString());
}
