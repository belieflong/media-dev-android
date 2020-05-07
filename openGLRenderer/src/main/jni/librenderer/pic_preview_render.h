#ifndef PIC_PREVIEW_RENDER_H
#define PIC_PREVIEW_RENDER_H
#include "../../libcommon/CommonTools.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "./pic_preview_texture.h"
//lyl 指定几何图形 顶点处理 图元组装 栅格化操作 片元处理 帧缓冲操作
//Shader.frag文件内容 片元着色器(像素着色器)
static const char* PIC_PREVIEW_FRAG_SHADER_2 = //代替(阶段五)的片元处理[Fragment Shader] 用来代替片元处理阶段
		"varying highp vec2 v_texcoord;\n"  //highp一般用于顶点坐标
		"uniform sampler2D yuvTexSampler;\n"//uniform 用于不经常更改的信息
		"void main() {\n"
		"  gl_FragColor = texture2D(yuvTexSampler, v_texcoord);\n" //gl_FragColor片元处理 (取出 二维纹理中该纹理坐标点上的纹理像素值)
		"}\n"; //阶段四：栅格化操作  阶段六：帧缓冲操作

//Shader.vert文件内容 顶点着色器
static const char* PIC_PREVIEW_VERTEX_SHADER_2 =
		"attribute vec4 position;    \n" //attribute 用于经常更改的信息(只能在顶点着器中使用) vec4 四维向量
		"attribute vec2 texcoord;   \n"
		"varying vec2 v_texcoord;     \n"//varying用于修饰从顶点着色器向片元着色器传递的变量
		"void main(void)               \n"
		"{                            \n"
		"   gl_Position = position;  \n" //代替(阶段二)的顶点处理[Vertex Shader代替]、(阶段三)图元组装
		"   v_texcoord = texcoord;  \n"
		"}                            \n";

/**
 * Video OpenGL View
 */
class PicPreviewRender {
protected:
	GLint _backingLeft;
	GLint _backingTop;
	GLint _backingWidth;
	GLint _backingHeight;

	GLuint vertShader;
	GLuint fragShader;

	PicPreviewTexture* picPreviewTexture;
	GLint uniformSampler;
	GLuint program;

	int useProgram();
	int initShaders();
	GLuint compileShader(GLenum type, const char *sources);
	bool checkGlError(const char* op);

public:
	PicPreviewRender();
	virtual ~PicPreviewRender();
	virtual bool init(int width, int height, PicPreviewTexture* picPreviewTexture);
	virtual void render();
	virtual void dealloc();
	void resetRenderSize(int left, int top, int width, int height);
};

#endif //PIC_PREVIEW_RENDER_H
