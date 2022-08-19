//方框
void DrawRect(float upper_leftX, float upper_leftY, float lower_rightX, float lower_rightY, float a, float r, float g, float b, float rounding, float thickness) {
    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(upper_leftX, upper_leftY), ImVec2(lower_rightX, lower_rightY), ImColor(r, g, b, a), rounding, ImDrawFlags_RoundCornersAll, thickness);
}

void DrawRect(float upper_leftX, float upper_leftY, float lower_rightX, float lower_rightY, ImColor &color, float rounding, float thickness) {
    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(upper_leftX, upper_leftY), ImVec2(lower_rightX, lower_rightY), color, rounding, ImDrawFlags_RoundCornersAll, thickness);
}

void DrawRectCxy(float centerX, float centerY, int width, int high, float a, float r, float g, float b, float rounding, float thickness) {
    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(centerX - (width >> 1), centerY - (high >> 1)), ImVec2(centerX + (width >> 1), centerY + (high >> 1)), ImColor(r, g, b, a), rounding, ImDrawFlags_RoundCornersAll, thickness);
}

void DrawRectCxy(float centerX, float centerY, int width, int high, ImColor &color, float rounding, float thickness) {
    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(centerX - (width >> 1), centerY - (high >> 1)), ImVec2(centerX + (width >> 1), centerY + (high >> 1)), color, rounding, ImDrawFlags_RoundCornersAll, thickness);
}

void DrawRectCxy(float centerX, float centerY, int width, int high, ImColor *color, float rounding, float thickness) {
    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(centerX - (width >> 1), centerY - (high >> 1)), ImVec2(centerX + (width >> 1), centerY + (high >> 1)), *color, rounding, ImDrawFlags_RoundCornersAll, thickness);
}


//填充方框
void DrawRectFilled(float upper_leftX, float upper_leftY, float lower_rightX, float lower_rightY, float a, float r, float g, float b, float rounding) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(upper_leftX, upper_leftY), ImVec2(lower_rightX, lower_rightY), ImColor(r, g, b, a), rounding);
}

void DrawRectFilled(float upper_leftX, float upper_leftY, float lower_rightX, float lower_rightY, ImColor &color, float rounding) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(upper_leftX, upper_leftY), ImVec2(lower_rightX, lower_rightY), color, rounding);
}

void DrawRectFilledCxy(float centerX, float centerY, int width, int high, float a, float r, float g, float b, float rounding) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(centerX - (width >> 1), centerY - (high >> 1)), ImVec2(centerX + (width >> 1), centerY + (high >> 1)), ImColor(r, g, b, a), rounding);
}

void DrawRectFilledCxy(float centerX, float centerY, int width, int high, ImColor &color, float rounding) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(centerX - (width >> 1), centerY - (high >> 1)), ImVec2(centerX + (width >> 1), centerY + (high >> 1)), color, rounding);
}

//线段
void DrawLine(float startx, float starty, float endx, float endy, float a, float r, float g, float b, float thickness) {
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(startx, starty), ImVec2(endx, endy), ImColor(r, g, b, a), thickness);
}

void DrawLine(float startx, float starty, float endx, float endy, ImColor &color, float thickness) {
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(startx, starty), ImVec2(endx, endy), color, thickness);
}
//圆圈
void DrawCircle(float 启始坐标1,float 启始坐标2,float 圆角,ImU32 颜色,int 边,float 粗细){
    ImGui::GetForegroundDrawList()->AddCircle(ImVec2{启始坐标1,启始坐标2}, 圆角, 颜色, 边, 粗细);
}
//文字
void DrawText(float 启始坐标1,float 启始坐标2,ImU32 颜色,char* 文字){
    ImGui::GetForegroundDrawList()->AddText({启始坐标1,启始坐标2}, 颜色,文字,NULL);
}
