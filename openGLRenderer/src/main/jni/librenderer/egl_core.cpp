#include "egl_core.h"

#define LOG_TAG "EGLCore"

EGLCore::EGLCore() {
	pfneglPresentationTimeANDROID = 0;
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
}

EGLCore::~EGLCore() {
}

void EGLCore::release() {
	eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	LOGI("after eglMakeCurrent...");
	eglDestroyContext(display, context);
	LOGI("after eglDestroyContext...");
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
}

void EGLCore::releaseSurface(EGLSurface eglSurface) {
	eglDestroySurface(display, eglSurface);
	eglSurface = EGL_NO_SURFACE;
}

EGLContext EGLCore::getContext(){
	LOGI("return EGLCore getContext...");
	return context;
}

EGLDisplay EGLCore::getDisplay(){
	return display;
}

EGLConfig EGLCore::getConfig(){
	return config;
}

EGLSurface EGLCore::createWindowSurface(ANativeWindow* _window) {
	EGLSurface surface = NULL;
	EGLint format;
	if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
		LOGE("eglGetConfigAttrib() returned error %d", eglGetError());
		release();
		return surface;
	}
	ANativeWindow_setBuffersGeometry(_window, 0, 0, format);
	if (!(surface = eglCreateWindowSurface(display, config, _window, 0))) { // OpenGL的是输出渲染到屏幕上(将EGL和屏幕的设备连接起来)
		LOGE("eglCreateWindowSurface() returned error %d", eglGetError());
	}
	return surface;
}
//离线渲染
EGLSurface EGLCore::createOffscreenSurface(int width, int height) {
	EGLSurface surface;
	EGLint PbufferAttributes[] = { EGL_WIDTH, width, EGL_HEIGHT, height, EGL_NONE, EGL_NONE };
	if (!(surface = eglCreatePbufferSurface(display, config, PbufferAttributes))) {
		LOGE("eglCreatePbufferSurface() returned error %d", eglGetError());
	}
	return surface;
}

int EGLCore::setPresentationTime(EGLSurface surface, khronos_stime_nanoseconds_t nsecs) {
	pfneglPresentationTimeANDROID(display, surface, nsecs);
}

int EGLCore::querySurface(EGLSurface surface, int what) {
	int value = -1;
	eglQuerySurface(display, surface, what, &value);
	return value;
}

bool EGLCore::swapBuffers(EGLSurface eglSurface) {
	return eglSwapBuffers(display, eglSurface); //可以在屏幕上看到OpenGL的渲染的输出了
}

bool EGLCore::makeCurrent(EGLSurface eglSurface) {
	return eglMakeCurrent(display, eglSurface, eglSurface, context);//为该线程绑定绑定Surface、Context
}

void EGLCore::doneCurrent() {
	eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

bool EGLCore::init() {
	return this->init(NULL);
}

bool EGLCore::init(EGLContext sharedContext) { // 初始化 EGL 是 OpenGL ES的窗口管理和上下文环境 的职责
	EGLint numConfigs;
	EGLint width;
	EGLint height;

	const EGLint attribs[] = { EGL_BUFFER_SIZE, 32, EGL_ALPHA_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE };

	if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) { //(封装系统物理屏幕的数据类型) OpenGl ES渲染的目标(可以理解为输出到这里)
		LOGE("eglGetDisplay() returned error %d", eglGetError());
		return false;
	}
	if (!eglInitialize(display, 0, 0)) {
		LOGE("eglInitialize() returned error %d", eglGetError());
		return false;
	}

	if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
		LOGE("eglChooseConfig() returned error %d", eglGetError());
		release();
		return false;
	}
	//EGLContext 创建上下文环境(任何一条OpenGL指令都必须在自己的 上下文环境中的运行)
	EGLint eglContextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	if (!(context = eglCreateContext(display, config, NULL == sharedContext ? EGL_NO_CONTEXT : sharedContext, eglContextAttributes))) {
		LOGE("eglCreateContext() returned error %d", eglGetError());
		release();
		return false;
	}

	pfneglPresentationTimeANDROID = (PFNEGLPRESENTATIONTIMEANDROIDPROC)eglGetProcAddress("eglPresentationTimeANDROID");
	if (!pfneglPresentationTimeANDROID) {
		LOGE("eglPresentationTimeANDROID is not available!");
	}

	return true;
}
