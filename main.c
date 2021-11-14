#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>


#include "wav_format.h"


#define  BUF_SIZE 	1024



void print_usage(const char *process)
{
	printf("sample: \n"
		   "\t %s -h\n"
		   "\t %s --help\n"
		   "\t %s -f ./test.pcm -r 8000 -b 16 -c 1\n"
		   "\t %s --input_pcmfile=./audio/test.pcm --sample_rate=8000 --sample_bits=16 --channels=1\n",
		   process, process, process, process);
}


int main(int argc, char *argv[])
{
	/* WAV文件的CHUNK */
	RIFF_HEADER riff_header;
	FMT_BLOCK fmt_block;
	DATA_BLOCK data_block;

	/* 输入/输出文件 */
	FILE *fpPcm = NULL;
	FILE *fpWav = NULL;
	char pcmFileName[128] = {0};
	char wavFileName[128] = {0};

	/* 读取/写入文件用到的中间变量 */
	char *buf = (char *)malloc(BUF_SIZE);
	unsigned int readcnt = 0;
	unsigned long pcmFileSize = 0;

	/* PCM参数 */
	unsigned int SampleRate = 0; // 采样率
	unsigned short SampleBits = 0; // 采样位数
	unsigned short Channels = 0; // 声道数


	if(argc <= 1)
	{
		print_usage(argv[0]);
		return -1;
	}

	/* 解析命令行参数  -- 开始 */
	char option = 0;
	int option_index = 0;
	char *short_options = "hf:r:b:c:";
	struct option long_options[] =
    {
        {"help",          no_argument,       NULL, 'h'},
        {"input_pcmfile", required_argument, NULL, 'f'},
        {"sample_rate",   required_argument, NULL, 'r'},
        {"sample_bits",   required_argument, NULL, 'b'},
        {"channels",      required_argument, NULL, 'c'},
        {NULL,            0,                 NULL,  0 },
    };
	while((option = getopt_long_only(argc, argv, short_options, long_options, &option_index)) != -1)
	{
		switch(option)
		{
			case 'h':
				print_usage(argv[0]);
				return 0;
			case 'f':
				strncpy(pcmFileName, optarg, 128);
				strncpy(wavFileName, optarg, 128);
				char *p = strrchr(wavFileName, '.');
				if(p == NULL)
				{
					printf("the input file name should be 'xxx.pcm'");
					return -1;
				}
				strncpy(p, ".wav", 4);
				break;
			case 'r':
				SampleRate = atoi(optarg);
				break;
			case 'c':
				Channels = atoi(optarg);
				break;
			case 'b':
				SampleBits = atoi(optarg);
				break;
			defalut:
				break;
		}
	}
	printf("\n**************************************\n"
		   "input: \n"
		   "\t file name: %s\n"
		   "\t sample rate: %d Hz\n"
		   "\t sample bits: %d bits\n"
		   "\t channels: %d\n"
		   "\t bits per second: %d bps\n"
		   "output: \n"
		   "\t file name: %s\n"
		   "**************************************\n\n",
		  pcmFileName, SampleRate, SampleBits, Channels, SampleRate*SampleBits*Channels, wavFileName);
	/* 解析命令行参数  -- 结束 */


	/* 先打开输入/输出文件，读取输入文件的大小 */
	fpPcm = fopen(pcmFileName, "rb+");
	if(fpPcm == NULL)
	{
		char errMsg[128] = {0};
		snprintf(errMsg, 128, "open file(%s) error", pcmFileName);
		perror(errMsg);
		return -1;
	}
	fseek(fpPcm, 0, SEEK_END);
	pcmFileSize = ftell(fpPcm);
	fseek(fpPcm, 0, SEEK_SET);
	
	fpWav = fopen(wavFileName, "wb+");
	if(fpPcm == NULL)
	{
		char errMsg[128] = {0};
		snprintf(errMsg, 128, "open file(%s) error", wavFileName);
		perror(errMsg);
		return -1;
	}


	/* 填充WAV的CHUNK */
	memset(&riff_header, 0, sizeof(RIFF_HEADER));
	strncpy(riff_header.szRiffID, "RIFF", 4);     // 固定
	// WAV文件数据大小 = wav文件大小 - "RIFF"4个字节 - dwRiffSize大小4个字节 = wav文件大小 - 8个字节
	riff_header.dwRiffSize = pcmFileSize + sizeof(RIFF_HEADER) + sizeof(FMT_BLOCK) + sizeof(DATA_BLOCK) - 
							 strlen("RIFF") - sizeof(riff_header.dwRiffSize);
	strncpy(riff_header.szRiffFormat, "WAVE", 4); // 与后面的"fmt "为固定格式
	fwrite(&riff_header, sizeof(RIFF_HEADER), 1, fpWav);


	memset(&fmt_block, 0, sizeof(FMT_BLOCK));
	strncpy(fmt_block.szFmtID, "fmt ", 4); 											// 固定
	fmt_block.dwFmtSize = sizeof(fmt_block.wavFormat); 								// 16或18，有些软件添加2字节附加信息，这里不需要
	fmt_block.wavFormat.wFormatTag = 0x0001; 										// 代表PCM数据
	fmt_block.wavFormat.wChannels = Channels; 										// 1: 单声道; 2: 双声道
	fmt_block.wavFormat.dwSamplesPerSec = SampleRate; 								// 采样率
	fmt_block.wavFormat.dwAvgBytesPerSec = SampleRate *  SampleBits / 8 * Channels; // 每秒所需字节数
	fmt_block.wavFormat.wBlockAlign = SampleBits / 8 * Channels; 					// 数据块对齐单位
	fmt_block.wavFormat.wBitsPerSample = SampleBits; 								// 每个采样需要的bit数
	fwrite(&fmt_block, sizeof(FMT_BLOCK), 1, fpWav);


	memset(&data_block, 0, sizeof(DATA_BLOCK));
	strncpy(data_block.szDataID, "data", 4); // 固定
	data_block.dwDataSize = pcmFileSize;     // PCM数据的大小
	fwrite(&data_block, sizeof(DATA_BLOCK), 1, fpWav);



	while(!feof(fpPcm))
	{
		readcnt = fread(buf, 1, BUF_SIZE, fpPcm);
		fwrite(buf, 1, readcnt, fpWav);
	}


	free(buf);
	fclose(fpPcm);
	fclose(fpWav);

	printf("\n\033[32mPCM to WAV Success!\033[0m\n");

	return 0;
}


