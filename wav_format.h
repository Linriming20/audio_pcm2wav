#ifndef __WAV_FORMAT__
#define __WAV_FORMAT__


#ifndef WORD
#define WORD unsigned short
#endif

#ifndef DWORD
#define DWORD unsigned int
#endif


typedef struct _RIFF_HEADER
{
	char szRiffID[4]; 		/* 固定：'R','I','F','F' */
	DWORD dwRiffSize; 		/* 填充数值 = wav文件大小 - "RIFF"4个字节 - dwRiffSize大小4个字节 = wav文件大小 - 8个字节 */
	char szRiffFormat[4]; 	/* 固定：'W','A','V','E' */
}RIFF_HEADER; 	/* 12 Bytes */



typedef struct _WAVE_FORMAT
{
	WORD wFormatTag; 		/* PCM: 0x0001 */
	WORD wChannels; 		/* 声道数: 1/2 */
	DWORD dwSamplesPerSec; 	/* 采样率：8000/16000/441000.. */
	DWORD dwAvgBytesPerSec; /* 每秒所需字节数，采样频率 */
	WORD wBlockAlign; 		/* 数据块对齐单位（每个采样需要的字节数，例：16位、单通道 = 16 / 8 * 1 = 2） */
	WORD wBitsPerSample; 	/* 采样位数：8/16... */
	//WORD wAddtional; 		/* 2个字节附加信息，非必须，由FMT_BLOCK的dwFmtSize设定 */
}WAVE_FORMAT;
typedef struct _FMT_BLOCK
{
	char szFmtID[4]; 		/* 固定：'f','m','t',' ' */
	DWORD dwFmtSize; 		/* WAVE_FORMAT的大小：16或18，18代表包含2个字节附加信息 */
	WAVE_FORMAT wavFormat; 	/* 音频数据格式 */
}FMT_BLOCK;  	/* 24 Bytes 如果不需要WAVE_FORMAT为16字节 */



/* 可选的CHUNK，一些软件会生成 */
typedef struct _FACT_BLOCK
{
	char szFactID[4]; 		/* 固定：'f','a','c','t' */
	DWORD dwFactSize;
	char factData[4];
}FACT_BLOCK; 	/* 12 Bytes */



typedef struct _DATA_BLOCK
{
	char szDataID[4]; 		/* 固定：'d','a','t','a' */
	DWORD dwDataSize; 		/* PCM或其他音频数据的大小 */
}DATA_BLOCK; 	/* 8 Bytes */


#endif

