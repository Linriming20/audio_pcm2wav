
### 1. 编译

编译时需要编译成32位的可执行程序（int需要指定为4字节），所以如果需要在64位主机上运行该程序，编译时就需要在Makefile上添加`-m32`选项（默认已加），如果运行的主机是32位的则将Makefile上的编译选项`-m32`移除。

```bash
$ make  		# 或者交叉编译: make CC=your-crosscompile-gcc
$ ls -l pcm2wav # 编译生成的可执行程序
```

### 2. 运行

```bash
$ # 查看帮助信息
$ ./pcm2wav -h
$ ./pcm2wav --help
$ 
$ # 转化
$ ./pcm2wav -i ./audio/test_8000_16_1.pcm -r 8000 -b 16 -c 1 -o ./test_8000_16_1.wav
$ ./pcm2wav --input_pcmfile=./audio/test_8000_16_1.pcm --sample_rate=8000 --sample_bits=16 --channels=1 --output_wavfile=./test_8000_16_1.wav
```

### 3. 参考文章

 - https://www.jianshu.com/p/fd43c1c82945

 - https://www.cnblogs.com/ranson7zop/p/7657874.html

### 附录

```
.
├── audio
│   ├── pcm params.png
│   ├── test_8000_16_1.pcm
│   └── test_8000_16_1.wav
├── docs
│   ├── PCM音频数据 - 简书.mhtml
│   ├── WAV文件格式分析.pdf
│   └── wav文件格式分析与详解 - nigaopeng - 博客园.mhtml
├── main.c
├── Makefile
├── README.md
└── wav_format.h
```

