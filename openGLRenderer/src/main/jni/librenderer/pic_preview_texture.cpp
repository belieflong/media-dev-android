#include "./pic_preview_texture.h"

#define LOG_TAG "PicPreviewTexture"

PicPreviewTexture::PicPreviewTexture() {
}

PicPreviewTexture::~PicPreviewTexture() {
}

bool PicPreviewTexture::createTexture() {
	LOGI("enter PicPreviewTexture::createTexture");
	texture = 0;
	int ret = initTexture();
	if (ret < 0) {
		LOGI("init texture failed...");
		this->dealloc();
		return false;
	}
	return true;
}

void PicPreviewTexture::updateTexImage(byte* pixels, int frameWidth, int frameHeight) {
	if (pixels) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		if (checkGlError("glBindTexture")) {
			return;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		//将该RGBA的数组表示的像素内容 上传到显卡里面 texId所代表的 纹理对象中去 (该纹理对象->PNG图片)
		//pixels -> RGBA数据
		//将png素材放到纹理对象上
	}
}

bool PicPreviewTexture::bindTexture(GLint uniformSampler) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (checkGlError("glBindTexture")) {
		return false;
	}
	glUniform1i(uniformSampler, 0);
	return true;
}

int PicPreviewTexture::initTexture() {
	glGenTextures(1, &texture); //lyl 创建纹理对象 把该纹理对象的句柄 返回给texId
	glBindTexture(GL_TEXTURE_2D, texture); // 绑定纹理对象
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// 纹理的过滤方式 纹理过滤器
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// 双线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// 设置映射规则(重复映射或者简约映射)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// 大于1的纹理 都要设置为1 小于0的纹理都要设置为0
	return 1;
}

bool PicPreviewTexture::checkGlError(const char* op) {
	GLint error;
	for (error = glGetError(); error; error = glGetError()) {
		LOGI("error::after %s() glError (0x%x)\n", op, error);
		return true;
	}
	return false;
}

void PicPreviewTexture::dealloc() {
	LOGI("enter PicPreviewTexture::dealloc");
	if (texture) {
		glDeleteTextures(1, &texture);
	}
}
