#include "./pic_preview_render.h"

#define LOG_TAG "PicPreviewRender"
enum {
	ATTRIBUTE_VERTEX, ATTRIBUTE_TEXCOORD,
};
PicPreviewRender::PicPreviewRender() {

}
PicPreviewRender::~PicPreviewRender() {

}

bool PicPreviewRender::init(int width, int height, PicPreviewTexture* picPreviewTexture) {
	this->_backingLeft = 0;
	this->_backingTop = 0;
	this->_backingWidth = width;
	this->_backingHeight = height;
	this->picPreviewTexture = picPreviewTexture;
	//初始化shaders、program、textures
	vertShader = 0;
	fragShader = 0;
	program = 0;
	int ret = initShaders();
	if(ret < 0){
		LOGI("init shader failed...");
		this->dealloc();
		return false;
	}
	ret = useProgram();
	if(ret < 0){
		LOGI("use program failed...");
		this->dealloc();
		return false;
	}
	return true;
}

void PicPreviewRender::resetRenderSize(int left, int top, int width, int height) {
	this->_backingLeft = left;
	this->_backingTop = top;
	this->_backingWidth = width;
	this->_backingHeight = height;
}

void PicPreviewRender::render(){
	glViewport(_backingLeft, _backingTop, _backingWidth, _backingHeight); // 固定窗口大小
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(program); //使用显卡绘制程序
	static const GLfloat _vertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
	glVertexAttribPointer(ATTRIBUTE_VERTEX, 2, GL_FLOAT, 0, 0, _vertices); //设置物体坐标 (绑定 vertex 坐标值)
	glEnableVertexAttribArray(ATTRIBUTE_VERTEX); //(启用 vertex)
	static const GLfloat texCoords[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
	glVertexAttribPointer(ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, 0, 0, texCoords);// 设置纹理坐标
	glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD);
	picPreviewTexture->bindTexture(uniformSampler);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);//lyl 渲染管线 (阶段一：指定几何对象)以三角形的形式进行绘制，所有二维图像的渲染都会使用这种方式
}

void PicPreviewRender::dealloc() {
	if (vertShader)
		glDeleteShader(vertShader);

	if (fragShader)
		glDeleteShader(fragShader);

	if(picPreviewTexture){
		picPreviewTexture->dealloc();
	}

	if (program) {
		glDeleteProgram(program);
		program = 0;
	}
}

int PicPreviewRender::useProgram() {
	program = glCreateProgram(); //创建一个对象 作为程序的容器（此函数将返回容器的句柄）
	glAttachShader(program, vertShader);//将前文编译的 Shader 附加到刚刚创建的程序中 参数（程序容器句柄、编译的Shader容器句柄）
	glAttachShader(program, fragShader);
	glBindAttribLocation(program, ATTRIBUTE_VERTEX, "position");
	glBindAttribLocation(program, ATTRIBUTE_TEXCOORD, "texcoord");
	glLinkProgram(program);//链接程序
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);//是否链接成功
	if (status == GL_FALSE) {
		LOGI("Failed to link program %d", program);
		return -1;
	}
	glUseProgram(program); //使用该程序

	uniformSampler = glGetUniformLocation(program, "yuvTexSampler");

	return 1;
}

int PicPreviewRender::initShaders() {
	vertShader = compileShader(GL_VERTEX_SHADER, PIC_PREVIEW_VERTEX_SHADER_2);
	if (!vertShader){
		return -1;
	}
	fragShader = compileShader(GL_FRAGMENT_SHADER, PIC_PREVIEW_FRAG_SHADER_2);
	if (!fragShader){
		return -1;
	}
	return 1;
}

bool PicPreviewRender::checkGlError(const char* op) {
	GLint error;
	for (error = glGetError(); error; error = glGetError()) {
		LOGI("error::after %s() glError (0x%x)\n", op, error);
		return true;
	}
	return false;
}

GLuint PicPreviewRender::compileShader(GLenum type, const char *sources) {
	GLint status;
	GLuint shader = glCreateShader(type); //创建显卡执行程序
	if (shader == 0 || shader == GL_INVALID_ENUM) {
		LOGI("Failed to create shader %d", type);
		return 0;
	}
	glShaderSource(shader, 1, &sources, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status); //验证是否编译成功
	if (status == GL_FALSE) {
		glDeleteShader(shader);
		LOGI("Failed to compile shader : %s\n", sources);
		return 0;
	}
	return shader;
}
