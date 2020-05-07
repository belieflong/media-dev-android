#include "mp3_encoder.h"

#define LOG_TAG "Mp3Encoder"

Mp3Encoder::Mp3Encoder() {
}

Mp3Encoder::~Mp3Encoder() {
}

int Mp3Encoder::Init(const char* pcmFilePath, const char *mp3FilePath, int sampleRate, int channels, int bitRate) {
	int ret = -1;
	pcmFile = fopen(pcmFilePath, "rb");
	if(pcmFile) {
		mp3File = fopen(mp3FilePath, "wb");
		if(mp3File) {
			lameClient = lame_init();
			lame_set_in_samplerate(lameClient, sampleRate);
			lame_set_out_samplerate(lameClient, sampleRate);
			lame_set_num_channels(lameClient, channels);
			lame_set_brate(lameClient, bitRate / 1000);
			lame_init_params(lameClient);
			ret = 0;
		}
	}
	return ret;
}

void Mp3Encoder::Encode() {
	int bufferSize = 1024 * 512; // 缓冲区的数据量大小 (512KB)
	short* buffer = new short[bufferSize / 2]; //262144[bit] -> 256[Byte] (262144B -> 256KB)
	short* leftBuffer = new short[bufferSize / 4];
	short* rightBuffer = new short[bufferSize / 4];
	uint8_t* mp3_buffer = new uint8_t[bufferSize];
	int readBufferSize = 0;
	LOGD("sizeof(short) = %zd", sizeof(short));
	//将PCM数据读取到 buffer 中，一次性读 2*(bufferSize / 2)的数据量
	while ((readBufferSize = fread(buffer, 2, bufferSize / 2, pcmFile)) > 0) {
		for (int i = 0; i < readBufferSize; i++) {
			// 读取到的长度 分割为两半：一半左声道、一半右声道，分别填充数据
			if (i % 2 == 0) {
				leftBuffer[i / 2] = buffer[i];
			} else {
				rightBuffer[i / 2] = buffer[i];
			}
		}
		LOGD("readBufferSize = %d", readBufferSize); // 成功读取的元素总数 * size(short) = pcm的总字节数
		//左声道数据、右声道数据、每个通道的样本数量、MP3buffer、buffer大小  ->返回每段数据 编码后的长度，写入文件
		int wroteSize = lame_encode_buffer(lameClient, (short int *) leftBuffer, (short int *) rightBuffer, readBufferSize / 2, mp3_buffer, bufferSize);
		fwrite(mp3_buffer, 1, wroteSize, mp3File);
	}
	delete[] buffer;
	delete[] leftBuffer;
	delete[] rightBuffer;
	delete[] mp3_buffer;
}

void Mp3Encoder::Destory() {
	if(pcmFile) {
		fclose(pcmFile);
	}
	if(mp3File) {
		fclose(mp3File);
		lame_close(lameClient);
	}
}
