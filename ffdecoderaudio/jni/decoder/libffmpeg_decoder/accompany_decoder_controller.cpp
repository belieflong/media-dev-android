#include "accompany_decoder_controller.h"

#define LOG_TAG "AccompanyDecoderController"

AccompanyDecoderController::AccompanyDecoderController() {
	accompanyDecoder = NULL;
	pcmFile = NULL;
}

AccompanyDecoderController::~AccompanyDecoderController() {
}

void AccompanyDecoderController::Init(const char* accompanyPath, const char* pcmFilePath) {
	//初始化两个decoder
	AccompanyDecoder* tempDecoder = new AccompanyDecoder();
	int accompanyMetaData[2];
	tempDecoder->getMusicMeta(accompanyPath, accompanyMetaData);
	delete tempDecoder;
	//初始化伴奏的采样率
	accompanySampleRate = accompanyMetaData[0];
	LOGI("lyl accompanySampleRate = %d",accompanySampleRate);
	int accompanyByteCountPerSec = accompanySampleRate * CHANNEL_PER_FRAME * BITS_PER_CHANNEL / BITS_PER_BYTE; // 16000 * 2 * 16 / 8 = 64000Bps(B/s)
	LOGI("lyl accompanyByteCountPerSec = %d",accompanyByteCountPerSec);
	accompanyPacketBufferSize = (int) ((accompanyByteCountPerSec / 2) * 0.2); // 64000 / 2 * 0.2
	LOGI("lyl accompanyPacketBufferSize = %d",accompanyPacketBufferSize); // 6400
	accompanyDecoder = new AccompanyDecoder();
	accompanyDecoder->init(accompanyPath, accompanyPacketBufferSize);
	pcmFile = fopen(pcmFilePath, "wb+");
}

void AccompanyDecoderController::Decode() {
	while(true) {
		AudioPacket* accompanyPacket = accompanyDecoder->decodePacket();
		if(-1 == accompanyPacket->size) {
			break;
		}
		fwrite(accompanyPacket->buffer, sizeof(short), accompanyPacket->size, pcmFile);
	}
}

void AccompanyDecoderController::Destroy() {
	if (NULL != accompanyDecoder) {
		accompanyDecoder->destroy();
		delete accompanyDecoder;
		accompanyDecoder = NULL;
	}
	if(NULL != pcmFile) {
		fclose(pcmFile);
		pcmFile = NULL;
	}
}
