// WaveIn.cpp : implementation file
//

#include "stdafx.h"
#include "my_vs_chart.h"
#include "WaveIn.h"
#include "my_vs_chartDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include <stdio.h>
using namespace std;

#include "WebRtcMoudle/signal_processing_library.h"
#include "WebRtcMoudle/noise_suppression_x.h"
#include "WebRtcMoudle/noise_suppression.h"
#include "WebRtcMoudle/gain_control.h"

void NoiseSuppression32(char *pInBuffer, char * pOutBuffer,int len, int nSample, int nMode)
{
	int nRet = 0;
	NsHandle *pNS_inst = NULL;
	do
	{
		int i = 0;
		int nFileSize = 0;
		int nTime = 0;
		if (0 != WebRtcNs_Create(&pNS_inst))
		{
			printf("Noise_Suppression WebRtcNs_Create err! \n");
			break;
		}

		if (0 != WebRtcNs_Init(pNS_inst, nSample))
		{
			printf("Noise_Suppression WebRtcNs_Init err! \n");
			break;
		}

		if (0 != WebRtcNs_set_policy(pNS_inst, nMode))
		{
			printf("Noise_Suppression WebRtcNs_set_policy err! \n");
			break;
		}

		nFileSize = len;
		

		int  filter_state1[6], filter_state12[6];
		int  Synthesis_state1[6], Synthesis_state12[6];

		memset(filter_state1, 0, sizeof(filter_state1));
		memset(filter_state12, 0, sizeof(filter_state12));
		memset(Synthesis_state1, 0, sizeof(Synthesis_state1));
		memset(Synthesis_state12, 0, sizeof(Synthesis_state12));

		nTime = GetTickCount();
		for (i = 0; i < nFileSize; i += 640)
		{
			if (nFileSize - i >= 640)
			{
				short shBufferIn[320] = { 0 };

				short shInL[160], shInH[160];
				short shOutL[160] = { 0 }, shOutH[160] = { 0 };

				memcpy(shBufferIn, (char*)(pInBuffer + i), 320 * sizeof(short));
				//首先需要使用滤波函数将音频数据分高低频，以高频和低频的方式传入降噪函数内部
				WebRtcSpl_AnalysisQMF(shBufferIn, 320, shInL, shInH, filter_state1, filter_state12);

				//将需要降噪的数据以高频和低频传入对应接口，同时需要注意返回数据也是分高频和低频
				if (0 == WebRtcNs_Process(pNS_inst, shInL, shInH, shOutL, shOutH))
				{
					short shBufferOut[320];
					//如果降噪成功，则根据降噪后高频和低频数据传入滤波接口，然后用将返回的数据写入文件
					WebRtcSpl_SynthesisQMF(shOutL, shOutH, 160, shBufferOut, Synthesis_state1, Synthesis_state12);
					memcpy(pOutBuffer + i, shBufferOut, 320 * sizeof(short));
				}
			}
		}
	} while (0);

	WebRtcNs_Free(pNS_inst);

}


/////////////////////////////////////////////////////////////////////////////
// CWaveIn

//IMPLEMENT_DYNCREATE(CWaveIn, CWinThread)

CWaveIn::CWaveIn()
{
}

CWaveIn::CWaveIn(SOCKET sock)
{
	CreateThread();
	m_sock = sock;
}

CWaveIn::~CWaveIn()
{
/*	if (m_buf1)
	{
		delete [] m_buf1;
		m_buf1 = NULL;
	}
	if (m_buf2)
	{
		delete [] m_buf2;
		m_buf2 = NULL;
	}*/
}

BOOL CWaveIn::InitInstance()
{
	m_dwThread = ::GetCurrentThreadId();
	Start(m_sock);
	return TRUE;
}

int CWaveIn::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWaveIn, CWinThread)
	//{{AFX_MSG_MAP(CWaveIn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_THREAD_MESSAGE(MM_WIM_OPEN, On_WIM_OPEN)
	ON_THREAD_MESSAGE(MM_WIM_DATA, On_WIM_DATA)
	ON_THREAD_MESSAGE(MM_WIM_CLOSE, On_WIM_CLOSE)
	ON_THREAD_MESSAGE(WM_WAVEINEND, OnWaveInEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveIn message handlers

void CWaveIn::On_WIM_OPEN(UINT wParam, LONG lParam)
{
	m_bRecording = TRUE;

	//******************************************************
	if (m_sock)
	{
		char buf[5];
		DWORD ret = recv(m_sock, buf, 5, 0);
		send(m_sock, (char*)&m_wfx, sizeof(WAVEFORMATEX), 0);
	}
	//******************************************************
}

static int t = 0;
void CWaveIn::On_WIM_DATA(UINT wParam, LONG lParam)
{
	LPWAVEHDR ccpwh = (LPWAVEHDR)lParam;

	char buff[BUF_LEN] = { 0 };
	memset(buff, 0, ccpwh->dwBytesRecorded);
	//******************************************************
	if (m_sock)
	{


		//NoiseSuppression32(ccpwh->lpData, buff, ccpwh->dwBytesRecorded, 32000, 1);

		//将音频写入到文件中
					FILE* fp = fopen("inecord.pcm", "ab+");
				if (fp == NULL)
				{
					printf("fopen error,%d", __LINE__);
				}
				fwrite(((PWAVEHDR)lParam)->lpData, ((PWAVEHDR)lParam)->dwBytesRecorded, 1, fp);
				fclose(fp);
		
// 				FILE* fp_end = fopen("inecord_end.pcm", "ab+");
// 				if (fp_end == NULL)
// 				{
// 					printf("fopen error,%d", __LINE__);
// 				}
// 				fwrite(buff, ((PWAVEHDR)lParam)->dwBytesRecorded, 1, fp_end);
// 				fclose(fp_end);

		DWORD len = send(m_sock, ccpwh->lpData, ccpwh->dwBytesRecorded, 0);
		
		if (len == SOCKET_ERROR)
		{
		
			//	m_bRecording = FALSE;
			PostThreadMessage(WM_WAVEINEND, 0, 0);
		//	AfxEndThread(1,TRUE);
		//	AfxMessageBox("thread end.");
		//	StopRecord();
		//	return;
		}
	}

	//******************************************************
	if (m_bRecording)
	{
		m_ret = ::waveInAddBuffer(m_hwi, ccpwh, sizeof(WAVEHDR));
		if (MMSYSERR_NOERROR != m_ret)
		{
			AfxMessageBox("add buf failed.");
		}
	}
}

void CWaveIn::On_WIM_CLOSE(UINT wParam, LONG lParam)
{
	//******************************************************
	if (m_sock)
		closesocket(m_sock);
	//******************************************************

	if (m_buf1)
	{delete [] m_buf1; m_buf1 = NULL;}

	if (m_buf2)
	{delete [] m_buf2;m_buf2 = NULL;}
}

BOOL CWaveIn::StopRecord()
{
	m_bRecording = FALSE;

	m_ret = ::waveInStop(m_hwi);
	if (MMSYSERR_NOERROR != m_ret)
	{
		CString err = "waveInStop ";
		switch (m_ret)
		{
		case MMSYSERR_INVALHANDLE:
			err += "Specified device handle is invalid.";
			break;
		case MMSYSERR_NODRIVER:
			err += "No device driver is present.";
			break;
		case MMSYSERR_NOMEM:
			err += "Unable to allocate or lock memory.";
			break;
		}
		AfxMessageBox(err);
	}

	m_ret = ::waveInReset(m_hwi);
	if (MMSYSERR_NOERROR != m_ret)
	{
		CString err = "CWaveIn::StopRecord waveInReset ";
		switch (m_ret)
		{
		case MMSYSERR_INVALHANDLE:
			err += "Specified device handle is invalid.";
			break;
		case MMSYSERR_NODRIVER:
			err += "No device driver is present.";
			break;
		case MMSYSERR_NOMEM:
			err += "Unable to allocate or lock memory.";
			break;
		}
		AfxMessageBox(err);
		return FALSE;
	}
	m_ret = ::waveInUnprepareHeader(m_hwi, &m_wh1, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != m_ret)
	{
		CString err = "CWaveIn::On_WIM_DATA waveInUnprepareHeader ";
		switch (m_ret)
		{
		case MMSYSERR_INVALHANDLE:
			err += "Specified device handle is invalid.";
			break;
		case MMSYSERR_NODRIVER:
			err += "No device driver is present.";
			break;
		case MMSYSERR_NOMEM:
			err += "Unable to allocate or lock memory.";
			break;
		case WAVERR_STILLPLAYING:
			err += "The buffer pointed to by the pwh parameter is still in the queue.";
			break;
		}
		AfxMessageBox(err);
		return FALSE;
	}

	m_ret = ::waveInUnprepareHeader(m_hwi, &m_wh2, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != m_ret)
	{
		CString err = "CWaveIn::On_WIM_DATA waveInUnprepareHeader ";;
		switch (m_ret)
		{
		case MMSYSERR_INVALHANDLE:
			err += "Specified device handle is invalid.";
			break;
		case MMSYSERR_NODRIVER:
			err += "No device driver is present.";
			break;
		case MMSYSERR_NOMEM:
			err += "Unable to allocate or lock memory.";
			break;
		case WAVERR_STILLPLAYING:
			err += "The buffer pointed to by the pwh parameter is still in the queue.";
			break;
		}
		AfxMessageBox(err);
		return FALSE;
	}

	

	m_ret = ::waveInClose(m_hwi);
	if (MMSYSERR_NOERROR != m_ret)
	{
		CString err = "CWaveIn::On_WIM_DATA waveInClose ";
		switch (m_ret)
		{
		case MMSYSERR_INVALHANDLE:
			err += "Specified device handle is invalid.";
			break;
		case MMSYSERR_NODRIVER:
			err += "No device driver is present.";
			break;
		case MMSYSERR_NOMEM:
			err += "Unable to allocate or lock memory.";
			break;
		case WAVERR_STILLPLAYING:
			err += "There are still buffers in the queue.";
			break;
		}
		AfxMessageBox(err);
		return FALSE;
	}

	return TRUE;
}

BOOL CWaveIn::Start(SOCKET s)
{
//	AfxMessageBox("T running");
	// TODO:  perform and per-thread initialization here
	// 具有 CD 音质的录音效果
	CString err = "error";
/* Initialize the WAVEFORMATEX for 16-bit, 44KHz, stereo */
	m_wfx.wFormatTag = WAVE_FORMAT_PCM;
	m_wfx.nChannels = 1;
	m_wfx.nSamplesPerSec = 32*1000;
	m_wfx.wBitsPerSample = 16;
	m_wfx.nBlockAlign = m_wfx.nChannels * (m_wfx.wBitsPerSample/8);
	m_wfx.nAvgBytesPerSec = m_wfx.nSamplesPerSec * m_wfx.nBlockAlign;
	m_wfx.cbSize = 0;

/*	m_wfx.wFormatTag = WAVE_FORMAT_PCM;
	m_wfx.nChannels = 1;
	m_wfx.nSamplesPerSec = 8000;
	m_wfx.wBitsPerSample = 8;
	m_wfx.nBlockAlign = m_wfx.nChannels * (m_wfx.wBitsPerSample/8);
	m_wfx.nAvgBytesPerSec = m_wfx.nSamplesPerSec * m_wfx.nBlockAlign;
	m_wfx.cbSize = 0;*/

//	err.Format("%d", m_wfx.nAvgBytesPerSec);
//	AfxMessageBox(err);

/*	m_wfx.cbSize = 0;
	m_wfx.wFormatTag = WAVE_FORMAT_PCM;
	m_wfx.nChannels = 1;
	m_wfx.wBitsPerSample = 8;
	m_wfx.nAvgBytesPerSec = 11025;
	m_wfx.nSamplesPerSec = 11025;
	m_wfx.nBlockAlign = 1;
	*/
	m_ret = ::waveInOpen(&m_hwi, WAVE_MAPPER, &m_wfx, m_dwThread, NULL, CALLBACK_THREAD);
	if (MMSYSERR_NOERROR != m_ret)
	{
		err += "::waveInOpen";
		AfxMessageBox(err);
		return FALSE;
	}

	m_buf1 = new char[BUF_LEN];
	m_buf2 = new char[BUF_LEN];

	ZeroMemory(&m_wh1, sizeof(WAVEHDR));
	ZeroMemory(&m_wh2, sizeof(WAVEHDR));

	m_wh1.lpData = m_buf1;
	m_wh1.dwBufferLength = BUF_LEN;
	m_wh1.dwLoops = 1;

	m_ret = ::waveInPrepareHeader(m_hwi, &m_wh1, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != m_ret)
	{
		err += "::waveInPrepareHeader";
		AfxMessageBox(err);
		return FALSE;
	}

	m_wh2.lpData = m_buf2;
	m_wh2.dwBufferLength = BUF_LEN;
	m_wh2.dwLoops = 1;
	m_ret = ::waveInPrepareHeader(m_hwi, &m_wh2, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != m_ret)
	{
		err += "::waveInPrepareHeader";
		AfxMessageBox(err);
		return FALSE;
	}

	m_ret = ::waveInAddBuffer(m_hwi, &m_wh1, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != m_ret)
	{
		err += "waveInAddBuffer";
		AfxMessageBox(err);
		return FALSE;
	}

	m_ret = ::waveInAddBuffer(m_hwi, &m_wh2, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != m_ret)
	{
		err += "waveInAddBuffer";
		AfxMessageBox(err);
		return FALSE;
	}

	waveInStart(m_hwi);

	return TRUE;
}

void CWaveIn::OnWaveInEnd(WPARAM wParam, LPARAM lParam)
{
//	StopRecord();
//	AfxEndThread(0);
	ExitThread(1);
}
