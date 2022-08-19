#include "Includes.h"
#include "draw.h"

using namespace std;
ImGuiStyle* style;
bool login = false;
int pid = 0;
bool g_Initialized;
ANativeWindow *window;
AAssetManager *assetManager;
char g_LogTag[] = "ImGuiExample";
EGLDisplay g_EglDisplay = EGL_NO_DISPLAY;
EGLSurface g_EglSurface = EGL_NO_SURFACE;
EGLContext g_EglContext = EGL_NO_CONTEXT;
struct MenuPos {
    float w;
    float h;
    float x;
    float y;
} menuPos;
MyInputEvent inputEvent;


const EGLint attribs1[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_BLUE_SIZE, 5,   //-->delete
        EGL_GREEN_SIZE, 6,  //-->delete
        EGL_RED_SIZE, 5,    //-->delete
        EGL_BUFFER_SIZE, 32,  //-->new field
        EGL_DEPTH_SIZE, 16,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
};


#define TAG "testMenu" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"TAG" ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型


void Gini()
{
	g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (g_EglDisplay == EGL_NO_DISPLAY)
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s",
							"eglGetDisplay(EGL_DEFAULT_DISPLAY) returned EGL_NO_DISPLAY");

	if (eglInitialize(g_EglDisplay, 0, 0) != EGL_TRUE)
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s",
							"eglInitialize() returned with an error");

	// const EGLint egl_attributes[] = { EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
	// EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 16 ,EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	// EGL_NONE };
	EGLint num_configs = 0;
	if (eglChooseConfig(g_EglDisplay, attribs1, nullptr, 0, &num_configs) != EGL_TRUE)
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s",
							"eglChooseConfig() returned with an error");
	if (num_configs == 0)
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s",
							"eglChooseConfig() returned 0 matching config");

	// Get the first matching config
	EGLConfig egl_config;
	eglChooseConfig(g_EglDisplay, attribs1, &egl_config, 1, &num_configs);
	EGLint egl_format;
	eglGetConfigAttrib(g_EglDisplay, egl_config, EGL_NATIVE_VISUAL_ID, &egl_format);
	ANativeWindow_setBuffersGeometry(window, 0, 0, egl_format);

	const EGLint egl_context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
	g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT,
									egl_context_attributes);

	if (g_EglContext == EGL_NO_CONTEXT)
		__android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s",
							"eglCreateContext() returned EGL_NO_CONTEXT");

	g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, window, NULL);
	eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);
}

void clearBuffers() {
    glViewport(0, 0, (int) ImGui::GetIO().DisplaySize.x, (int) ImGui::GetIO().DisplaySize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);              // background color
}


void imguiMainWinStart() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();
}

void imguiMainWinEnd() {
    ImGui::Render();
	glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int swapBuffers() {
    //opengl当前buff传递至屏幕
    if (eglSwapBuffers(g_EglDisplay, g_EglSurface)) {
        return 1;
    }
    LOGE("eglSwapBuffers  error = %u", glGetError());
    return 0;
}

void InitMenu();//IMGUI-UI
void TheJniDraw();//绘图

void *init(void *args) {
    if (!g_Initialized) {
		Gini();
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
		
        ImGui::StyleColorsClassic();
        ImGui_ImplAndroid_Init(window);
        ImGui_ImplOpenGL3_Init("#version 300 es");
		
		static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = 3.0;
        icons_config.OversampleV = 3.0;
		
        io.Fonts->AddFontFromMemoryTTF((void *) font_v, font_v_size, 28.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
		io.Fonts->AddFontDefault();
        IM_ASSERT(imFont != NULL);
        // FIXME: Put some effort into DPI awareness
        ImGui::GetStyle().ScaleAllSizes(3.0f);
        g_Initialized = true;
    }
	//颜色初始化();
    clearBuffers();
	//InitTexture();
    while(true) {
       imguiMainWinStart();
       InitMenu();
       TheJniDraw();
       imguiMainWinEnd();
       swapBuffers();
    }
}


bool Draw_Checked;//启动绘图
bool Draw_Box;//方框
bool Draw_Line;//射线
bool Draw_Health;//血量
bool Draw_Name;//名字
bool Draw_Circle;//圆圈
/*************颜色配置*************/
ImColor CoBox = ImColor(0, 255, 255, 255);
ImColor CoText = ImColor(0, 255, 179, 255);
ImColor CoCircle = ImColor(0, 255, 179, 255);

/*************部分申明*************/
static int ioCL = 50;        
void InitMenu()
{
    ImGui::StyleColorsLight();  // 白色主题
    if (ImGui::Begin(u8"哔咔[免费]", 0, ImGuiWindowFlags_NoSavedSettings))
    {
        style = &ImGui::GetStyle();
        style->WindowMinSize = ImVec2(570, 550);
        style->WindowTitleAlign = ImVec2(0.5, 0.5);
        ImGui::Text((u8"游戏进程: %d "), pid);

        if (ImGui::BeginTabBar(u8"菜单", ImGuiTabBarFlags_FittingPolicyScroll))
        {
            if (ImGui::BeginTabItem(u8"玩家绘制"))
            {
                ImGui::Checkbox(u8"方框", &Draw_Box);
                ImGui::SameLine();
                ImGui::Checkbox(u8"射线", &Draw_Line);
                ImGui::SameLine();
                ImGui::Checkbox(u8"血量", &Draw_Health);
                ImGui::SameLine();
                ImGui::Checkbox(u8"名字", &Draw_Name);
                
                ImGui::Checkbox(u8"圆圈", &Draw_Circle);
                
                ImGui::Text(u8"调节圆圈大小");
                ImGui::BulletText(u8"长按快速增加/减少");
                
                ImGui::InputInt(u8"##范围", &ioCL);
                ImGui::SameLine();
                if(ImGui::Button(u8"重置")){
                    ioCL = 50;
                }
                ImGui::BulletText(u8"唯一电报群：https://t.me/gyfzwjfx");
                ImGui::BulletText(u8"了解更多联系：@Lawrence_Lucy");
                ImGui::BulletText(u8"严格打击倒卖,二改等行为");
                ImGui::BulletText(u8"哔咔采用JNIROOT只支持ROOT");
                ImGui::Spacing();   // 间距
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(u8"画笔设置"))
            {              
            if (ImGui::CollapsingHeader("文本颜色")) {
                ImGui::BulletText("文本颜色");
                ImGui::SameLine();
                ImGui::ColorEdit3(" ", (float*)&CoText, ImGuiColorEditFlags_NoInputs);
                ImGui::EndTabItem();
                }
                if (ImGui::CollapsingHeader("圆圈颜色")) {
                ImGui::BulletText("圆圈颜色");
                ImGui::SameLine();
                ImGui::ColorEdit3(" ", (float*)&CoCircle, ImGuiColorEditFlags_NoInputs);
                ImGui::EndTabItem();
                }
                
                ImGui::Spacing();   // 间距
                ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem(u8"调试模式"))
            {              
                ImGui::Spacing();   // 间距
                ImGui::BulletText(u8"TheWindowSize");
                ImGui::Text((u8"窗口大小：%d × %d "),(int)menuPos.w,(int)menuPos.h);
                //ImGui::BulletText(u8"结束运行");          
                ImGui::Spacing();   // 间距
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
    menuPos.w = ImGui::GetWindowWidth();
    menuPos.h = ImGui::GetWindowHeight();
    menuPos.x = ImGui::GetWindowPos().x;
    menuPos.y = ImGui::GetWindowPos().y;
    ImGui::End();
}


void TheJniDraw(){
    ImGui_ImplAndroid_NewFrame();
    DrawText(50,50,CoText,"哔咔[免费绘制]");
    DrawText(50,80,CoText,"唯一电报群组@Mahe063");
    DrawText(50,110,CoText,"唯一电报联系方式@Lawrence_Lucy");
    
    if (Draw_Circle){
    DrawCircle(720/2,1600/2,ioCL,CoCircle,0,3.0f);
    }
}

int32_t handleInputEvent(MyInputEvent inputEvent) {
    return ImGui_ImplAndroid_HandleInputEvent(inputEvent);
}

// Helper to retrieve data placed into the assets/ directory (android/app/src/main/assets)
int GetAssetData(const char *filename, void **outData) {
    int num_bytes = 0;
    AAsset *asset_descriptor = AAssetManager_open(assetManager, filename, AASSET_MODE_BUFFER);
    if (asset_descriptor) {
        num_bytes = AAsset_getLength(asset_descriptor);
        *outData = IM_ALLOC(num_bytes);
        int64_t num_bytes_read = AAsset_read(asset_descriptor, *outData, num_bytes);
        AAsset_close(asset_descriptor);
        IM_ASSERT(num_bytes_read == num_bytes);
    }
    return num_bytes;
}
