#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <windows.h>
#include <gl/gl.h>

#define GL_COMPILE_STATUS 0x8B81
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define TIMER_ID 400
int w = 0;
int h = 0;

void* original_pixel_array = NULL;

HDC hdc2 = 0;
HGLRC hrc = 0;
HWND hwnd = NULL;
uint32_t shaderProgram = 0;
uint32_t inputTexture = 0;

const char* vertexShaderSource = 
    "#version 120\n"
    "varying vec2 texCoord;\n"
    "void main() {\n"
    "    texCoord = gl_MultiTexCoord0.xy;\n" 
    "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n";

const char* fragmentShaderSource = 
    "#version 120\n"
    "#define TAU 6.2831853\n"
    "const float u_strength = 0.005;\n"
    "uniform float iTime;\n"
    "varying vec2 texCoord;\n"
    "const float time_speed = 0.2;\n"
    "uniform sampler2D u_pixel_texture;\n"
    "void main() {\n"
    "    vec2 uv = vec2(texCoord.x, 1.0 - texCoord.y);\n"
    "    uv.x += sin(uv.y * TAU * 1.0 + iTime*time_speed) * u_strength;\n"
    "    uv.y += sin(uv.x * TAU * 1.0 + iTime*time_speed) * u_strength;\n"
    "	 vec4 color = texture2D(u_pixel_texture, uv);"
    "    gl_FragColor = vec4(color.b, color.g, color.r, color.a);\n"
    "}\n";

int iTimeLocation = 0;
int iResolutionLocation = 0;
int iChannel0Location = 0;

uint32_t (APIENTRY *glCreateShader)(uint32_t type);
uint32_t (APIENTRY *glCreateProgram)();
void (APIENTRY *glUseProgram)(uint32_t type);
void (APIENTRY *glDeleteShader)(uint32_t);
void (APIENTRY *glAttachShader)(uint32_t, uint32_t);
void (APIENTRY *glLinkProgram)(uint32_t);
void (APIENTRY *glShaderSource)(uint32_t shader, int count, const char** src, int* len);
void (APIENTRY *glCompileShader)(uint32_t shader);
void (APIENTRY *glGetShaderiv)(uint32_t shader, uint32_t pname, int* params);
void (APIENTRY *glGetShaderInfoLog)(uint32_t shader, int len, int* retlen, char* infolog);

// Добавьте в начало файла после других объявлений функций
int (APIENTRY *glGetUniformLocation)(uint32_t program, const char* name);
void (APIENTRY *glUniform1f)(int location, float v0);
void (APIENTRY *glUniform2f)(int location, float v0, float v1);
void (APIENTRY *glUniform1i)(int location, int v0);

void opengl_init() {
	glCreateShader = wglGetProcAddress("glCreateShader");
	glShaderSource = wglGetProcAddress("glShaderSource");
	glCompileShader = wglGetProcAddress("glCompileShader");
	glGetShaderiv = wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = wglGetProcAddress("glGetShaderInfoLog");

	glCreateProgram = wglGetProcAddress("glCreateProgram");
	glAttachShader = wglGetProcAddress("glAttachShader");
	glLinkProgram = wglGetProcAddress("glLinkProgram");
	glDeleteShader = wglGetProcAddress("glDeleteShader");

	glUseProgram = wglGetProcAddress("glUseProgram");

	glGetUniformLocation = wglGetProcAddress("glGetUniformLocation");
    glUniform1f = wglGetProcAddress("glUniform1f");
    glUniform2f = wglGetProcAddress("glUniform2f");
    glUniform1i = wglGetProcAddress("glUniform1i");

}

uint32_t create_shader(uint32_t type, const char* src) {
	uint32_t shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);

	char log[1024];
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(shader, 1024, NULL, log);
		printf("%s\n", log);
		return 0;
	}
	return shader;
}

uint32_t create_program() {
	uint32_t vs = create_shader(GL_VERTEX_SHADER, vertexShaderSource);
	uint32_t ps = create_shader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	uint32_t sp = glCreateProgram();

	glAttachShader(sp, vs);
	glAttachShader(sp, ps);
	glLinkProgram(sp);

	iTimeLocation = glGetUniformLocation(sp, "iTime");
    iResolutionLocation = glGetUniformLocation(sp, "iResolution");
    iChannel0Location = glGetUniformLocation(sp, "iChannel0");

	glDeleteShader(vs);
	glDeleteShader(ps);
	return sp;
}

// Вызовите это один раз в конце opengl_setup после создания контекста
void init_input_texture() {
    glGenTextures(1, &inputTexture);
    glBindTexture(GL_TEXTURE_2D, inputTexture);
    
    // Настройки фильтрации (встроенные константы из gl.h)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // 0x80E1 — это GL_BGRA_EXT. Так как в старом gl.h его нет, пишем числом.
    // Выделяем пустую память на видеокарте
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 0x80E1, GL_UNSIGNED_BYTE, NULL);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}


void SetupPixelFormat(HDC input_hdc) {
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelformat = ChoosePixelFormat(input_hdc, &pfd);
	SetPixelFormat(input_hdc, pixelformat, &pfd);
}

__declspec(dllexport) void opengl_setup(int wt, int ht) {
	hwnd = CreateWindow("static", "offscreen", WS_POPUP, 0, 0, wt, ht,
		NULL, NULL, NULL, NULL);
	hdc2 = GetDC(hwnd);
	SetupPixelFormat(hdc2);
	hrc = wglCreateContext(hdc2);
	wglMakeCurrent(hdc2, hrc);

	w = wt;
	h = ht;

	opengl_init();
	init_input_texture();
	shaderProgram = create_program();
}

__declspec(dllexport) void opengl_render(uint32_t* pixels) {

	if(!original_pixel_array) {
		original_pixel_array = malloc(sizeof(uint32_t)*w*h);
		memcpy(original_pixel_array, pixels, sizeof(uint32_t)*w*h);
	}

	wglMakeCurrent(hdc2, hrc);
	static int time = 0;
	time += 1;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);

	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, inputTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, 0x80E1, GL_UNSIGNED_BYTE, original_pixel_array);

	glUseProgram(shaderProgram);

	glUniform1f(iTimeLocation, time);
    glUniform2f(iResolutionLocation, (float)w, (float)h);
    glUniform1i(iChannel0Location, 0);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0f);
	glEnd();

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glFinish();

	wglMakeCurrent(NULL, NULL);
}