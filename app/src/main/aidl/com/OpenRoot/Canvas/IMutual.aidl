// ITestService.aidl
package com.OpenRoot.Canvas;

// Declare any non-default types here with import statements

interface IMutual {
    void setSurface(in Surface surface);
    void navateInputEvent(in MotionEvent motionEvent);
    float getMenuW();
    float getMenuH();
    float getMenuX();
    float getMenuY();
    void setPid(int pid);
    int getPID(String packageName);
}
