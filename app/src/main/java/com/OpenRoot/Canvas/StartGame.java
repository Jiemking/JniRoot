package com.OpenRoot.Canvas;

import android.app.Activity;
import android.content.Context;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.os.Build;
import android.os.RemoteException;
import android.os.SystemClock;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.TextView;

public class StartGame {
    public static WindowManager manager;
    public static View editPwdView;
    public static int width;
    public static int height;
    public static View view;
    public static ImageView imageView;
    public static TextView tv;
	
	public static void showFloatWindow(final Activity Activity) {
        manager = (WindowManager) Activity.getSystemService(Context.WINDOW_SERVICE);
		Point point = new Point();
        manager.getDefaultDisplay().getRealSize(point);
        width = Math.max(point.x, point.y);
        height = Math.min(point.x, point.y);
        final WindowManager.LayoutParams lp = new WindowManager.LayoutParams();
        lp.systemUiVisibility = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
			View.SYSTEM_UI_FLAG_FULLSCREEN |
			View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
			View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
			View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
			View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN;
        lp.type = Build.VERSION.SDK_INT >= Build.VERSION_CODES.O ? WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY : WindowManager.LayoutParams.TYPE_SYSTEM_ALERT;
        lp.gravity = Gravity.TOP | Gravity.LEFT;
        lp.format = PixelFormat.TRANSPARENT;
        lp.width = WindowManager.LayoutParams.MATCH_PARENT;
        lp.height = WindowManager.LayoutParams.MATCH_PARENT;
        lp.flags = //WindowManager.LayoutParams.FLAG_SECURE |//防截屏
			WindowManager.LayoutParams.FLAG_DITHER | //抖动(过录屏)
			WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE |//不接受触控
			WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
			WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL |
//                        WindowManager.LayoutParams.FLAG_SPLIT_TOUCH |
			WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED |//硬件加速
			WindowManager.LayoutParams.FLAG_FULLSCREEN |//隐藏状态栏导航栏以全屏(貌似没什么用)
			WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS |//忽略屏幕边界
			WindowManager.LayoutParams.FLAG_LAYOUT_ATTACHED_IN_DECOR |//显示在状态栏上方(貌似高版本无效
			WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN;//布局充满整个屏幕 忽略应用窗口限制

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;//覆盖刘海
        }

        final SurfaceView surfaceView = new SurfaceView(Activity);
        surfaceView.setZOrderOnTop(true);
        surfaceView.getHolder().setFormat(PixelFormat.TRANSPARENT);
        final WindowManager.LayoutParams layoutParams = new WindowManager.LayoutParams();
        layoutParams.type = Build.VERSION.SDK_INT >= Build.VERSION_CODES.O ? WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY : WindowManager.LayoutParams.TYPE_SYSTEM_ALERT;
        layoutParams.gravity = Gravity.TOP | Gravity.LEFT;
        layoutParams.format = PixelFormat.TRANSPARENT;
        layoutParams.width = 770;
        layoutParams.height = 780;
        layoutParams.x = 60;
        layoutParams.y = 60;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            layoutParams.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;//覆盖刘海
        }
        //surfaceView.setBackgroundColor(Color.RED);
        layoutParams.flags = 1024 | 8 | 256 |
			//WindowManager.LayoutParams.FLAG_SECURE |//防截屏
			//WindowManager.LayoutParams.FLAG_DITHER | //抖动(过录屏)
//                WindowManager.LayoutParams.FLAG_SPLIT_TOUCH |
			WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL |
			WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED |//硬件加速
			WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS |//忽略屏幕边界
			WindowManager.LayoutParams.FLAG_LAYOUT_ATTACHED_IN_DECOR;//显示在状态栏上方(貌似高版本无效

        surfaceView.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
					try {
						MainActivity.iTestService.navateInputEvent(event);
						int x = (int) MainActivity.iTestService.getMenuX();
						int y = (int) MainActivity.iTestService.getMenuY();
						int w = (int) MainActivity.iTestService.getMenuW();

						layoutParams.x = x;
						layoutParams.y = y;
						layoutParams.width = w;

						manager.updateViewLayout(surfaceView, layoutParams);
						new Thread(new Runnable() {
								@Override
								public void run() {
									SystemClock.sleep(200);
									Activity.runOnUiThread(new Runnable() {
											public void run() {
												int h = 0;
												try {
													h = (int) MainActivity.iTestService.getMenuH();
												} catch (RemoteException e) {
													e.printStackTrace();
												}
												layoutParams.height = h;
												manager.updateViewLayout(surfaceView, layoutParams);
											}
										});
								}
							}).start();
					} catch (RemoteException e) {
						e.printStackTrace();
					}

					return true;
				}
			});
        runWindow(Activity,lp, surfaceView, layoutParams);
    }

    public static int pid = -1;

    public static void runWindow(final Activity Activity,final WindowManager.LayoutParams lp, final SurfaceView surfaceView, final WindowManager.LayoutParams layoutParams) {
        new Thread(new Runnable() {

				@Override
				public void run() {
					
						try {
							pid = MainActivity.iTestService.getPID("com.tencent.tim");
						} catch (RemoteException e) {
							e.printStackTrace();
						}
						
					
					Activity.runOnUiThread(new Runnable() {
							@Override
							public void run() {
								ESPView dv = new ESPView(Activity);
								manager.addView(dv, lp);
								manager.addView(surfaceView, layoutParams);
							}
						});
				}
			}).start();
	}
}
